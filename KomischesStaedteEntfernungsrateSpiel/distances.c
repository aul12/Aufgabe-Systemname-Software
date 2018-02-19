#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//#define ANALYZEHASHMAP
//#define MAPOUTPUT
#define HASHALGO 0
#define MAX_LINE_LEN 128
#define MAX_NAME_LEN 128
#define NUM_OF_CITIES 8000 //
#define EARTH_RADIUS 6378.137
#define F (1/298.257223563)
#define PI 3.14159265358979323846264338327950 //Alle Stellen!!!
#define DEG2RAD(X) X*PI*(1/180.0)
#define SQ(X) (X*X)

typedef struct Location{
    double longitude;
    double lattitude;
}Location;

typedef struct City{
    char name[MAX_NAME_LEN];
    Location location;

}City;

typedef struct ListElem{
    City gemeinde;
    struct ListElem* next;
}ListElem;

unsigned long int calcHashVal(const char *key) {
    unsigned long int hashval = 0;
    for (const char* cp = key; *cp; ++cp) {
        hashval = (hashval << 3) ^ *cp ^ (hashval >> 28);
    }
    return hashval % NUM_OF_CITIES;
}

unsigned long fowlerNollVo(const char* key){
    unsigned int ret = 2166136261;
    for(const char* c = key; *c != '\0'; c++){
        ret = (16777619 * ret) ^ *c;
    }
    return ret % NUM_OF_CITIES;
}

unsigned long hashFunction(const char *key, unsigned short hashAlgo){
    if(hashAlgo)
        return calcHashVal(key);
    else return fowlerNollVo(key);
}

void append(ListElem** list, City gemeinde){
    ListElem* newElem = calloc(1, sizeof(ListElem));
    if(newElem == NULL){
        printf("Failed to allocate memory!\n");
        abort();
    }
    newElem->gemeinde = gemeinde;
    newElem->next = NULL;
    if(*list == NULL) {
        *list = newElem;
    }
    else{
        ListElem* currElem = *list;
        while (currElem->next){
            currElem = currElem->next;
        }
        currElem->next = newElem;
    }
}

City getCity(ListElem** mem, char* cityName){
    City failure = {"\0", {-1, -1}};
    ListElem* temp = mem[hashFunction(cityName, HASHALGO)];
    if(!temp)
        return failure;
    while (strcmp(temp->gemeinde.name, cityName) && temp->next)
        temp = temp->next;
    return !strcmp(temp->gemeinde.name, cityName) ? temp->gemeinde : failure;
}

double calcDistance(City city1, City city2){
    double f = (city1.location.lattitude + city2.location.lattitude)/2;
    double g = (city1.location.lattitude - city2.location.lattitude)/2;
    double l = (city1.location.longitude - city2.location.longitude)/2;
    f = DEG2RAD(f);
    g = DEG2RAD(g);
    l = DEG2RAD(l);
    double s = SQ(sin(g)) * SQ(cos(l)) + SQ(cos(f)) * SQ(sin(l));
    double c = SQ(cos(g)) * SQ(cos(l)) + SQ(sin(f)) * SQ(sin(l));
    double w = atan2(sqrt(s), sqrt(c));
    double d = 2 * w * EARTH_RADIUS;
    double t = sqrt(s * c) / w;
    double h1 = (3 * t - 1) / (2 * c);
    double h2 = (3 * t + 1) / (2 * s);
    return d * (1 + F * h1 * SQ(sin(f)) * SQ(cos(g)) - F * h2 * SQ(cos(f)) * SQ(sin(g)));
}

short extractCityData(City *city, char *data){
    short delCount = 0;
    for(char* c = data; *c != '\0'; c++){
        if(*c == ':')
            delCount++;
    }
    if(delCount != 2)
        return 1;
    strcpy(city->name, strtok(data, ":"));
    city->location.lattitude = strtof(strtok(NULL, ":"), NULL);
    city->location.longitude = strtof(strtok(NULL, ":"), NULL);
    if(city->location.lattitude <= 0 || city->location.longitude <= 0)
        return 1;
    return 0;
}

int main() {
    ListElem* hashMap[NUM_OF_CITIES];
    for(int i = 0; i < NUM_OF_CITIES; i++){
        hashMap[i] = NULL;
    }
    char line[MAX_LINE_LEN];
    FILE* file = fopen("gemeinden.txt", "r");
    while(fgets(line, MAX_LINE_LEN, file) != NULL){
        City temp;
        if(extractCityData(&temp, line)){
            printf("Could not load table of towns.\n");
            exit(1);
        }
        unsigned long tempHash = hashFunction(temp.name, HASHALGO);
        append(&hashMap[tempHash], temp);
    }
    fclose(file);

    #ifdef ANALYZEHASHMAP
    int maxDepth = 0;
    int countNulls = 0;

    for(int i = 0; i < NUM_OF_CITIES; i++){
        int counter = 0;
        for(ListElem* c = hashMap[i]; c; c = c->next){
            #ifdef MAPOUTPUT
            printf("Name: %s\nBreite: %f\nLänge: %f\n\n", c->gemeinde.name, c->gemeinde.location.lattitude, c->gemeinde.location.longitude);
            #endif
            counter++;
        }
        if(counter > maxDepth)
            maxDepth = counter;
        if(!counter)
            countNulls++;
    }

    printf("Maximale Tiefe: %d\n", maxDepth);
    printf("Leere Stellen: %d\n\n", countNulls);
    #endif

    City start = getCity(hashMap, "Ulm");
    City dest = getCity(hashMap, "Kiel");
    City currentCity = start;

    printf("*** Long chain of short trips ***\n");
    printf("Your objective is to travel from %s to %s\n",
           start.name, dest.name);
    printf("through a long chain of intermediate towns where\n");
    printf("the maximal distance between two consecutive towns\n");
    printf("of your journey is to be minimized.\n");

    double maxDist = 0;

    while (strcmp(currentCity.name, dest.name)){

        printf("You are currently located in %s.\n", currentCity.name);
        printf("Next town? ");

        City nextCity;

        char inputBuf[128];
        if (!fgets(inputBuf, sizeof inputBuf, stdin)) {
            printf("Bye!\n"); exit(1);
        }
        for (char* cp = inputBuf; *cp; ++cp) {
            if (*cp == '\n') {
                *cp = 0; break;
            }
        }

        if(!*(nextCity = getCity(hashMap, inputBuf)).name){
            printf("There is no such town.\n");     //Ich find meine Ausgabe ja schöner...
            //printf("%s is not in the list of cities. Try again...\n", input);
            continue;
        }

        if(!strcmp(currentCity.name, nextCity.name)){
            printf("You are already in %s, try again...\n", currentCity.name);
            continue;
        }

        double dist = calcDistance(currentCity, nextCity);
        printf("Distance from %s to %s: %.1lf km\n",
               currentCity.name, nextCity.name, dist);

        if(dist > maxDist){
            maxDist = dist;
            printf("This is the new maximal distance!\n");
        }
        currentCity = nextCity;
    }
    printf("Welcome to %s!\n", dest.name);
    printf("Your maximal intermediate distance was %.1lf km\n",
           maxDist);
}

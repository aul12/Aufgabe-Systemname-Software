#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>

#define HASH_MAP_LENGTH 128
#define LINE_LENGTH 128

typedef struct City{
    double lat, lng;
    char* name;
    struct City* next;
}City_t;

double dist(City_t c, City_t d) {
    const double R =  6378.137;
    double dLat = M_PI/180*(d.lat-c.lat);
    double dLon = M_PI/180*(d.lng-c.lng);
    double a =
            sin(dLat/2) * sin(dLat/2) +
            cos(M_PI/180*(c.lat)) * cos(M_PI/180*(d.lat)) *
            sin(dLon/2) * sin(dLon/2);
    double e = 2 * atan2(sqrt(a), sqrt(1-a));
    return R * e;
}


unsigned long fowlerNollVo(const char* key){
    unsigned int ret = 2166136261;
    for(const char* c = key; *c != '\0'; c++){
        ret = (16777619 * ret) ^ *c;
    }
    return ret;
}

unsigned long int get_hashval(const char* name) {
    unsigned long int hashval = 0;
    for (const char* cp = name; *cp; ++cp) {
        hashval = (hashval << 3) ^ *cp ^ (hashval >> 28);
    }
    return hashval;
}

unsigned long naiveHash(const char* name) {
    unsigned int sum = 0;
    while(*(name++) != '\0')
        sum += (*name+200);
    return sum;
}

unsigned int hash(char* name) {
    return (unsigned int)(fowlerNollVo(name) % HASH_MAP_LENGTH);
}

City_t* rd_file(char *file, City_t *cities) {
    bool isSet[HASH_MAP_LENGTH] = {false};
    FILE* f = fopen(file, "r");
    if(!f) {
        printf("Could not load table of towns.\n");
        exit(1);
    }
    char buf[LINE_LENGTH];
    while(fgets(buf, sizeof(buf), f)) {
        char* name = strtok(buf, ":");
        double lat = strtod(strtok(NULL, ":"), NULL);
        double lng = strtod(strtok(NULL, ":"), NULL);
        City_t* ncty = (City_t*) malloc(sizeof(City_t));
        if(!ncty) {
            abort();
        }
        ncty->name = (char*) malloc(sizeof(char) * LINE_LENGTH);
        if(!ncty->name) {
            abort();
        }
        strcpy(ncty->name, name); ncty->lat = lat; ncty->lng = lng;
        int hsh = hash(name);
        City_t* tail = cities + hsh;
        if(!isSet[hsh]) {
            cities[hsh] = *ncty;
            isSet[hsh] = true;
        } else {
            while(tail->next) tail = tail->next;
            tail->next = ncty;
        }
    }
    fclose(f);
    return cities;
}

void printHashInfo(City_t map[HASH_MAP_LENGTH]) {
    int empty = 0;
    int maxDepth = 0;
    for(int c=0; c<HASH_MAP_LENGTH; c++) {
        if(!map[c].name) {
            empty ++;
        } else {
            int depth = 1;
            City_t head = map[c];
            while (head.next) {
                head = *head.next;
                depth++;
            }
            if (depth > maxDepth) {
                maxDepth = depth;
            }
        }
    }
    printf("Empty:\t%d\n", empty);
    printf("Max-Depth:\t%d\n", maxDepth);
}

void testHash(const char* file) {
    size_t count[HASH_MAP_LENGTH];
    FILE* f = fopen(file, "r");
    if(!f) {
        printf("Could not load table of towns.\n");
        exit(1);
    }
    char buf[LINE_LENGTH];
    while(fgets(buf, sizeof(buf), f)) {
        char* name = strtok(buf, ":");
        count[hash(name)]++;
    }
    fclose(f);

    size_t histogram[11061];
    int max = 0;
    for(int c=0; c<HASH_MAP_LENGTH; c++) {
        histogram[count[c]]++;
        if(count[c] > max) {
            max = (int) count[c];
        }
    }

    for(int c=0; c<=max; c++) {
        printf("%d: %zu\n", c, histogram[c]);
    }
}

bool get(City_t* map, char* name, City_t* city) {
    int hsh = hash(name);
    City_t ret = map[hsh];
    while (strcmp(ret.name, name) != 0) {
        if(!ret.next) {
            return false;
        }
        ret = *ret.next;
    }
    city->lat = ret.lat; city->lng = ret.lng; city->name = ret.name;
    return true;
}

int main() {
    testHash("gemeinden.txt");
    /*City_t map[HASH_MAP_LENGTH];
    for(int c=0; c<HASH_MAP_LENGTH; c++) {
        map[c].name = NULL;
    }
    rd_file("gemeinden.txt", map);
    printHashInfo(map);

    City_t curr, dest;
    double maxDist = 0;

    if (!get(map, "Ulm", &curr) || !get(map, "Kiel", &dest)) {
        printf("Oops, input data appears to be incomplete.\n");
        exit(1);
    }
    printf("*** Long chain of short trips ***\n");
    printf("Your objective is to travel from %s to %s\n",
           curr.name, dest.name);
    printf("through a long chain of intermediate towns where\n");
    printf("the maximal distance between two consecutive towns\n");
    printf("of your journey is to be minimized.\n");

    while (dist(curr, dest) > DBL_EPSILON) {
        printf("You are currently located in %s.\n", curr.name);
        City_t next; bool townExists = true;
        do {
            printf("Next town? ");
            townExists = true;
            char buf[128];
            if (!fgets(buf, sizeof buf, stdin)) {
                printf("Bye!\n"); exit(1);
            }
            for (char* cp = buf; *cp; ++cp) {
                if (*cp == '\n') {
                    *cp = 0; break;
                }
            }
            if (!get(map, buf, &next)) {
                printf("There is no such town.\n");
                townExists = false;
            }
        } while (!townExists);
        double distance = dist(curr, next);
        printf("Distance from %s to %s: %.1lf km\n", curr.name, next.name, distance);
        if (distance > maxDist) {
            maxDist = distance;
            printf("This is the new maximal distance!\n");
        }
        curr = next;
    }
    printf("Welcome to %s!\n", dest.name);
    printf("Your maximal intermediate distance was %.1lf km\n", maxDist);*/
    return 0;
}
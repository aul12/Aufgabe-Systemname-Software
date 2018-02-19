#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 64
#define CITIES_TO_GUESS 8

typedef struct City {
    char name[MAX_LINE_LENGTH];
    size_t inhabitants;
} City_t;

City_t parse_line(const char *line, size_t len) {
    size_t c = 0;
    City_t stadt;
    for(; c<len && line[c] != ':'; c++) {
        stadt.name[c] = line[c];
    }
    stadt.name[c++] = '\0';
    stadt.inhabitants = 0;
    for(; c<len && line[c] >= '0' && line[c] <= '9'; c++) {
        stadt.inhabitants = stadt.inhabitants * 10 + line[c] - '0';
    }
    return stadt;
}

void read_file(City_t *stadte, int len) {
    FILE* file = fopen("staedte", "r");
    char line[MAX_LINE_LENGTH];
    size_t t = 0;
    for(; t<len && fgets(line, MAX_LINE_LENGTH, file) != NULL; t++) {
        stadte[t] = parse_line(line, MAX_LINE_LENGTH);
    }

    while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        size_t m = rand() % t++;
        if(m < len) {
            stadte[m] = parse_line(line, MAX_LINE_LENGTH);
        }
    }
    fclose(file);
}

void printCities(City_t* cities, size_t len) {
    printf("Remaining citys:\n");
    for(size_t c=0; c<len; c++) {
        printf("%zu:\t%s\n", c, cities[c].name);
    }
}

void printOrderedCities(City_t* cities, size_t len) {
    printf("Still to be sorted:\n");
    for(size_t c=0; c<len; c++) {
        printf("%zu\n\t%s\n", c, cities[c].name);
    }
    printf("%zu\n", len);
}
void removeCity(City_t* cities, size_t len, size_t index) {
    for(;index < len-1; index++) {
        cities[index] = cities[index+1];
    }
}

void addCity(City_t* cities, size_t len, City_t city_to_insert, size_t index) {
    for(;len > index; len--) {
        cities[len] = cities[len-1];
    }
    cities[index] = city_to_insert;
}

bool checkOrder(City_t* cities, size_t len) {
    for(size_t c=0; c<len-1; c++) {
        if(cities[c].inhabitants > cities[c+1].inhabitants) {
            return false;
        }
    }
    return true;
}

int main() {
    srand((unsigned int) time(0));

    City_t cities[CITIES_TO_GUESS];
    read_file(cities, CITIES_TO_GUESS);
    City_t orderedCitys[CITIES_TO_GUESS];
    for(size_t c=0; c < CITIES_TO_GUESS; c++) {
        printOrderedCities(orderedCitys, c);
        printCities(cities, CITIES_TO_GUESS - c);
        size_t cityNumber, slot;
        do {
            printf("What is to be inserted where?\t");
            if(scanf("%zu %zu", &cityNumber, &slot) != 2) {
                printf("Wrong input!\n");
                //exit(1);
                cityNumber = (size_t) -1;
                fflush(stdin);
            }
        } while(cityNumber >= CITIES_TO_GUESS-c || slot > c);

        addCity(orderedCitys, c, cities[cityNumber], slot);
        removeCity(cities, CITIES_TO_GUESS-c, cityNumber);
        if(!checkOrder(orderedCitys, c+1)){
            printf("You lost!\nYour points:%zu\n",c);
            return 0;
        }
    }
    printf("You won!\n");
    return 0;
}

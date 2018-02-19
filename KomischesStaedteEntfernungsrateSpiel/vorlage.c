/* Vorlage fuer das 7. Blatt */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
   double latitude;
   double longitude;
} Coordinates;

#define	PI 3.14159265358979323846

/* formula taken from https://de.wikipedia.org/wiki/Orthodrome */
double compute_distance(Coordinates a, Coordinates b) {
   // FIXME
}

/* compute the hash value for a name */
unsigned long int get_hashval(const char* name) {
   // FIXME
}

typedef struct Town {
   char* name;
   Coordinates coord;
   struct Town* next; // next town with the same hash value
} Town;

// FIXME: declare type Table (for hash table)

void init_table(Table* table) {
   // FIXME
}

/* add town to hash table; return false if anything fails */
bool add_town(Table* table, char* name, Coordinates coord) {
   // FIXME
}

/* lookup name in the hash table; return 0, if not found */
Town* find_town(Table* table, char* name) {
   // FIXME
}

bool load_towns(Table* table, char* filename) {
   // FIXME
}

int main() {
   Table table;
   init_table(&table);
   if (!load_towns(&table, "gemeinden.txt")) {
      printf("Could not load table of towns.\n");
      exit(1);
   }
   Town* start = find_town(&table, "Ulm");
   Town* destination = find_town(&table, "Kiel");
   if (!start || !destination) {
      printf("Oops, input data appears to be incomplete.\n");
      exit(1);
   }
   printf("*** Long chain of short trips ***\n");
   printf("Your objective is to travel from %s to %s\n",
      start->name, destination->name);
   printf("through a long chain of intermediate towns where\n");
   printf("the maximal distance between two consecutive towns\n");
   printf("of your journey is to be minimized.\n");

   Town* current = start; double max_distance = 0;
   while (current != destination) {
      printf("You are currently located in %s.\n", current->name);
      Town* next;
      do {
	 printf("Next town? ");
	 char buf[128];
	 if (!fgets(buf, sizeof buf, stdin)) {
	    printf("Bye!\n"); exit(1);
	 }
	 for (char* cp = buf; *cp; ++cp) {
	    if (*cp == '\n') {
	       *cp = 0; break;
	    }
	 }
	 next = find_town(&table, buf);
	 if (!next) {
	    printf("There is no such town.\n");
	 }
      } while (!next);
      double distance = compute_distance(current->coord, next->coord);
      printf("Distance from %s to %s: %.1lf km\n",
	 current->name, next->name, distance);
      if (distance > max_distance) {
	 max_distance = distance;
	 printf("This is the new maximal distance!\n");
      }
      current = next;
   }
   printf("Welcome to %s!\n", destination->name);
   printf("Your maximal intermediate distance was %.1lf km\n",
      max_distance);
}

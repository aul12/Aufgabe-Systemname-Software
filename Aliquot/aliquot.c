#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_START 100

bool updateNumber(int *number, int input, bool player){
	if(input <= 0 || *number % input != 0 || *number == input){
                printf("%d is not a proper divisor of %d\n",input, *number);
		return false;
	} else {
		*number -= input;
		printf("Number: %d\n", *number);
		if(*number <= 1){
			printf(player?"You lost\n":"You won\n");
			exit(0);
		}
		return true;
	}
}


int main(void) {
	printf("*** Aliquot Game ***\n");

	int number;

	srand(time(0));

	number = rand() % MAX_START + 2;
	printf("Number: %d\n", number);	

	while(number > 1){
		// User thingy
		int input = 0;
	
		bool inputError = false;
		do {
			printf("Your move:");
			if(scanf("%d", &input) != 1){
				while(getchar() != '\n');
				inputError = true;
				
				printf("Invalid input\n");
				exit(1);
			} else {
				inputError = false;
			}
		} while(!updateNumber(&number, input, true) || inputError);
		
		// Computer Thingy
		printf("My move: 1\n");
		updateNumber(&number, 1, false);	//@TODO Error checking if algo more complex
	}
	return 0;
}

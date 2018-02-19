#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_LENGTH 50
#define MIN_LENGTH 10

bool isValid(unsigned long long data, int length, int position){
	int lleft, lright;
	if(!(data & (0b1 << position))){
		return false;
	}
	for(lleft = 0; position - lleft >= 0 && data & (0b1 << (position - lleft)); lleft ++);
	for(lright= 0; position + lright < length && data & (0b1 << (position + lright)); lright ++);

	return lleft != lright;
}

bool movePossible(unsigned long long data, int length){
	for(int c=0; c<length; c++){
		if(isValid(data, length, c)){
			return true;
		}
	}
	return false;
}

void print(unsigned long long data, int length){
	printf(".");
	for(int c=0; c<length; c++){
		if(!(data & (0b1 << c))){
			printf(" ");
		} else {
			if(isValid(data, length, c)){
				printf("%d", c);
			}
		}
		printf(".");
	}
	printf("\n");
}

int main(void){
	srand(time(0));

	int length = rand() % (MAX_LENGTH - MIN_LENGTH) + MIN_LENGTH;

	printf("Length: %d\n", length);

	unsigned long long data = ~0;
	bool playerTurn = true;

	print(data, length);

	while(movePossible(data, length)){
		int input = 0;
		if(playerTurn){
			printf("Your input: \n");
			do{
				if(scanf("%d", &input) != 1){
					exit(1);
				}
			} while(!isValid(data, length, input));
			printf("Your choice: %d\n", input);
		} else {
			while(!isValid(data, length, ++input));
			printf("Computer choice: %d\n", input);
		}
		playerTurn = !playerTurn;
		data &= ~(0b1 << input); 
		
		print(data, length);
	}
	printf(playerTurn?"You":"The computer");
	printf(" lost\n");

	return 0;
}

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifndef GAME_CONF
	#define GAME_CONF "gold-cult.h"
#endif
#include GAME_CONF

#define NUMBER_OF_CHARS 26
#define NUMBER_OF_PLAYERS 2

bool removeElement(char* word, char** words, int* wordsLength, int wordLength) {
	for(int c=0; c<*wordsLength; c++){
		if(strncmp(word, words[c], word_len) == 0) {
			words[c] = words[*wordsLength - 1];
			(*wordsLength)--;
			return true;
		}	
	}
	return false;
}

bool hasWon(int charCount[NUMBER_OF_PLAYERS][NUMBER_OF_CHARS], bool player, int winCount) {
	for(int c=0; c<NUMBER_OF_CHARS; c++){
		if(charCount[player][c] >= winCount) {
			return true;
		}
	}
	return false;
}

void addToCharCount(const char* input, int charCount[NUMBER_OF_PLAYERS][NUMBER_OF_CHARS], bool player, int wordLength) {
	for(int c=0; c<wordLength; c++) {
		charCount[player][input[c]-'a']++;
	}
}

void print(char** words, int wordsLength) {
	printf("\n\nRemaining words:\n");
	for(int c=0; c<wordsLength; c++) {
		printf("%s\t", words[c]);
	}
	printf("\n");
}

void printCharCount(int charCount[NUMBER_OF_PLAYERS][NUMBER_OF_CHARS], bool player){
    printf("Characters count:\n");
    for(int c=0; c<NUMBER_OF_CHARS; c++){
        printf("%c: %d\t",'a'+c, charCount[player][c]);
    }
    printf("\n");
}

int main(void){
	bool player = 0;
	int wordsLength = nof_words;
	printf("Player und so...\n");
	int _player = 0;
	if(scanf("%d", &_player) != 1){
		exit(1);
	}
	if(_player != 0 && _player != 1) {
		exit(1);
	}
	player = (bool)_player;

	int charCount[NUMBER_OF_PLAYERS][NUMBER_OF_CHARS] = {0};

	while(true) {
		print(words, wordsLength);
        	printCharCount(charCount, player);
		char input[word_len];
		do {
			if(player) {
				printf("Player tu was!\n");
				if(scanf(word_format, input) != 1){
					exit(1);
				}
			} else {
				printf("Der Computer tut was!\n");
                		strncpy(input, words[0], word_len);
			}
		} while (!removeElement(input, words, &wordsLength, word_len));
		printf("Input: %.*s\n",word_len, input);
		addToCharCount(input, charCount, player, word_len);
		if(hasWon(charCount, player, winning_count)) {
			break;
		} else if(wordsLength <= 0) {
		    printf("You both lost!\n");
		    exit(0);
		}
		player = !player;
	}

	if(player) {
		printf("You won\n");
	} else {
		printf("You lost!!!!\n");
	}

	return 0;
}

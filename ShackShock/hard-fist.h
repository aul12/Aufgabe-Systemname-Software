#ifndef WORDS_H
#define WORDS_H

char* words[] = {
   "blue",
   "cant",
   "curt",
   "dish",
   "fist",
   "hard",
   "hung",
   "rein",
   "weak",
};
enum { nof_words = sizeof(words)/sizeof(words[0]) };
const unsigned int winning_count = 3;
enum { word_len = 4 };
const char name[] = "hard fist";
const char word_format[] = "%4s";

#endif

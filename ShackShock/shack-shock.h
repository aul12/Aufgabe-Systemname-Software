#ifndef WORDS_H
#define WORDS_H

char* words[] = {
   "align",
   "draft",
   "empty",
   "fount",
   "glory",
   "midge",
   "nurse",
   "shack",
   "shock",
};
enum { nof_words = sizeof(words)/sizeof(words[0]) };
enum { word_len = 5 };
const unsigned int winning_count = 3;
const char name[] = "shack shock";
const char word_format[] = "%5s";

#endif

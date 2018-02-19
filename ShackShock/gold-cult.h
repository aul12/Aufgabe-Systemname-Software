#ifndef WORDS_H
#define WORDS_H

char* words[] = {
   "andy",
   "bent",
   "coin",
   "crud",
   "cult",
   "defy",
   "erik",
   "fort",
   "fuji",
   "gold",
   "keys",
   "kiva",
   "lars",
   "path",
   "show",
   "shun",
};
enum { nof_words = sizeof(words)/sizeof(words[0]) };
const unsigned int winning_count = 4;
enum { word_len = 4 };
const char name[] = "gold cult";
const char word_format[] = "%4s";

#endif

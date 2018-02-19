unsigned long int get_hashval(const char* name) {
   unsigned long int hashval = 0;
   for (const char* cp = name; *cp; ++cp) {
      hashval = (hashval << 3) ^ *cp ^ (hashval >> 28);
   }
   return hashval;
}


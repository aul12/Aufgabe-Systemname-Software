#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <limits.h>
#include <time.h>

#define ERROR(x) {fprintf(stderr, "ERRROR (Line: %d): %s\n", __LINE__, x);exit(1);}
#define DERROR(x) {fprintf(stderr, "ERRROR (Line: %d): %d\n", __LINE__, x);exit(1);}

typedef struct file_info {
    char* name;
    long long int timestamp;
} file_info_t;

void print_usage() {
    printf("findnewest\n"); 
}

void print_list(file_info_t* file_list, size_t file_list_len, bool show_date) {
    for(int c=0; c<file_list_len; c++) {
        if(file_list[c].name != 0) {
            if(show_date) {
                printf("%s\t", ctime((time_t*)&file_list[c].timestamp));
            }
            printf("%s\n", file_list[c].name);
        }
    }
}


void insert(file_info_t* file_list, size_t file_list_len, char* name, long long int timestamp, bool oldest_first) {
    int insert;
    for(int c=0; c<file_list_len; c++) {
        if((!oldest_first && file_list[c].timestamp < timestamp) ||
           (oldest_first && file_list[c].timestamp > timestamp)) { // Upper half
            insert = c;
            break;
        }
    }

    for(size_t c=file_list_len-1; c>insert; c--) {
        file_list[c] = file_list[c-1];
    }
    if(insert >= 0 && insert < file_list_len) {
        file_list[insert] = (file_info_t) {name, timestamp};
    }
}


void traverse(file_info_t* file_list, size_t file_list_len, char* dirname, bool oldest_first) {
    DIR* dir = opendir(dirname);
    if(dir == 0) {
        DERROR(errno); 
    }

    struct dirent* entry;
    while(entry =  readdir(dir)) {
        struct stat stat_res;
        char* fname = (char*)malloc(strlen(dirname) + strlen(entry->d_name) + 2);
        if(fname == 0) {
            ERROR("Malloc fuckup");
        }
        strcpy(fname, dirname);
        fname[strlen(dirname)] = '/';
        strcpy(fname+strlen(dirname)+1, entry->d_name);
        if(stat(fname,&stat_res) != 0) {
            DERROR(errno);
        }
        
        size_t file_type = stat_res.st_mode & S_IFMT;

        if(file_type == S_IFDIR) {
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                traverse(file_list, file_list_len, fname, oldest_first);
            }
        } else if(file_type == S_IFREG) {
            insert(file_list, file_list_len, fname, stat_res.st_ctim.tv_sec, oldest_first);
        }
    }

    if(closedir(dir) != 0) {
        DERROR(errno);
    }
}

int main(int argc, char* argv[]) {
    char* pathname = 0;
    size_t num_of_files = 5;
    bool oldest_first = false;
    bool show_date = false;
    file_info_t* file_list;

    for(size_t c=1; c<argc; c++) {
        size_t paramLen = strlen(argv[c]);        
        if(argv[c][0] != '-') {
            pathname = argv[c];
        } else {
            if(paramLen != 2) {
                print_usage();
                exit(1);
            }
            switch(argv[c][1]) {
                case 'c':
                    if(c < (argc-1)) {
                        num_of_files = (size_t) atol(argv[++c]);
                    } else {
                        print_usage();
                        exit(1);
                    }
                    break;
                case 'd':
                    show_date = true; 
                    break;
                case 'o':
                    oldest_first = true;
                    break;
                default:
                    print_usage();
                    exit(1);
            }
        }        
    }
    if(pathname == 0) {
        pathname = ".";
    }

    file_list = (file_info_t*) malloc(num_of_files * sizeof(file_info_t));
    if(file_list == 0) {
        fprintf(stderr, "malloc issue\n");
        exit(2);
    }
    for(size_t c=0; c<num_of_files; c++) {
        if(oldest_first) {
            file_list[c].timestamp = LLONG_MAX;
        } else {
            file_list[c].timestamp = 0;
        }
        file_list[c].name = 0;
    }

    traverse(file_list, num_of_files, pathname, oldest_first);
    print_list(file_list, num_of_files, show_date);
    return 0;
}

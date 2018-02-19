#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUF_SIZE 64

typedef struct {
    char* name;
    size_t bid;
}bid_t;

bid_t parse(char* buf) {
    bid_t ret;
    ret.bid = 0;
    char* curr = buf;
    do {
        if(*curr == ' ') {
            break;
        } else if(*curr < '0' || *curr > '9') {
            return (bid_t){0,0};
        } else {
            ret.bid = ret.bid*10 + *curr - '0';
        }
    } while(curr++);

    ret.name = (char*)malloc(strlen(curr));
    strcpy(ret.name, curr);
    return ret;
}

void print_bid(bid_t bid) {
    printf("Bid: %zu by %s\n", bid.bid, bid.name);
}

void fd_write(int fd, char* buf) {
    size_t len = strlen(buf);
    ssize_t written = 0;
    do {
        written = write(fd, buf + written, len - written);
        if(written == -1) {
            exit(4);
        }
    }
    while(written != len);
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Missing arguments\n");
        return 1;
    }

    char *dir = argv[1];
    char *name = argv[2];
    char fname[] = "current_bid";
    char tmpname[] = "current_bid.tmp";

    size_t dirlen = strlen(dir);

    char* bid_fname = (char*)malloc(dirlen + sizeof(fname));
    char* tmp_fname = (char*)malloc(dirlen + sizeof(tmpname));

    strcpy(bid_fname, dir);
    strcpy(bid_fname+dirlen, fname);
    strcpy(tmp_fname, dir);
    strcpy(tmp_fname+dirlen, tmpname);

    char buf[BUF_SIZE];
    bid_t last_bid;
    for(int c=0; c<10000; c++) {
        fd_write(1, "Your bid:");

        size_t bid = 0;
        ssize_t len = read(0, buf, BUF_SIZE);
        for(int d=0; d<len; d++) {
            if(buf[d] == 0 || buf[d] == '\n') {
                break;
            } else if(buf[d] < '0' || buf[d] > '9') {
                fprintf(stderr, "Please enter a valid number\n");
            } else {
                bid = bid * 10 + buf[d] - '0';
            }
        }

        int tmp_fd;
        while((tmp_fd = open(tmp_fname, O_RDONLY | O_EXCL | O_CREAT, 0666)) == -1) {
            sleep(1);
            fprintf(stderr, "File is blocked\n");
        }

        int write_fd = open(bid_fname, O_RDWR | O_CREAT | O_TRUNC, 0666);
        if(write_fd == -1) {
            fprintf(stderr, "Cannot open bid-file\n");
        }

        read(write_fd, buf, BUF_SIZE);
        bid_t last_bit = parse(buf);

        if(bid > last_bid.bid) {
            lseek(write_fd, 0, SEEK_SET);
            char stack[BUF_SIZE];
            size_t stack_size;
            for (stack_size = 0; stack_size < BUF_SIZE; stack_size++) {
                stack[stack_size] = (char) ((bid % 10) + '0');
                bid /= 10;
                if (bid == 0) {
                    break;
                }
            }
            for (size_t s = 0; s <= stack_size; s++) {
                buf[s] = stack[stack_size - s];
            }
            buf[++stack_size] = ' ';
            strcpy(buf + ++stack_size, name);
            stack_size += strlen(name);
            buf[stack_size++] = '\0';
            fd_write(write_fd, buf);
        } else {
            fd_write(1, "To late\n");
        }
        close(write_fd);
        close(tmp_fd);
        remove(tmp_fname);
    }
}
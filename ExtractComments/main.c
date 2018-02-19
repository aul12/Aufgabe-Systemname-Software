#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 64

enum STATE {
    CODE, COMMENT_BEGIN, SINGLE_LINE_COMMENT, MULTILINE_COMMENT,
    MULTILINE_END
};

// This is a function
/*
 * This is a multiline comment
 */

void extractAndPrintComments(char c) {
    static enum STATE state;
    switch (state) {
        case CODE:
            if (c == '/') {
                state = COMMENT_BEGIN;
            }
            break;
        case COMMENT_BEGIN:
            if (c == '/') {
                state = SINGLE_LINE_COMMENT;
            } else if (c == '*') {
                state = MULTILINE_COMMENT;
            } else {
                state = CODE;
            }
            break;
        case SINGLE_LINE_COMMENT:
            if (c == '\n') {
                state = CODE;
            } else {
                write(1, &c, 1);
            }
            break;
        case MULTILINE_COMMENT:
            if (c == '*') {
                state = MULTILINE_END;
            } else {
                write(1, &c, 1);
            }
            break;
        case MULTILINE_END:
            if (c == '/') {
                state = CODE;
            } else {
                state = MULTILINE_COMMENT;
            }
            break;
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2){
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
        return 2;
    }

    char buf[BUF_SIZE];
    ssize_t len;
    while(len = read(fd, buf, BUF_SIZE)) {
        for(size_t c=0; c<len; c++) {
            extractAndPrintComments(buf[c]);
        }
    }
    return 0;
}
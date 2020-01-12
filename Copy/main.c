#include <fcntl.h>
#include <zconf.h>

#define BUF_SIZE 4096

#define STDERR(x) write(2, x, sizeof(x)-1)

int main(int argc, char* argv[]) {
    if(argc != 3) {
        STDERR("Options stuff");
        return 1;
    }

    int src_fd = open(argv[1], O_RDONLY);
    int dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    char buf[BUF_SIZE];
    ssize_t len;
    while((len = read(src_fd, buf, BUF_SIZE)) > 0) {
        ssize_t written = 0;
        do {
            written += write(dst_fd, buf+written, (size_t)(len-written));
        } while(written < len);
    }

    if(len == -1) {
        STDERR("Tod und Verderben");
        return 2;
    }

    close(src_fd);
    close(dst_fd);
    return 0;
}

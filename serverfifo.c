#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int errno;
#define FIFO1 "fifo.1" 
#define FIFO2 "fifo.2"
#define PERMS 0666
#define MAXBUFF 1024

void server(int readfd, int writefd) {
    char buff[MAXBUFF];
    int n;

    n = read(readfd, buff, MAXBUFF);
    if (n < 0) {
        perror("\nServer error reading from client\n");
    } else {
        printf("\nServer got: %s from client\n", buff);
        strcpy(buff, "Hello Client, I received your message safely");
        if (write(writefd, buff, strlen(buff)) != strlen(buff)) {
            perror("\nServer error writing to client\n");
        }
    }
}

int main() {
    int readfd, writefd;

    if (mknod(FIFO1, S_IFIFO | PERMS, 0) == -1) {
        perror("upk-1: FIF01 failed to create");
        exit(EXIT_FAILURE);
    }

    if (mknod(FIFO2, S_IFIFO | PERMS, 0) == -1) {
        perror("upk-2: FIFO2 failed to create");
        unlink(FIFO1);
        exit(EXIT_FAILURE);
    }

    if ((writefd = open(FIFO2, 1)) < 0) {
        perror("\nServer program unable to open write FIFO2\n");
        exit(EXIT_FAILURE);
    }

    if ((readfd = open(FIFO1, 0)) < 0) {
        perror("\nServer program unable to open read FIF01\n");
        exit(EXIT_FAILURE);
    }

    server(readfd, writefd);

    close(readfd);
    close(writefd);

    exit(0);
}
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int errno;
#define FIF01 "fifo.1" // To write
#define FIF02 "fifo.2" // To read
#define PERMS 0666
#define MAXBUFF 1024

void client(int readfd, int writefd) {
    char buff[MAXBUFF] = "Welcome to INDIA";
    int n = strlen(buff);

    if (write(writefd, buff, n) != n) {
        printf("\nClient error writing to server\n");
    }

    n = read(readfd, buff, MAXBUFF);
    if (n < 0) {
        printf("\nClient error reading from server\n");
    } else {
        printf("\nClient got: %s from server\n", buff);
    }
}

int main() {
    int readfd, writefd;

    if ((readfd = open(FIF02,0)) < 0) {
        perror("\nChild unable to open read FIF02\n");
        exit(EXIT_FAILURE);
    }

    if ((writefd = open(FIF01,1)) < 0) {
        perror("\nChild unable to open write FIF01\n");
        exit(EXIT_FAILURE);
    }

    client(readfd, writefd);

    close(readfd);
    close(writefd);

    if (unlink(FIF01) < 0) {
        printf("\nClient unable to unlink FIF01\n");
    }

    if (unlink(FIF02) < 0) {
        printf("\nClient unable to unlink FIF02\n");
    }

    exit(0);
}
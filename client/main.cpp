#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>


extern int errno;

int port;
#define BUFFER_SIZE 10000


int main(int argc, char *argv[]) {
    int sd;
    struct sockaddr_in server{};
    char buf[BUFFER_SIZE];

    if (argc != 3) {
        printf("Syntax: %s <server_address> <port>\n", argv[0]);
        return -1;
    }

    port = atoi(argv[2]);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket error.\n");
        return errno;
    }


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    if (connect(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        perror("[client]Error at connect().\n");
        return errno;
    }
    printf("\`033`[0;34m");
    printf("------------Welcome on our local marketplace-------------\n");
    printf("============Type <help> to get the available commands==========\n");


    while (1) {
        fflush(stdout);
        printf("\033[0;34m");
        printf(">");
        printf("\033[1;31m");
        fgets(buf, BUFFER_SIZE, stdin);
        fflush(stdin);
        printf("\033[1;31m");
        printf("[client]Sending to server: %s\n", buf);
        printf("\033[0m");
        buf[strcspn(buf, "\n")] = '\0';
        if (write(sd, &buf, BUFFER_SIZE) <= 0) {
            perror("[client]Error writing to the server.\n");
            return errno;
        }

        bzero(buf, BUFFER_SIZE);

        if (read(sd, &buf, BUFFER_SIZE) < 0) {
            perror("[client]Error reading from the server.\n");
            return errno;
        }
        printf("\033[0;32m");
        printf("[client]: %s\n", buf);
        printf("\033[0m");

        if (strcmp(buf, "You left the server.") == 0) {
            fflush(stdout);
            fflush(stdin);
            close(sd);
            return 0;
        }
        bzero(buf, BUFFER_SIZE);
    }
}

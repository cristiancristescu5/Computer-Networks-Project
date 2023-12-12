#include "Client/client.h"
#include "Database/database.h"
#include <vector>
#include "Utils/utils.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <mutex>

const char *DBURL = "/home/cristi/Desktop/marketplace/marketplace.db";

#define BUFFER_SIZE 10000

#define PORT 4024

extern int errno;

static void *treat(void *);

void execute(void *);

int main() {
    auto *db = new Database(DBURL);
    struct sockaddr_in server{};
    struct sockaddr_in from{};
    int sd;
    pthread_t th[100];
    int i = 0;
    std::mutex mutex;


    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[server]Socket error.\n");
        return errno;
    }
    int on = 1;

    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        perror("[server]Bind error.\n");
        return errno;
    }

    if (listen(sd, 5) == -1) {
        perror("[server]Listen error.\n");
        return errno;
    }
    while (1) {
        int client;
        thData *td;
        int length = sizeof(from);

        printf("[server]Waiting on the port: %d...\n", PORT);
        fflush(stdout);

        if ((client = accept(sd, (struct sockaddr *) &from, reinterpret_cast<socklen_t *>(&length))) < 0) {
            perror("[server]Accept error.\n");
            continue;
        }

        td = new thData(i, client, db, new Client(client), mutex);

        pthread_create(&th[i], nullptr, &treat, td);
        i++;
    }
}


static void *treat(void *arg) {
    thData tdL = *((thData *) arg);
    printf("[thread]- %d - Waiting for the command...\n", tdL.idThread);
    fflush(stdout);
    pthread_detach(pthread_self());
    execute((struct thData *) arg);
    close(tdL.cl);
    close((intptr_t) arg);
    return (nullptr);

}


void execute(void *arg) {
    char buffer[BUFFER_SIZE];
    thData tdL = *((struct thData *) arg);
    Database *db = tdL.db;
    Client *client = tdL.user;
    while (1) {
        if (read(tdL.cl, &buffer, BUFFER_SIZE) <= 0) {
            printf("[Thread %d]\n", tdL.idThread);
            close(client->getClientSocket());
            break;
        }


        printf("[Thread %d]Message from client: %s\n", tdL.idThread, buffer);

        auto *command = new Command(buffer);

        bzero(buffer, BUFFER_SIZE);

        std::string response;

        response = handleClient(client, db, command, tdL.mutex);

        delete command;

        printf("[Thread %d]Sending back to client:%s\n", tdL.idThread, buffer);


        if (write(tdL.cl, response.c_str(), response.size()) <= 0) {
            printf("[Thread %d] ", tdL.idThread);
            close(client->getClientSocket());
            break;
        } else {
            printf("[Thread %d]Message sent successfully.\n", tdL.idThread);
        }
        if(strstr(response.c_str(), "left") != nullptr){
            close(tdL.cl);
            break;
        }
    }

}

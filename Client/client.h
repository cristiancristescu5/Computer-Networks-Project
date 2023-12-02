//
// Created by cristi on 12/1/23.
//

#ifndef PROJECT_CLIENT_H
#define PROJECT_CLIENT_H

#include <string>
#include "../Database/database.h"

class Client {
private:
    std::string name;
    std::string password;
    int id;
    int clientSocket;
public:
    Client(std::string name, std::string password, int clientSocket, int id);

    Client(std::string name, std::string password);

    explicit Client(int clientSocket);

    std::string getName();

    std::string getPassword();

    [[nodiscard]] int getClientSocket() const;

    void setClientSocket(int clientSocket);

    void setID(int id);

    [[nodiscard]] int getId() const;

    void printClient();

    ~Client();
};

std::string addUser(Database db, Client *client);

Client *loginUser(Database db, const std::string &username, const std::string &password);

void deleteUser(Database db, Client *client);

int existsUser(Database*, int);

#endif //PROJECT_CLIENT_H

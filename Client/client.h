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
    std::string getName();
    std::string getPassword();
    [[nodiscard]] int getClientSocket() const;
    void setClientSocket(int clientSocket);
    void setID(int id);
    [[nodiscard]] int getId() const;
    ~Client();
};
std::string addUser(Database db, Client* client);
Client* loginUser(Database db, char* username, char* password);
void deleteUser(Database db, Client* client);


#endif //PROJECT_CLIENT_H

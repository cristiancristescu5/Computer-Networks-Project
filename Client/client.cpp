//
// Created by cristi on 12/1/23.
//
#include <iostream>
#include "client.h"
#include <string>
#include <utility>
#include <stdexcept>

Client::Client(std::string name, std::string password, int clientSocket, int id) {
    this->name = std::move(name);
    this->password = std::move(password);
    this->clientSocket = clientSocket;
    this->id = id;
}

std::string Client::getName() {
    return this->name;
}

std::string Client::getPassword() {
    return this->password;
}

int Client::getClientSocket() const {
    return this->clientSocket;
}

int Client::getId() const {
    return this->id;
}

Client::~Client() {
    delete (this);
}

Client::Client(std::string name, std::string password) {
    this->name = std::move(name);
    this->password = std::move(password);
}

void Client::setClientSocket(int clientSocket) {
    this->clientSocket = clientSocket;
}

void Client::setID(int id) {
    this->id = id;
}

void Client::printClient() {
    std::cout << this->id << " " << this->name << " " << this->password << std::endl;
}

std::string addUser(Database db, Client *client) {
    char *errmsg = nullptr;
    std::string query;
    query.append("insert into users(username, password) values('")
            .append(client->getName()).append("'")
            .append(",")
            .append("'").append(client->getPassword()).append("')");
    int conn;
    try {
        conn = db.getConnection();
    } catch (std::invalid_argument &e) {
        return "There was a problem connecting to the database!";
    }
    conn = sqlite3_exec(db.getDB(), query.c_str(), nullptr, nullptr, &errmsg);
    if (conn != SQLITE_OK) {
        return "The user already exists!";
    } else {
        return "The user has been successfully created!";
    }
}

Client *loginUser(Database db, const std::string &username, const std::string &password) {
    char *errmsg = 0;
    std::string query;
    sqlite3_stmt *stmt;
    query.append("select id, username, password from users where username = '")
            .append(username).append("' and password = '").append(password).append("'");
    int conn;
    try {
        conn = db.getConnection();
    } catch (std::invalid_argument &e) {
        return nullptr;
    }

    conn = sqlite3_prepare_v2(db.getDB(), query.c_str(), -1, &stmt, 0);

    if (conn != SQLITE_OK) {
        return nullptr;
    }
    Client *client = nullptr;
    while ((conn = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *user = (const char *) sqlite3_column_text(stmt, 1);
        const char *pass = (const char *) sqlite3_column_text(stmt, 2);
        client = new Client(std::string(user), std::string(pass));
        client->setID(id);
        return client;
    }
    return nullptr;
}


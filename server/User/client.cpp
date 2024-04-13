//
// Created by cristi on 12/1/23.
//
#include <iostream>
#include "client.h"
#include <string>
#include <utility>
#include <stdexcept>

#define QUERY_SIZE 1024


Client::Client(std::string name, std::string password) {
    this->name = std::move(name);
    this->password = std::move(password);
    this->id = -1;
    this->clientSocket = -1;
}

Client::Client(int clientSocket) {
    this->clientSocket = clientSocket;
    this->id = -1;
}

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

void Client::setClientSocket(int clientSocket) {
    this->clientSocket = clientSocket;
}

void Client::setID(int id) {
    this->id = id;
}

void Client::printClient() {
    std::cout << this->id << " " << this->name << " " << this->password << std::endl;
}

void Client::setName(std::string name) {
    this->name = std::move(name);
}

void Client::setPassword(std::string password) {
    this->password = std::move(password);
}
void Client::removePassword(){
    this->password.clear();
}
void Client::removeName(){
    this->name.clear();
}

std::string addUser(Database *db, const std::string &username, const std::string &password) {
    char *errmsg = nullptr;
    std::string query;
    if(username.size() <= 2){
        return "Username too short.";
    }
    if(password.size() <= 2){
        return "Password too short";
    }
    query.append("insert into users(username, password) values('")
            .append(username).append("'")
            .append(",")
            .append("'").append(password).append("')");
    int conn;
    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "There was a problem connecting to the database!";
    }
    conn = sqlite3_exec(db->getDB(), query.c_str(), nullptr, nullptr, &errmsg);
    if (conn != SQLITE_OK) {
        return "The user already exists!";
    } else {
        return "The user has been successfully created!";
    }
}

int existsUser(Database *db, int id) {
    int conn;
    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        sqlite3_close(db->getDB());
        std::cout << "[server]" << e.what() << std::endl;
        return 0;
    }
    char *query = (char *) malloc(QUERY_SIZE * sizeof(char *));
    sqlite3_stmt *stmt;

    sprintf(query, "select * from users where id = %d;", id);

    conn = sqlite3_prepare_v2(db->getDB(), query, -1, &stmt, 0);
    conn = sqlite3_step(stmt);
    if (conn == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        free(query);
        return 1;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db->getDB());
    free(query);
    return -1;
}

Client *loginUser(Database *db, const std::string &username, const std::string &password) {
    std::string query;
    sqlite3_stmt *stmt;
    query.append("select id, username, password from users where username = '")
            .append(username).append("' and password = '").append(password).append("'");
    int conn;
    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        sqlite3_close(db->getDB());
        return nullptr;
    }

    conn = sqlite3_prepare_v2(db->getDB(), query.c_str(), -1, &stmt, 0);

    if (conn != SQLITE_OK) {
        sqlite3_close(db->getDB());
        sqlite3_finalize(stmt);
        return nullptr;
    }
    Client *client;
    conn = sqlite3_step(stmt);
    if (conn != SQLITE_ROW) {
        sqlite3_close(db->getDB());
        sqlite3_finalize(stmt);
        return nullptr;
    }
    int id = sqlite3_column_int(stmt, 0);
    const char *user = (const char *) sqlite3_column_text(stmt, 1);
    const char *pass = (const char *) sqlite3_column_text(stmt, 2);
    client = new Client(std::string(user), std::string(pass));
    client->setID(id);
    sqlite3_close(db->getDB());
    sqlite3_finalize(stmt);
    return client;
}
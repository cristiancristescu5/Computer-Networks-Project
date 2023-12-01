//
// Created by cristi on 12/1/23.
//
#include "database.h"
#include "string"
#include <sqlite3.h>
#include <stdexcept>
#include <utility>
#include <sqlite3.h>

Database::Database(std::string dbURL) {
    this->dbURL = std::move(dbURL);
}

int Database::getConnection() {
    int connection = sqlite3_open(this->dbURL.c_str(), &this->db);
    if(connection != SQLITE_OK){
        throw std::invalid_argument("invalid database URL!");
    }else{
        return connection;
    }
}

sqlite3* Database::getDB(){
    return this->db;
}
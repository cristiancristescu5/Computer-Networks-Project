//
// Created by cristi on 12/1/23.
//

#ifndef PROJECT_DATABASE_H
#define PROJECT_DATABASE_H
#include <sqlite3.h>
#include <string>
class Database{
private:
    sqlite3* db;
    std::string dbURL;
public:
    Database(std::string dbURL);
    int getConnection();
    sqlite3* getDB();
};
#endif //PROJECT_DATABASE_H

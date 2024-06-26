//
// Created by cristi on 12/2/23.
//
#pragma once
#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include <string>
#include <array>
#include "../User//client.h"
#include "command.h"
#include "../Article/article.h"
#include <vector>
#include <mutex>

extern std::vector<Client*> clients;

struct thData {
    int idThread;
    int cl;
    Database *db;
    Client *user;
    std::mutex &mutex;

    thData(int id, int c, Database *d,
           Client *us, std::mutex &m) : mutex(m),
           idThread(id), cl(c), db(d), user(us) {};
};

const std::vector<std::string> categories = {"vehicles", "electronic devices", "beauty", "household", "sport",
                                             "miscellaneous"};

const std::string FORBIDDEN = "You cannot perform this action. Log in first.";

std::string handleClient(Client *client, Database *db, Command *command, std::mutex &);

std::string login(Database *db, const std::string &name, const std::string &password, Client *client);

std::string registerUser(Database *db, const std::string &name, const std::string &password, Client *client);

std::string logOut(Client *client);

std::string addArticle(article *ad, Client *client, Database *db);

std::string help();

int isLogged(Client *client);

#endif //PROJECT_UTILS_H

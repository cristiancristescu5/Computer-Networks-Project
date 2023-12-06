//
// Created by cristi on 12/2/23.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include <string>
#include <array>
#include "../Client/client.h"
#include "command.h"
#include "../Article/article.h"
#include <vector>
#include <mutex>

struct thData {
    int idThread;
    int cl;
    Database *db;
    Client *user;
    std::mutex &mutex;

    thData(int id, int c, Database *d, Client *us, std::mutex &m) : mutex(m), idThread(id), cl(c), db(d), user(us) {};
};

const std::string vec_status[] = {"sold", "unsold"};
const std::vector<std::string> categories = {"vehicles", "electronic devices", "beauty", "household", "sport",
                                             "miscellaneous"};

const std::string FORBIDDEN = "You cannot perform this action. Log in first.";

//thread -> Client
std::string handleClient(Client *client, Database *db, Command *command, std::mutex&);

std::string login(Database *db, const std::string &name, const std::string &password, Client *client);

std::string registerUser(Database *db, const std::string &name, const std::string &password, Client *client);

std::string logOut(Client *client);

std::string addArticle(article *ad, Client *client, Database *db);

std::string buyArticle(int articleId, int userId, Client *client);

std::string getAllAds();

std::string getAdsById(int adId);

std::string getUserAds(int userId);

std::string help();

int isLogged(Client *client);

std::string quit();

#endif //PROJECT_UTILS_H

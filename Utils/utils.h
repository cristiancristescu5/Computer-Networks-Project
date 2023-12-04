//
// Created by cristi on 12/2/23.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include <string>
#include <array>
#include "../Client/client.h"
#include "../Ad/Article.h"
#include "command.h"
#include <vector>

const std::string vec_status[] = {"sold", "unsold"};
const std::vector<std::string> categories = {"vehicles", "electronic devices", "beauty", "household", "sport", "miscellaneous"};

//thread -> Client
std::string handleClient(Client *client, Database *db, Command *command);

std::string login(Database *db, const std::string &name, const std::string &password, Client *client);

std::string registerUser(Database *db, const std::string &name, const std::string &password, Client *client);

std::string logOut(Client *client);

std::string addArticle(Article *ad, Client *client, Database *db);

std::string removeAdvertisement(int userId, int adId);

std::string buyArticle(int articleId, int userId, Client *client);

std::string getAllAds();

std::string getAdsById(int adId);

std::string getUserAds(int userId);

std::string help();

int isLogged(Client *client);

std::string quit();

#endif //PROJECT_UTILS_H

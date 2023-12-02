//
// Created by cristi on 12/2/23.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H
#include <string>
#include <array>
#include "../Client/client.h"
#include "../Ad/Advertisement.h"

const std::string vec_status[] = {"sold", "unsold"};
const std::string categories[] = {"vehicles", "electronic devices", "beauty", "household", "sport", "miscellaneous"};
//thread -> Client
std::array<std::string, 10> parseRequest(char* request);
Client* loginUser(std::string name, std::string password, int fd);
std::string logOut(Client* client);
std::string addAdvertisement(Advertisement* ad, int ownId);
std::string removeAdvertisement(int userId, int adId);
std::string buyArticle(int articleId, int userId, Client* client);
std::string getAllAds();
std::string getAdsById(int adId);
std::string getUserAds(int userId);
int isLogged(Client* client);

#endif //PROJECT_UTILS_H

//
// Created by cristi on 12/1/23.
//

#ifndef PROJECT_ADVERTISEMENT_H
#define PROJECT_ADVERTISEMENT_H

#include <iostream>
#include <string>
#include "../Database/database.h"
#include "../Client/client.h"

class Advertisement {
private:
    int id;
    std::string description;
    float price;
    int ownerID;
    std::string status;
    std::string category;
public:
    Advertisement(int id, std::string description, float price, int ownerId, std::string status, std::string category);

    Advertisement(std::string description, float price, int ownerId, std::string status, std::string category);

    Advertisement(std::string description, float price, std::string status, std::string category);

    std::string getDescription();

    std::string getStatus();

    int getId();

    void setId(int id);

    float getPrice();

    int getOwnerId();

    std::string getCategory();

    void setOwnerId(int id);

    std::string toString();
};

std::string addAdvertisement(Advertisement *advertisement, int ownerId, Database *db);

std::string getAllAdvertisements(Database *db);

std::string buyArticle(int id);

std::string updateAd(Advertisement *newAdd);

std::string removeAd(Database *db, int id, Client *client);


#endif //PROJECT_ADVERTISEMENT_H

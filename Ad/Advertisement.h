//
// Created by cristi on 12/1/23.
//

#ifndef PROJECT_ADVERTISEMENT_H
#define PROJECT_ADVERTISEMENT_H

#include <iostream>
#include <string>
#include "../Database/database.h"

class Advertisement {
private:
    int id;
    std::string description;
    float price;
    int ownerID;
    std::string status;
    int catId;
public:
    Advertisement(int id, std::string description, float price, int ownerId, std::string status, int catId);

    Advertisement(std::string description, float price, int ownerId, std::string status, int catId);

    Advertisement(std::string description, float price, std::string status, int catId);

    std::string getDescription();

    std::string getStatus();

    int getId();

    void setId(int id);

    float getPrice();

    int getOwnerId();

    int getCatId();

    void setOwnerId(int id);

    std::string toString();
};

std::string addAdvertisement(Advertisement *advertisement, int ownerId, Database *db);

std::string getAllAdvertisements();

std::string buyArticle(int id);

std::string updateAd(Advertisement newAdd);


#endif //PROJECT_ADVERTISEMENT_H

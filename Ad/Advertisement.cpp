//
// Created by cristi on 12/1/23.
//
#include "Advertisement.h"
#include <string>
#include <utility>
#include <stdexcept>
#include "../Client/client.h"

#define QUERY_SIZE 1024

Advertisement::Advertisement(std::string description, float price, int ownerId, std::string status,
                             std::string category) {
    this->description = std::move(description);
    this->price = price;
    this->ownerID = ownerId;
    this->status = std::move(status);
    this->category = std::move(category);
}

Advertisement::Advertisement(int id, std::string description, float price, int ownerId, std::string status,
                             std::string category) {
    this->id = id;
    this->description = std::move(description);
    this->price = price;
    this->ownerID = ownerId;
    this->status = std::move(status);
    this->category = std::move(category);
}

Advertisement::Advertisement(std::string description, float price, std::string status, std::string category) {
    this->description = std::move(description);
    this->price = price;
    this->status = std::move(status);
    this->category = std::move(category);
}

std::string Advertisement::getDescription() {
    return this->description;
}

std::string Advertisement::getStatus() {
    return this->status;
}

int Advertisement::getId() {
    return this->id;
}

void Advertisement::setId(int id) {
    this->id = id;
}

float Advertisement::getPrice() {
    return this->price;
}

int Advertisement::getOwnerId() {
    return this->ownerID;
}


void Advertisement::setOwnerId(int id) {
    this->ownerID = id;
}

std::string Advertisement::toString() {
    std::string response;
    response.append("Id: ").append(std::to_string(this->id)).append("\n")
            .append("Owner: ").append(std::to_string(this->ownerID)).append("\n")
            .append("Price: ").append(std::to_string(this->price)).append("\n")
            .append("Category: ").append(this->category).append("\n")
            .append("Description: ").append(this->description)

            .append("\n-----------------------------------------------------------");
    return response;
}

std::string Advertisement::getCategory() {
    return this->category;
}

std::string addAdvertisement(Advertisement *advertisement, int ownerId, Database *db) {
    int conn;
    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "There was a problem connecting to the database";
    }
    int res = existsUser(db, ownerId);
    if (res == 0 || res == -1) {
        return "Failed to publish the ad. User does not exist or database error.";
    }
    advertisement->setOwnerId(ownerId);
    char *errmsg = nullptr;
    char q[QUERY_SIZE];
    sprintf(q, "insert into advertisements(description, price, owner_id, category) values('%s', %f, %d, '%s')",
            advertisement->getDescription().c_str(), advertisement->getPrice(), ownerId,
            advertisement->getCategory().c_str());
    conn = sqlite3_exec(db->getDB(), q, nullptr, nullptr, &errmsg);

//    conn = sqlite3_exec(db->getDB(), "COMMIT;", nullptr, nullptr, &errmsg);
    sqlite3_close(db->getDB());
    if (conn == SQLITE_OK) {
        return "The advertisement has been published successfully.";
    } else {
        return "Failed to publish the add.";
    }
}


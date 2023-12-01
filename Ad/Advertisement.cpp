//
// Created by cristi on 12/1/23.
//
#include "Advertisement.h"
#include <string>
#include <utility>
#include <stdexcept>

using namespace std::string_literals;

Advertisement::Advertisement(std::string description, float price, int ownerId, std::string status, int catId) {
    this->description = std::move(description);
    this->price = price;
    this->ownerID = ownerId;
    this->status = std::move(status);
    this->catId = catId;
}

Advertisement::Advertisement(int id, std::string description, float price, int ownerId, std::string status, int catId) {
    this->id = id;
    this->description = std::move(description);
    this->price = price;
    this->ownerID = ownerId;
    this->status = std::move(status);
    this->catId = catId;
}

Advertisement::Advertisement(std::string description, float price, std::string status, int catId) {
    this->description = std::move(description);
    this->price = price;
    this->status = std::move(status);
    this->catId = catId;
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

int Advertisement::getCatId() {
    return this->catId;
}

void Advertisement::setOwnerId(int id) {
    this->ownerID = id;
}

std::string Advertisement::toString() {
    std::string response;
    response.append("Id: ").append(std::to_string(this->id)).append("\n")
            .append("Owner: ").append(std::to_string(this->ownerID)).append("\n")
            .append("Price: ").append(std::to_string(this->price)).append("\n")
            .append("Description: ").append(this->description)
            .append("\n-----------------------------------------------------------");
    return response;
}

std::string addAdvertisement(Advertisement *advertisement, int ownerId, Database *db) {
    advertisement->setOwnerId(ownerId);
    int conn;
    char *errmsg = 0;
    std::string query;
    try {
        conn = db->getConnection();
    } catch(std::invalid_argument &e){
        return "There was a problem connecting to the database";
    }

    query.append("insert into advertisements(description, price, owner_id, category_id), values('")
    .append(advertisement->getDescription()).append("',")
    .append(std::to_string(advertisement->getPrice())).append(",").append(std::to_string(ownerId))
    .append(",").append(std::to_string(advertisement->getCatId()));
    conn = sqlite3_exec(db->getDB(), query.c_str(), nullptr, nullptr, &errmsg);

    if(conn != SQLITE_OK){
        return "Failed to add the advertisement on the platform";
    }else{
        return "Advertisement published successfully";
    }
}


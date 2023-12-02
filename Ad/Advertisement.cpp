//
// Created by cristi on 12/1/23.
//
#include "Advertisement.h"
#include <string>
#include <utility>
#include <stdexcept>
#include <cstring>
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

std::string Advertisement::getCategory() {
    return this->category;
}

std::string Advertisement::toString() {
    std::string response;
    response.append("Id: ").append(std::to_string(this->id)).append("\n")
            .append("Owner: ").append(std::to_string(this->ownerID)).append("\n")
            .append("Price: ").append(std::to_string(this->price)).append("\n")
            .append("Category: ").append(this->category).append("\n")
            .append("Description: ").append(this->description)

            .append("\n-----------------------------------------------------------\n");
    return response;
}

std::string addAdvertisement(Advertisement *advertisement, int ownerId, Database *db) {
    int conn;
    try {//obtain connection
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "There was a problem connecting to the database";
    }

    int res = existsUser(db, ownerId);//verify if the user exists

    if (res == 0 || res == -1) {
        return "Failed to publish the ad. User does not exist or database error.";
    }

    advertisement->setOwnerId(ownerId);

    char *errmsg = nullptr;
    char q[QUERY_SIZE];

    sprintf(q, "insert into advertisements(description, price, owner_id, category) values('%s', %f, %d, '%s')",
            advertisement->getDescription().c_str(), advertisement->getPrice(), ownerId,
            advertisement->getCategory().c_str());//preparing the query

    conn = sqlite3_exec(db->getDB(), q, nullptr, nullptr, &errmsg);//executing the query

    sqlite3_close(db->getDB());//closing connection

    if (conn == SQLITE_OK) {
        return "The advertisement has been published successfully.";
    } else {
        return "Failed to publish the add.";
    }
}

std::string getAllAdvertisements(Database *db) {
    int conn;

    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "Failed to fetch all the ads.";
    }

    sqlite3_stmt *stmt;

    char *query = (char *) malloc(QUERY_SIZE * sizeof(char *));
    sprintf(query,
            "select id, description, price, status, owner_id, category from advertisements where status <> 'sold';");

    conn = sqlite3_prepare_v2(db->getDB(), query, -1, &stmt, nullptr);

    if (conn != SQLITE_OK) {
        return "Failed to fetch all ads.";
    }
    std::string response;
    while ((conn = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *description = (const char *) sqlite3_column_text(stmt, 1);
        auto price = (float) sqlite3_column_double(stmt, 2);
        const char *status = (const char *) sqlite3_column_text(stmt, 3);
        int ownerId = sqlite3_column_int(stmt, 4);
        const char *category = (const char *) sqlite3_column_text(stmt, 5);
        auto *ad = new Advertisement(id, description, price, ownerId, status, category);
        response.append(ad->toString());
        delete (ad);
    }
    sqlite3_close(db->getDB());
    return response;
}

Advertisement *getAd(Database *db, int id) {
    int conn;
    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return nullptr;
    }
    char *query = (char *) malloc(QUERY_SIZE * sizeof(char *));
    sprintf(query, "select id, description, price, status, ownerId, category from advertisements where id = %d;", id);

    sqlite3_stmt *stmt;
    conn = sqlite3_prepare_v2(db->getDB(), query, -1, &stmt, 0);
    if (conn != SQLITE_OK) {
        return nullptr;
    }

    conn = sqlite3_step(stmt);

    free(query);
    int adId = sqlite3_column_int(stmt, 0);
    const char *description = (const char *) sqlite3_column_text(stmt, 1);
    auto price = (float) sqlite3_column_double(stmt, 2);
    const char *status = (const char *) sqlite3_column_text(stmt, 3);
    int ownerId = sqlite3_column_int(stmt, 4);
    const char *category = (const char *) sqlite3_column_text(stmt, 5);
    auto *ad = new Advertisement(adId, description, price, ownerId, status, category);
    std::cout<<ad->toString();
    sqlite3_close(db->getDB());
    return ad;
}

std::string removeAd(Database *db, int id, Client *client) {
    if (client->getId() == -1) {
        return "You are not logged in!";
    }
    auto *ad = getAd(db, id);

    if (ad == nullptr) {
        return "The ad with this id does not exist.";
    }

    if (ad->getOwnerId() != client->getId()) {
        delete (ad);
        return "You do not own this ad.";
    }

    int conn;

    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "Error while opening the database.";
    }

    char *query = (char *) malloc(QUERY_SIZE * sizeof(char *));
    sprintf(query, "delete from advertisements where id = %d;", id);
    conn = sqlite3_exec(db->getDB(), query, nullptr, nullptr, nullptr);

    free(query);
    sqlite3_close(db->getDB());
    if (conn != SQLITE_OK) {
        return "Failed to remove the ad.";
    } else {
        return "The ad was removed successfully.";
    }
}



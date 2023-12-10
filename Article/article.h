#ifndef PROJECT_ARTICLE_H
#define PROJECT_ARTICLE_H

#include <iostream>
#include <string>
#include "../Database/database.h"
#include "../Client/client.h"

class article {
private:
    int id;
    std::string description;
    float price;
    int ownerID;
    std::string status;
    std::string category;
    std::string title;
public:
    article(int id, std::string title, std::string description, float price, int ownerId, std::string status,
            std::string category);

    article(std::string title, std::string description, float price, int ownerId, std::string status,
            std::string category);

    article(std::string title, std::string description, float price, std::string status, std::string category);

    article(std::string, std::string, float, std::string);

    std::string getDescription();

    std::string getStatus();

    int getId();

    void setId(int id);

    float getPrice();

    int getOwnerId();

    std::string getCategory();

    void setOwnerId(int id);

    std::string toString();

    std::string getTitle();
};

std::string addArticle(article *advertisement, int ownerId, Database *db);

std::string getAllArticles(Database *db);

std::string getAllArticles(Database *db, int ownerID);

std::string getAllArticles(Database *db, std::string category);

std::string getAllArticles(Database *db, float down, float up);

std::string buyArticle(Database *db, int id, Client *client);

std::string updateArticleDescription(Database *db, int id, int ownerId, std::string newDescription);

std::string updateArticleTitle(Database *db, int id, int ownerId, std::string newTitle);

std::string updateArticleCategory(Database *db, int id, int ownerId, std::string newCategory);

std::string updateArticlePrice(Database *db, int id, int ownerId, float price);

std::string removeArticle(Database *db, int id, Client *client);

std::string getPurchaseHistory(Database *db, Client *client);

#endif

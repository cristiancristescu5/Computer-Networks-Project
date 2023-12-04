#ifndef PROJECT_ARTICLE_H
#define PROJECT_ARTICLE_H

#include <iostream>
#include <string>
#include "../Database/database.h"
#include "../Client/client.h"

class Article {
private:
    int id;
    std::string description;
    float price;
    int ownerID;
    std::string status;
    std::string category;
    std::string title;
public:
    Article(int id, std::string title, std::string description, float price, int ownerId, std::string status,
            std::string category);

    Article(std::string title, std::string description, float price, int ownerId, std::string status,
            std::string category);

    Article(std::string title, std::string description, float price, std::string status, std::string category);

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

std::string addArticle(Article *advertisement, int ownerId, Database *db);

std::string getAllArticles(Database *db);

std::string getAllArticles(Database *db, Client *cl);

std::string  getAllArticles(Database *db, int ownerID);

std::string getAllArticles(Database *db, std::string category);

std::string getAllUnsoldArticles(Database *db);

std::string buyArticle(int id);

std::string updateArticleDescription(int id, int ownerId, char* newDescription);

std::string updateArticleTitle(int id, int ownerId, char* newTitle);

std::string updateArticleCategory(int id, int ownerId, char* newCategory);

std::string removeArticle(Database *db, int id, Client *client);




#endif

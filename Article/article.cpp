//
// Created by cristi on 12/1/23.
//
#include "article.h"
#include <string>
#include <utility>
#include <stdexcept>
#include <format>
#include <vector>

#define QUERY_SIZE 1024


article::article(std::string title, std::string description, float price, int ownerId, std::string status,
                 std::string category) {
    this->description = std::move(description);
    this->price = price;
    this->ownerID = ownerId;
    this->status = std::move(status);
    this->category = std::move(category);
    this->title = std::move(title);
}

article::article(int id, std::string title, std::string description, float price, int ownerId, std::string status,
                 std::string category) {
    this->id = id;
    this->title = std::move(title);
    this->description = std::move(description);
    this->price = price;
    this->ownerID = ownerId;
    this->status = std::move(status);
    this->category = std::move(category);
}

article::article(std::string title, std::string description, float price, std::string status, std::string category) {
    this->description = std::move(description);
    this->price = price;
    this->title = std::move(title);
    this->status = std::move(status);
    this->category = std::move(category);
}

std::string article::getDescription() {
    return this->description;
}

std::string article::getStatus() {
    return this->status;
}

int article::getId() {
    return this->id;
}

void article::setId(int id) {
    this->id = id;
}

float article::getPrice() {
    return this->price;
}

int article::getOwnerId() {
    return this->ownerID;
}


void article::setOwnerId(int id) {
    this->ownerID = id;
}

std::string article::getCategory() {
    return this->category;
}

std::string article::toString() {
    std::string response;
    response.append("Title: ").append(this->getTitle()).append("\n")
            .append("Id: ").append(std::to_string(this->id)).append("\n")
            .append("Owner: ").append(std::to_string(this->ownerID)).append("\n")
            .append("Price: ").append(std::to_string(this->price)).append("\n")
            .append("Status: ").append(this->status).append("\n")
            .append("Category: ").append(this->category).append("\n")
            .append("Description: ").append(this->description)
            .append("\n-----------------------------------------------------------\n");
    return response;
}

std::string article::getTitle() {
    return this->title;
}

article::article(std::string title, std::string description, float price, std::string category) {
    this->title = std::move(title);
    this->description = std::move(description);
    this->price = price;
    this->category = std::move(category);
}

std::string addArticle(article *advertisement, int ownerId, Database *db) {
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
    sprintf(q, "insert into articles(title,description, price, owner_id, category) values('%s','%s', %f, %d, '%s')",
            advertisement->getTitle().c_str(), advertisement->getDescription().c_str(), advertisement->getPrice(),
            ownerId,
            advertisement->getCategory().c_str());//preparing the query

    conn = sqlite3_exec(db->getDB(), q, nullptr, nullptr, &errmsg);//executing the query

    sqlite3_close(db->getDB());//closing connection
    delete advertisement;
    if (conn == SQLITE_OK) {
        return "The advertisement has been published successfully.";
    } else {
        return "Failed to publish the add.";
    }
}

std::string getAllArticles(Database *db) {
    int conn;

    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "Failed to fetch all the ads.";
    }

    sqlite3_stmt *stmt;

    char *query = (char *) malloc(QUERY_SIZE * sizeof(char *));
    sprintf(query,
            "select id, title,description, price, status, owner_id, category from articles where status <> 'sold';");

    conn = sqlite3_prepare_v2(db->getDB(), query, -1, &stmt, nullptr);

    if (conn != SQLITE_OK) {
        free(query);
        sqlite3_finalize(stmt);
        sqlite3_close(db->getDB());

        return "Failed to fetch all ads.";
    }
    std::string response;
    while ((conn = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *title = (const char *) sqlite3_column_text(stmt, 1);
        const char *description = (const char *) sqlite3_column_text(stmt, 2);
        auto price = (float) sqlite3_column_double(stmt, 3);
        const char *status = (const char *) sqlite3_column_text(stmt, 4);
        int ownerId = sqlite3_column_int(stmt, 5);
        const char *category = (const char *) sqlite3_column_text(stmt, 6);
        auto *ad = new article(id, title, description, price, ownerId, status, category);
        response.append(ad->toString());
        delete (ad);
    }
    free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db->getDB());
    return response;
}

article *getArticle(Database *db, int id) {
    int conn;
    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return nullptr;
    }
    char *query = (char *) malloc(QUERY_SIZE * sizeof(char *));
    sprintf(query,
            "select id, title, description, price, status, owner_id, category from articles where id = %d;", id);

    sqlite3_stmt *stmt;
    conn = sqlite3_prepare_v2(db->getDB(), query, -1, &stmt, 0);
    if (conn != SQLITE_OK) {
        sqlite3_close(db->getDB());
        return nullptr;
    }

    conn = sqlite3_step(stmt);
    if (conn != SQLITE_ROW) {
        sqlite3_close(db->getDB());
        free(query);
        return nullptr;
    }
    int adId = sqlite3_column_int(stmt, 0);
    const char *title = (const char *) sqlite3_column_text(stmt, 1);
    const char *description = (const char *) sqlite3_column_text(stmt, 2);
    auto price = (float) sqlite3_column_double(stmt, 3);
    const char *status = (const char *) sqlite3_column_text(stmt, 4);
    int ownerId = sqlite3_column_int(stmt, 5);
    const char *category = (const char *) sqlite3_column_text(stmt, 6);
    auto *ad = new article(adId, title, description, price, ownerId, status, category);
    std::cout << ad->toString();
    sqlite3_close(db->getDB());
    sqlite3_finalize(stmt);
    free(query);
    return ad;
}

std::string removeArticle(Database *db, int id, Client *client) {
    if (client->getId() == -1) {
        return "You are not logged in!";
    }
    auto *ad = getArticle(db, id);

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
    sprintf(query, "delete from articles where id = %d;", id);
    conn = sqlite3_exec(db->getDB(), query, nullptr, nullptr, nullptr);

    free(query);
    sqlite3_close(db->getDB());
    if (conn != SQLITE_OK) {
        return "Failed to remove the ad.";
    } else {
        return "The ad was removed successfully.";
    }
}

std::string getAllArticles(Database *db, int ownerID) {
    int conn;

    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "Failed to connect to the database.";
    }

    sqlite3_stmt *stmt;
    char *q = (char *) (malloc(QUERY_SIZE * sizeof(char *)));
    sprintf(q, "select id, title, description, price, status, owner_id, category from articles where owner_id = %d;",
            ownerID);
    conn = sqlite3_prepare_v2(db->getDB(), q, -1, &stmt, 0);

    if (conn != SQLITE_OK) {
        free(q);
        sqlite3_finalize(stmt);
        sqlite3_close(db->getDB());
        return "Failed to fetch data";
    }
    std::string response;
    while ((conn = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *title = (const char *) sqlite3_column_text(stmt, 1);
        const char *description = (const char *) sqlite3_column_text(stmt, 2);
        auto price = (float) sqlite3_column_double(stmt, 3);
        const char *status = (const char *) sqlite3_column_text(stmt, 4);
        int ownerId = sqlite3_column_int(stmt, 5);
        const char *category = (const char *) sqlite3_column_text(stmt, 6);
        auto *ad = new article(id, title, description, price, ownerId, status, category);
        response.append(ad->toString());
        delete (ad);
    }
    if (response.empty()) {
        free(q);
        sqlite3_close(db->getDB());
        sqlite3_finalize(stmt);
        return "You do not own any article.";
    }
    free(q);
    sqlite3_close(db->getDB());
    sqlite3_finalize(stmt);
    return response;
}

std::string getAllArticles(Database *db, std::string category) {
    int conn;

    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "Failed to connect to the database.";
    }

    sqlite3_stmt *stmt;
    std::string query = std::format(
            "select id, title, description, price, status, owner_id, category from articles where category = '{}';",
            category);
    conn = sqlite3_prepare_v2(db->getDB(), query.c_str(), -1, &stmt, nullptr);

    if (conn != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db->getDB());
        return "Failed to fetch data";
    }
    std::string response;
    while ((conn = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *title = (const char *) sqlite3_column_text(stmt, 1);
        const char *description = (const char *) sqlite3_column_text(stmt, 2);
        auto price = (float) sqlite3_column_double(stmt, 3);
        const char *status = (const char *) sqlite3_column_text(stmt, 4);
        int ownerId = sqlite3_column_int(stmt, 5);
        const char *cat = (const char *) sqlite3_column_text(stmt, 6);
        auto *ad = new article(id, title, description, price, ownerId, status, cat);
        response.append(ad->toString());
        delete (ad);
    }
    sqlite3_close(db->getDB());
    sqlite3_finalize(stmt);
    if (response.empty()) {
        return "Empty";
    }
    return response;
}

std::string updateArticleDescription(Database *db, int id, int ownerId, std::string newDescription) {
    int conn;

    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "Failed to connect to the database.";
    }

    auto *article = getArticle(db, id);

    if (article == nullptr) {
        return "This article doe not exist.";
    }

    if (article->getOwnerId() != ownerId) {
        delete article;
        return "You do not own this article.";
    }

    std::string query = std::format("update articles set description ='{}' where id = {} and owner_id = {};",
                                    newDescription, id, ownerId);

    std::cout << query << std::endl;
    char *error;

    conn = sqlite3_exec(db->getDB(), query.c_str(), nullptr, nullptr, &error);

    delete article;

    if (conn != SQLITE_OK) {
        std::cout << error << std::endl;
        return "Failed to update the article's description.";
    }

    return "Article's description updated successfully";
}

std::string updateArticleTitle(Database *db, int id, int ownerId, std::string newTitle) {
    int conn;

    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "Failed to connect to the database.";
    }

    auto *article = getArticle(db, id);

    if (article == nullptr) {
        return "This article doe not exist.";
    }

    if (article->getOwnerId() != ownerId) {
        delete article;
        return "You do not own this article.";
    }

    std::string query = std::format("update articles set title = '{}' where id = {} and owner_id = {}",
                                    newTitle, id, ownerId);

    conn = sqlite3_exec(db->getDB(), query.c_str(), nullptr, nullptr, nullptr);

    delete article;

    if (conn != SQLITE_OK) {
        return "Failed to update the article's title.";
    }

    return "Article's title updated successfully";
}

std::string updateArticleCategory(Database *db, int id, int ownerId, std::string newCategory) {
    int conn;

    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "Failed to connect to the database.";
    }

    auto *article = getArticle(db, id);

    if (article == nullptr) {
        return "This article doe not exist.";
    }

    if (article->getOwnerId() != ownerId) {
        delete article;
        return "You do not own this article.";
    }

    std::string query = std::format("update articles set category = '{}' where id = {} and owner_id = {}",
                                    newCategory, id, ownerId);

    conn = sqlite3_exec(db->getDB(), query.c_str(), nullptr, nullptr, nullptr);

    delete article;

    if (conn != SQLITE_OK) {
        return "Failed to update the article's category.";
    }

    return "Article's category updated successfully";
}


std::string buyArticle(Database *db, int articleId, Client *client) {
    int conn;

    try {
        conn = db->getConnection();
    } catch (std::invalid_argument &e) {
        return "Failed to connect to database.";
    }

    article *article = getArticle(db, articleId);

    if (article == nullptr) {
        return "This article does not exist.";
    }

    if (article->getStatus() == "sold") {
        return "This article is already sold.";
    }

    if (article->getOwnerId() == client->getId()) {
        return "You cannot buy this article. You are the owner.";
    }
    delete article;
    std::string query = std::format("update articles set status = 'sold' where id = {};", articleId);

    conn = sqlite3_exec(db->getDB(), query.c_str(), nullptr, nullptr, nullptr);

    if (conn != SQLITE_OK) {
        return "Failed to buy the article";
    }
    query.clear();
    query = std::format("insert into purchase_history(ad_id, user_id) values({}, {});", articleId, client->getId());

    conn = sqlite3_exec(db->getDB(), query.c_str(), nullptr, nullptr, nullptr);

    if (conn != SQLITE_OK) {
        return "Failed to buy the article";
    }
    sqlite3_close(db->getDB());

    return "Article bought successfully.";
}

std::vector<int> getArticlesID(Database *db, int userID){
    int conn = db->getConnection();
    std::vector<int> ids;
    try{
        conn = db->getConnection();
    }catch (std::invalid_argument &e){
        throw std::invalid_argument("Failed to connect to database");
    }

    std::string query = std::format("select ad_id from purchase_history where user_id = {};", userID);

    sqlite3_stmt *stmt;

    conn = sqlite3_prepare_v2(db->getDB(), query.c_str(), -1, &stmt, 0);

    if(conn != SQLITE_OK){
        throw std::invalid_argument("Error with the database");
    }

    while((conn = sqlite3_step(stmt))==SQLITE_ROW){
        int id = sqlite3_column_int(stmt, 0);
        ids.push_back(id);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db->getDB());
    return ids;

}

std::string getPurchaseHistory(Database *db, Client *client){
    std::vector<int> ids;
    try{
        ids = getArticlesID(db, client->getId());
    }catch (std::invalid_argument &e){
        return e.what();
    }

    if(ids.empty()){
        return "Your purchase history is empty";
    }

    std::vector<article> articles;

    for(int i : ids){
        articles.push_back(*getArticle(db, i));
    }

    std::string response;

    for(auto art : articles){
        response.append(art.toString());
    }

    return response;
}

std::string updateArticlePrice(Database *db, int id, int ownerId, float price){
    int conn;
    try{
        conn = db->getConnection();
    }catch (std::invalid_argument &e){
        return "Failed to connect to database";
    }

    auto *article = getArticle(db, id);
    if(article == nullptr){
        return "The article does not exist.";
    }

    if(article->getOwnerId() != ownerId){
        return "You do not own this article";
    }
    std::string query = std::format("update articles set price = {} where id = {};", price,article->getId());
    conn = sqlite3_exec(db->getDB(), query.c_str(), nullptr, nullptr, nullptr);

    if(conn != SQLITE_OK) {
        sqlite3_close(db->getDB());
        return "Failed to update the article";
    }
    sqlite3_close(db->getDB());
    return "The price of the article was updates successfully";
}

std::string getAllArticles(Database *db, float down, float up){
    int conn;
    try{
        conn = db->getConnection();
    }catch (std::invalid_argument &e){
        return "Failed to connect to database";
    }

    sqlite3_stmt *stmt;
    std::string query = std::format("elect id, title, description, price, status, owner_id, category from articles where price <={} and price >= {}", up, down);

    conn = sqlite3_prepare_v2(db->getDB(), query.c_str(), -1, &stmt, 0);

    if (conn != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db->getDB());
        return "Failed to fetch data";
    }

    std::string response;
    while((conn = sqlite3_step(stmt))==SQLITE_ROW){
        int id = sqlite3_column_int(stmt, 0);
        const char *title = (const char *) sqlite3_column_text(stmt, 1);
        const char *description = (const char *) sqlite3_column_text(stmt, 2);
        auto price = (float) sqlite3_column_double(stmt, 3);
        const char *status = (const char *) sqlite3_column_text(stmt, 4);
        int ownerId = sqlite3_column_int(stmt, 5);
        const char *cat = (const char *) sqlite3_column_text(stmt, 6);
        auto *ad = new article(id, title, description, price, ownerId, status, cat);
        response.append(ad->toString());
        delete (ad);
    }
    sqlite3_close(db->getDB());
    sqlite3_finalize(stmt);
    return response;

}


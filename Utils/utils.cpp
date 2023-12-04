//
// Created by cristi on 12/2/23.
//
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "utils.h"


std::string handleClient(Client *client, Database *db, Command *command) {
    std::cout << client->getId() << std::endl;
    if (command->getSize() == 1 && command->getCommand() == "help") {
        return help();
    }
    if (command->getSize() == 1 && command->getCommand() == "logout") {
        return logOut(client);
    }

    if (command->getSize() == 3) {
        if (command->getTokens()[0] == "login") {//login <> <>
            return login(db, command->getTokens()[1], command->getTokens()[2], client);
        }
        if (command->getTokens()[0] == "register") {//register <> <>
            return registerUser(db, command->getTokens()[1], command->getTokens()[2], client);
        }
    }
    if (command->getSize() == 5) {
        if (command->getTokens()[0] == "add_article") {
            std::string name = command->getTokens()[1];
            std::string description = command->getTokens()[2];
            float price;
            try {
                price = std::stof(command->getTokens()[3]);
            } catch (std::invalid_argument &e) {
                return "Invalid price.";
            }
            std::string category = command->getTokens()[4];
            return addArticle(new Article(name, description, price, category), client, db);
        }
    }
    if (command->getSize() == 2) {
        if (command->getTokens()[0] == "get_all_articles" && command->getTokens()[1] == "-a") {//fac aici functie on logat
            return getAllArticles(db);
        }
        if (command->getTokens()[0] == "get_all_articles" && command->getTokens()[1] == "-u") {//fac aici functi om logat
            if (client->getId() != -1) {
                std::cout << "aici la articcols" << std::endl;
                return getAllArticles(db, client->getId());
            } else {
                return "You cannot perform this action. Log in first.";
            }
        }
    }
    if (client->getId() != -1) {
        return "bau bau bnau";
    }
    return "bau";
}

std::string help() {
    FILE *f;
    f = fopen("/home/cristi/Desktop/marketplace/project/help.txt", "r");
    std::string response;
    char *line = (char *) (malloc(100 * sizeof(char *)));
    while (fgets(line, 100, f) != nullptr) {
        response.append(line);
    }
    fclose(f);
    return response;
}

std::string login(Database *db, const std::string &name, const std::string &password, Client *client) {
    if (client->getId() != -1) {
        return "User is already logged in.";
    }

    auto *loggedClient = loginUser(db, name, password);

    if (loggedClient == nullptr) {
        return "The user with this username and password does not exist.";
    }

    client->setID(loggedClient->getId());
    client->setName(loggedClient->getName());
    client->setPassword(loggedClient->getPassword());
    client->printClient();
    return "User logged in successfully";
}

std::string registerUser(Database *db, const std::string &name, const std::string &password, Client *client) {
    if (client->getId() != -1) {
        return "You cannot create an account, you are already logged in.";
    }

    return addUser(db, name, password);
}

std::string addArticle(Article *article, Client *client, Database *db) {
    if (std::ranges::find(categories, article->getCategory()) == categories.end()) {
        return "Invalid category.";
    }

    if (client->getId() == -1) {
        return "You cannot perform this action. Log in first.";
    }

    return addArticle(article, client->getId(), db);
}

std::string logOut(Client *client) {
    if (client->getId() == -1) {
        return "You are not logged in.";
    }

    client->removeName();
    client->getPassword();
    client->setID(-1);

    return "You logged out successful";
}
#include <string>
#include <cstdlib>
#include <cstdio>
#include "utils.h"
#include <ranges>
#include <algorithm>

std::vector<Client *> clients;

std::string handleClient(Client *client, Database *db, Command *command, std::mutex &mutex) {
    std::string response;
    if (command->getSize() == 1 && command->getCommand() == "help") {
        mutex.lock();
        response = help();
        mutex.unlock();

        return response;
    }
    if (command->getSize() == 1 && command->getCommand() == "logout") {
        if (client->getId() == -1) {
            return FORBIDDEN;
        } else {

            mutex.lock();
            response = logOut(client);
            mutex.unlock();

            return response;
        }
    }

    if (command->getSize() == 1 && command->getTokens()[0] == "get_purchase_history") {
        if (client->getId() == -1) {
            return FORBIDDEN;
        } else {
            mutex.lock();
            response = getPurchaseHistory(db, client);
            mutex.unlock();
            return response;
        }
    }

    if (command->getSize() == 1 && command->getTokens()[0] == "quit") {

        return "You left the server.";
    }

    if (command->getSize() == 3) {
        if (command->getTokens()[0] == "login") {//login <> <>
            mutex.lock();
            response = login(db, command->getTokens()[1], command->getTokens()[2], client);
            mutex.unlock();

            return response;
        }
        if (command->getTokens()[0] == "register") {//register <> <>
            mutex.lock();
            response = registerUser(db, command->getTokens()[1], command->getTokens()[2], client);
            mutex.unlock();

            return response;
        }
        if (command->getTokens()[0] == "get_all_articles") {
            if (client->getId() == -1) {
                return FORBIDDEN;
            } else {
                if (command->getTokens()[1] == "-c") {
                    mutex.lock();
                    response = getAllArticles(db, command->getTokens()[2]);
                    mutex.unlock();
                    return response;
                }
            }
        }
    }

    if (command->getSize() == 4) {
        if(command->getTokens()[0] == "get_all_articles" && command->getTokens()[1] == "-p"){
            if(client->getId() == -1){
                return FORBIDDEN;
            }
            float down, up;
            try{
                down = std::stof(command->getTokens()[2]);
                up = std::stof(command->getTokens()[3]);
            }catch (std::invalid_argument &e){
                return "Invalid argument";
            }
            mutex.lock();
            response = getAllArticles(db, down, up);
            mutex.unlock();
            return response;
        }
        if (command->getTokens()[0] == "update_article") {
            if (client->getId() == -1) {

                return FORBIDDEN;
            }
            int artId;
            try {
                artId = std::stoi(command->getTokens()[2]);
            } catch (std::invalid_argument &e) {

                return "Invalid argument.";
            }
            if (command->getTokens()[1] == "-d") {
                mutex.lock();
                response = updateArticleDescription(db, artId, client->getId(), command->getTokens()[3]);
                mutex.unlock();

                return response;
            }
            if (command->getTokens()[1] == "-c") {
                if (std::ranges::find(categories, command->getTokens()[3]) == categories.end()) {
                    return "Invalid category.";
                }
                mutex.lock();
                response = updateArticleCategory(db, artId, client->getId(), command->getTokens()[3]);
                mutex.unlock();
                return response;
            }
            if (command->getTokens()[1] == "-t") {
                mutex.lock();
                response = updateArticleTitle(db, artId, client->getId(), command->getTokens()[3]);
                mutex.unlock();

                return response;
            }
            if (command->getTokens()[1] == "-p") {
                float price;
                try {
                    price = std::stof(command->getTokens()[3]);
                } catch (std::invalid_argument &e) {
                    return "Invalid price";
                }
                if (client->getId() == -1) {
                    return FORBIDDEN;
                }
                mutex.lock();
                response = updateArticlePrice(db, artId, client->getId(), price);
                mutex.unlock();
                return response;
            }
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
            mutex.lock();
            response = addArticle(new article(name, description, price, category), client, db);
            mutex.unlock();
            return response;
        }
    }
    if (command->getSize() == 2) {
        if (command->getTokens()[0] == "get_all_articles" && command->getTokens()[1] == "-a") {
            if (client->getId() == -1) {

                return FORBIDDEN;
            } else {
                mutex.lock();
                response = getAllArticles(db);
                mutex.unlock();

                return response;
            }
        }
        if (command->getTokens()[0] == "get_all_articles" && command->getTokens()[1] == "-u") {
            if (client->getId() != -1) {
                mutex.lock();
                response = getAllArticles(db, client->getId());
                mutex.unlock();

                return response;
            } else {

                return FORBIDDEN;
            }
        }
        if (command->getTokens()[0] == "remove") {
            if (client->getId() == -1) {

                return FORBIDDEN;
            } else {
                int id;
                try {
                    id = std::stoi(command->getTokens()[1]);
                } catch (std::invalid_argument &e) {
                    return "Invalid argument";
                }
                mutex.lock();
                response = removeArticle(db, id, client);
                mutex.unlock();

                return response;
            }
        }
        if (command->getTokens()[0] == "buy") {
            if (client->getId() == -1) {
                return FORBIDDEN;
            } else {
                int artId;
                try {
                    artId = std::stoi(command->getTokens()[1]);
                } catch (std::invalid_argument &e) {
                    return "Invalid argument";
                }
                mutex.lock();
                response = buyArticle(db, artId, client);
                mutex.unlock();
                return response;
            }
        }
    }
    response.append("Invalid command.");
    return response;
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

    if (isLogged(loggedClient) == 1) {
        return "User is already logged in.";
    }

    if (loggedClient == nullptr) {
        return "The user with this username and password does not exist.";
    }

    client->setID(loggedClient->getId());
    client->setName(loggedClient->getName());
    client->setPassword(loggedClient->getPassword());
    clients.push_back(client);
//    client->printClient();
    return "User logged in successfully";
}

std::string registerUser(Database *db, const std::string &name, const std::string &password, Client *client) {
    if (client->getId() != -1) {
        return "You cannot create an account, you are already logged in.";
    }

    return addUser(db, name, password);
}

std::string addArticle(article *article, Client *client, Database *db) {
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


    auto it = std::find(clients.begin(), clients.end(), client);
    if (it != clients.end()) {
        clients.erase(it);
    }

    client->removeName();
    client->getPassword();
    client->setID(-1);

    return "You logged out successful";
}

int isLogged(Client *client) {
    if (clients.empty()) {
        return 0;
    }

    for (auto c: clients) {

        if (client->getId() == c->getId()) {
            return 1;
        }
    }
    return 0;
}


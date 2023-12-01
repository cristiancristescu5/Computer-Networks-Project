#include <iostream>
#include "Client/client.h"
#include "Database/database.h"
#include <stdexcept>
#include <vector>

const char * DBURL = "/home/cristi/Desktop/marketplace/marketplace.db";
#define numUsers 100
std::vector<Client> clients;

int main() {
    auto* database = new Database(DBURL);
    auto* client = new Client("name1", "pass", 1, 1);
    std::cout<<addUser(*database, client)<<std::endl;
    Client* client1 = loginUser(*database, "bau1", "bau1");
    if(client1 != nullptr){
        std::cout<<"Am gasit user"<<std::endl;
        client1->printClient();
    }
    return 0;
}

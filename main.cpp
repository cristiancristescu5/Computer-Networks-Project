#include <iostream>
#include "Client/client.h"
#include "Database/database.h"
#include <stdexcept>
#include <vector>
#include "Ad/Advertisement.h"

const char * DBURL = "/home/cristi/Desktop/marketplace/marketplace.db";
#define numUsers 100
std::vector<Client> clients;


int main() {
    auto* database = new Database(DBURL);
//    auto* client = new Client("name1", "pass", 1, 1);
//    auto* ad = new Advertisement("cea mai descriere FRUMOASA", 14.5, 2, "unsold", "categorie1");
//    std::cout<<addUser(*database, client)<<std::endl;
//    std::cout<<addAdvertisement(ad, 2, database)<<std::endl;
//    std::cout<<ad->toString()<<std::endl;
//    Client* client1 = loginUser(*database, "bau1", "bau1");
//    if(client1 != nullptr){
//        std::cout<<"Am gasit user"<<std::endl;
//        client1->printClient();
//    }
    std::cout<<removeAd(database,4, new Client("bau", "bau", 1, 1))<<std::endl;
    return 0;
}

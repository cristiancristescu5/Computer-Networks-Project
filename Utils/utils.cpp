//
// Created by cristi on 12/2/23.
//
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
std::string help(){
    FILE *f;
    f = fopen("/home/cristi/Desktop/marketplace/project/help.txt", "r");
    std::string response;
    char *line = (char*) (malloc(100 * sizeof(char *)));
    while(fgets(line, 100, f) != 0){
        response.append(line);
    }
    fclose(f);
    return response;
}
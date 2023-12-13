//
// Created by cristi on 12/4/23.
//
#include "command.h"
#include <regex>
#include "string"


Command::Command(char* command) {
    this->command = command;
    std::regex rgx("\"([^\"]*)\"|(\\S+)");
    std::smatch match;
    std::string com = command;
    int i = 0;

    while (std::regex_search(com, match, rgx)) {
        std::string token;
        if(i == 0){
            token = match[0].str();
        }else{
            token = match[1].str();
        }
        if (!token.empty()) {
            this->tokens.push_back(token);
        }
        com = match.suffix().str();
        i++;
    }
}

std::string Command::getCommand() {
    return this->command;
}

std::vector<std::string> Command::getTokens() {
    return this->tokens;
}

unsigned long Command::getSize() {
    return this->tokens.size();
}
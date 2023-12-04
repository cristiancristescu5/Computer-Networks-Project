//
// Created by cristi on 12/4/23.
//
#include "command.h"
#include <sstream>

Command::Command(char *command) {
    this->command = command;
    std::istringstream stream(this->command);

    do {//parsing the command
        std::string word;
        stream >> word;
        this->tokens.push_back(word);
    } while (stream);

    this->tokens.pop_back();

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
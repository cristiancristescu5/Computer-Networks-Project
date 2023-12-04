//
// Created by cristi on 12/4/23.
//
#include "command.h"
#include <sstream>

Command::Command(char *command) {
    this->command = command;
    std::istringstream stream(this->command);

    std::string word;

    while(std::getline(stream, word, '|')) {
        this->tokens.push_back(word);
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
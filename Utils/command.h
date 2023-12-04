//
// Created by cristi on 12/3/23.
//

#ifndef PROJECT_COMMAND_H
#define PROJECT_COMMAND_H

#include <string>
#include <iostream>
#include <vector>

class Command {
private:
    std::string command;
    std::vector<std::string> tokens;
public:
    Command(char *command);

    std::string getCommand();

    std::vector<std::string> getTokens();

    unsigned long getSize();
};


#endif //PROJECT_COMMAND_H

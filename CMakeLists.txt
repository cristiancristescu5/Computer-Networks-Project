cmake_minimum_required(VERSION 3.26)
project(project)

set(CMAKE_CXX_STANDARD 20)

add_executable(project main.cpp
        Client/client.h
        Client/client.cpp
        Database/database.h
        Database/database.cpp
        Ad/Article.h
        Ad/Article.cpp
        bau.cpp
        Utils/utils.h
        Utils/utils.cpp
        Utils/command.h)
target_link_libraries(project sqlite3)
cmake_minimum_required(VERSION 3.26)
project(project)

set(CMAKE_CXX_STANDARD 20)

add_executable(project main.cpp
        User/client.h
        User/client.cpp
        Database/database.h
        Database/database.cpp
        Article/article.h
        Article/article.cpp
        Utils/utils.h
        Utils/utils.cpp
        Utils/command.h
        Utils/command.cpp)
target_link_libraries(project sqlite3)
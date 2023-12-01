cmake_minimum_required(VERSION 3.26)
project(project)

set(CMAKE_CXX_STANDARD 17)

add_executable(project main.cpp
        Client/client.h
        Client/client.cpp
        Database/database.h
        Database/database.cpp
        Ad/Advertisement.h
        Ad/Advertisement.cpp)
target_link_libraries(project sqlite3)
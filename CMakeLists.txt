cmake_minimum_required(VERSION 3.10)
project(trainingManager)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(/opt/homebrew/opt/mysql-client/include)

link_directories(/opt/homebrew/opt/mysql-client/lib)

add_executable(training_login main.cpp)

target_link_libraries(training_login mysqlclient)

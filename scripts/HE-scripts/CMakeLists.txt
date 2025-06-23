cmake_minimum_required(VERSION 3.10)
project(EncryptSplit1)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/seal)

add_executable(encrypt_split_1 encrypt_split_1.cpp)
target_link_libraries(encrypt_split_1 ${CMAKE_SOURCE_DIR}/libseal.a)

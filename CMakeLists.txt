cmake_minimum_required(VERSION 3.16)
project(Ticket_System_master)

set(CMAKE_CXX_STANDARD 14)

add_executable(Ticket_System_master
        src/main.cpp
        src/BPT.hpp
        src/map.hpp
        src/utility.hpp
        src/exceptions.hpp)

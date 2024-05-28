cmake_minimum_required(VERSION 3.28)
project(CNetwork)

set(CMAKE_CXX_STANDARD 17)

add_executable(CNetwork main.cpp
        tcp_connect/fail_.cpp
        show_ip.cpp
        tcp_connect/socket.cpp
)

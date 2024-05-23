cmake_minimum_required(VERSION 3.28)
project(CNetwork)

set(CMAKE_CXX_STANDARD 17)

add_executable(CNetwork main.cpp
        ip_address.cpp
        show_ip.cpp
        socket.cpp
)

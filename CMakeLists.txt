cmake_minimum_required(VERSION 3.27)
project(filesharing C)

set(CMAKE_C_STANDARD 11)

add_executable(filesharing
        server.c
        socket_wrapper.c
        socket_wrapper.h
        client.c
        file_transfer.h
        file_transfer.c
        client.h
        server.h
        main.c)

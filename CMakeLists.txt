cmake_minimum_required(VERSION 3.17)
project(reactor C)

set(CMAKE_C_STANDARD 11)

add_executable(reactor main.c event.c event.h event_loop.c
        event_loop.h event_ops.c event_ops.h event_config.h
        backend/epoll.c backend/epoll.h log.c log.h utils.c utils.h)
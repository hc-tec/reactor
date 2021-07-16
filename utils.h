//
// Created by titto on 2021/7/16.
//

#ifndef REACTOR_UTILS_H
#define REACTOR_UTILS_H

#include <fcntl.h>
#include <stdio.h>

#include "log.h"

int
re_make_socket_nonblocking(int fd);

#endif //REACTOR_UTILS_H

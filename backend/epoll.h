//
// Created by titto on 2021/7/16.
//

#ifndef REACTOR_EPOLL_H
#define REACTOR_EPOLL_H

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>

#include "../event_ops.h"
#include "../event_loop.h"
#include "../utils.h"

struct epoll_op {
    struct epoll_event* events;
    int n_events;
    int ep_fd;
};

static void* epoll_init(struct re_event_loop*);
static int epoll_add(struct re_event_loop*, int fd, short events);
static int epoll_del(struct re_event_loop*, int fd, short events);
static int epoll_dispatch(struct re_event_loop*);
static void epoll_dealloc(struct re_event_loop*);

struct re_event_ops epoll_ops = {
        "epoll",
        epoll_init,
        epoll_add,
        epoll_del,
        epoll_dispatch,
        epoll_dealloc
};;

#endif //REACTOR_EPOLL_H

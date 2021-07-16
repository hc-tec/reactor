//
// Created by titto on 2021/7/16.
//

#ifndef REACTOR_EVENT_OPS_H
#define REACTOR_EVENT_OPS_H

#include <stdio.h>
#include "event_loop.h"
#include "event_config.h"

struct re_event_ops {
    const char* name;
    void* (*init)(struct re_event_loop*);
    int (*add)(struct re_event_loop*, int fd, short events);
    int (*del)(struct re_event_loop*, int fd, short events);
    int (*dispatch)(struct re_event_loop*);
    void (*dealloc)(struct re_event_loop*);
};

typedef struct re_event_ops* re_event_ops_t;

extern struct re_event_ops epoll_ops;

static struct re_event_ops* re_event_backends[] = {
#ifdef EVENT__HAVE_EPOLL
        &epoll_ops,
#endif
//#ifdef EVENT__HAVE_DEVPOLL
//        &devpollops,
//#endif
//#ifdef EVENT__HAVE_POLL
//        &pollops,
//#endif
//#ifdef EVENT__HAVE_SELECT
//        &selectops,
//#endif
//#ifdef _WIN32
//        &win32ops,
//#endif
        NULL
};



#endif //REACTOR_EVENT_OPS_H

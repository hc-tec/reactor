//
// Created by titto on 2021/7/16.
//

#ifndef REACTOR_EVENT_H
#define REACTOR_EVENT_H

#include <sys/queue.h>
#include "event_loop.h"


struct re_event_cb {
    union {
        void (*callback)(int fd, short event, void*);
        // may be other callback
    };
    void* cb_arg;
};

struct re_event {
    TAILQ_ENTRY(re_event) re_next;
    struct re_event_loop* loop;
    struct re_event_cb re_callback;
    int re_fd;
    short re_events;
    short flags;
};

//typedef struct re_event* re_event_t;

// TAILQ_HEAD(re_event_list, re_event);

struct re_event*
re_event_new(
        struct re_event_loop*,
        int fd,
        short events,
        void (*cb)(int fd, short event, void*),
        void* arg
);

int
re_event_del(struct re_event*);

#endif //REACTOR_EVENT_H

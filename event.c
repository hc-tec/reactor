//
// Created by titto on 2021/7/16.
//

#include "event.h"



struct re_event*
re_event_new(
        struct re_event_loop* loop,
        int fd,
        short events,
        void (*cb)(int fd, short event, void*),
        void* arg
) {
    struct re_event* event = NULL;
    event = (struct re_event*) malloc(sizeof(struct re_event));
    if(event == NULL) {
        re_log("[re_event_new] memory not enough");
        return NULL;
    }
    event->loop = loop;

    event->re_fd = fd;
    event->re_callback.callback = cb;
    event->re_callback.cb_arg = arg;
    event->re_events = events;
    {
        struct re_event_ops* backend = loop->re_backend;
        loop->re_event_count++;
        TAILQ_INSERT_TAIL(&loop->re_all_events_list, event, re_next);
        backend->add(loop, fd, events);
    }


    return event;
}

int
re_event_del(struct re_event* ev) {
    struct re_event_loop* loop = ev->loop;
    TAILQ_REMOVE(&loop->re_all_events_list, ev, re_next);
    TAILQ_REMOVE(&loop->re_active_event_list, ev, re_next);
}















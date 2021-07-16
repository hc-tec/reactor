//
// Created by titto on 2021/7/16.
//

#ifndef REACTOR_EVENT_LOOP_H
#define REACTOR_EVENT_LOOP_H

#include <stdlib.h>

#include "event_ops.h"
#include "event.h"
#include "log.h"

TAILQ_HEAD(re_event_list, re_event);

struct re_event_loop {
    struct re_event_ops* re_backend;
    void* re_backend_instance;
//    TAILQ_HEAD(re_all_events_list, re_event_t);
//    TAILQ_HEAD(re_active_event_list, re_event_t);
    struct re_event_list re_all_events_list;
    struct re_event_list re_active_event_list;
    int re_event_count;
    int re_event_active_count;

    int re_event_exit;
    int running;

};
typedef struct re_event_loop* re_event_loop_t;

struct re_event_loop* re_loop_new();
int re_loop(struct re_event_loop*);

int re_process_active(struct re_event_loop*);


#endif //REACTOR_EVENT_LOOP_H


//
// Created by titto on 2021/7/16.
//

#include "event_loop.h"




struct re_event_loop*
re_loop_new() {
    struct re_event_loop* loop = NULL;
    loop = (struct re_event_loop*) malloc(sizeof(struct re_event_loop));
    if(loop == NULL) {
        re_log("[re_loop_new] memory not enough");
        return NULL;
    }

    TAILQ_INIT(&loop->re_all_events_list);
    TAILQ_INIT(&loop->re_active_event_list);

    {
        loop->re_event_count = 0;
        loop->re_event_active_count = 0;
        loop->re_event_exit = 0;
        loop->running = 0;
    }

    for(int i=0; re_event_backends[i]; ++i) {
        loop->re_backend = re_event_backends[i];
        break;
    }

    loop->re_backend_instance = loop->re_backend->init(loop);

    return loop;
}
int
re_loop(struct re_event_loop* loop) {
    struct re_event_ops* backend = loop->re_backend;
    int n;
    if(loop->running) {
        re_log("[re_loop] loop is already running");
        return 0;
    }
    loop->running = 1;
    loop->re_event_exit = 0;
    while (1) {
        if(loop->re_event_exit) {
            re_log("[re_loop] loop is already exit");
            goto done;
        }
        if(TAILQ_EMPTY(&loop->re_all_events_list)) {
//            re_log("[re_loop] no events left");

//            loop->re_event_exit = 1;
//            break;
        }
        int res = backend->dispatch(loop);
        if(res == 0) {
            re_log("[re_loop] dispatch unsuccessfully");
            goto done;
        }
        if(!TAILQ_EMPTY(&loop->re_active_event_list)) {
            n = re_process_active(loop);
        }
    }
    done:
        loop->re_event_exit = 1;
    return 1;
}

int
re_process_active(struct re_event_loop* loop) {

}

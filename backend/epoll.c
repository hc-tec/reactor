//
// Created by titto on 2021/7/16.
//

#include "epoll.h"

static void*
epoll_init(struct re_event_loop* loop) {
    int ep_fd = -1;
    struct epoll_op* epollop;
    if ((ep_fd = epoll_create(32)) == -1) {
        if (errno != ENOSYS) {
            re_log("[epoll_init] ep_fd create failed");
            return NULL;
        }
    }
    epollop = (struct epoll_op*) malloc(sizeof(struct epoll_op));
    if(epollop == NULL) {
        re_log("[epoll_init] memory not enough");
        close(ep_fd);
        return NULL;
    }
    epollop->ep_fd = ep_fd;
    epollop->events = (struct epoll_event*) malloc(32*sizeof(struct epoll_event));
    if(epollop->events == NULL) {
        re_log("[epoll_init] memory not enough");
        close(ep_fd);
        return NULL;
    }
    epollop->n_events = 32;
    re_make_socket_nonblocking(ep_fd);
    return epollop;
}

static int
epoll_add(struct re_event_loop* loop, int fd, short events) {
    struct epoll_event ev;
    struct epoll_op* epollop = loop->re_backend_instance;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = events;
    epoll_ctl(epollop->ep_fd, EPOLL_CTL_DEL, fd, &ev);
    if(epoll_ctl(epollop->ep_fd, EPOLL_CTL_ADD, fd, &ev) == 0) {
        return 1;
    }
    return 0;
}
static int
epoll_del(struct re_event_loop* loop, int fd, short events) {
    struct epoll_event ev;
    struct epoll_op* epollop = loop->re_backend_instance;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = events;
    if(epoll_ctl(epollop->ep_fd, EPOLL_CTL_DEL, fd, &ev) == 0) {
        return 1;
    }
    return 0;
}

static int
epoll_dispatch(struct re_event_loop* loop) {
    struct epoll_op* epollop = loop->re_backend_instance;
    struct epoll_event* events = epollop->events;
    struct re_event* re_event;
    int res, i;

    res = epoll_wait(epollop->ep_fd, events, epollop->n_events, 2000);
    if (res == -1) {
        if (errno != EINTR) {
            re_log("[epoll_dispatch] epoll_wait");
            return -1;
        }
        return 0;
    }
    if(res == 0) {
        return 1;
    }
    for (i = 0; i < res; i++) {
        int what = events[i].events;
        int active_fd = events[i].data.fd;
        short ev = 0;

        if (what & EPOLLERR) {
            ev = 1;
        } else if ((what & EPOLLHUP) && !(what & EPOLLRDHUP)) {
            ev = 1;
        } else {
            if (what & EPOLLIN)
                ev |= 1;
            if (what & EPOLLOUT)
                ev |= 1;
            if (what & EPOLLRDHUP)
                ev |= 1;
        }

        if (!ev)
            continue;
        loop->re_event_active_count++;
        TAILQ_FOREACH(re_event, &loop->re_all_events_list, re_next) {
            if(re_event->re_fd == active_fd) {
                if(re_event->re_events & what) {
                    re_event->re_callback.callback(active_fd, what, re_event->re_callback.cb_arg);
                    TAILQ_REMOVE(&loop->re_all_events_list, re_event, re_next);
                }
            }
        }
//        TAILQ_INSERT_TAIL(loop->re_active_event_list)

    }

}
static void
epoll_dealloc(struct re_event_loop* loop) {

}

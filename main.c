#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include "event.h"
#include "event_loop.h"
#include "utils.h"

void
accept_cb(int fd, short events, void* arg);

void
read_cb(int fd, short events, void *arg);

void
write_cb(int fd, short events, void *arg);

void
cmd_msg_cb(int fd, short events, void* arg);

void
set_nonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

void
accept_cb_v2(int fd, short event, void* arg);

int
socket_create(char* ip, int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    set_nonblocking(fd);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    inet_aton(ip, &(serv_addr.sin_addr));
    serv_addr.sin_port = htons(port);
    int flag = bind(fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    if(flag < 0) {
        printf("Bind Error\n");
        return -1;
    }
    listen(fd, 20);
    return fd;
}

void
timer_cb(int fd, short event, void* arg);

int
main()
{
    int port = 8888;
    int sock_fd;
    do {
        port += 1;
        sock_fd = socket_create((char*) "127.0.0.1", port);
    } while (sock_fd == -1);
    printf("http://127.0.0.1:%d\n", port);

    struct re_event_loop* loop = re_loop_new();
    // struct event_base* base = event_base_new();

    //  设置定时器回调函数
    struct event* ev_conn = re_event_new(loop, sock_fd,
                                      EPOLLIN | EPOLLOUT, accept_cb, loop);

//    re_event_add(ev_conn, NULL);

//    printf("Using Libevent with backend method %s.",
//           event_base_get_method(base));
//    enum event_method_feature f = event_base_get_features(base);
//    if ((f & EV_FEATURE_ET))
//        printf("  Edge-triggered events are supported.");
//    if ((f & EV_FEATURE_O1))
//        printf("  O(1) event notification is supported.");
//    if ((f & EV_FEATURE_FDS))
//        printf("  All FD types are supported.");
//    puts("");
    //程序等待就绪事件并执行事件处理
    re_loop(loop);
//    event_base_dispatch(base);

//    event_base_free(base);
    return 0;
}

void
timer_cb(int fd, short event, void* arg) {
    printf("timer_cb execute\n");
}

void
cmd_msg_cb(int fd, short events, void* arg)
{
    char msg[1024];

    int ret = read(fd, msg, sizeof(msg));
    if( ret <= 0 )
    {
        perror("read fail ");
        exit(1);
    }

    int sockfd = *((int*)arg);

    //把终端的消息发送给服务器端
    //为了简单起见，不考虑写一半数据的情况
    write(sockfd, msg, ret);
}

void
accept_cb(int fd, short events, void* arg)
{
    int sockfd;

    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    sockfd = accept(fd, (struct sockaddr*)&client, &len);
    re_make_socket_nonblocking(sockfd);

    printf("accept a client %d\n", sockfd);

    struct re_event_loop* loop = (struct re_event_loop*) arg;

    re_event_new(loop, sockfd, EPOLLOUT | EPOLLIN,
                 read_cb, arg);
}

void
read_cb(int fd, short events, void *arg)
{
    sleep(1);
    char msg[4096];
    int len = read(fd, msg, sizeof(msg) - 1);
    if(len <= 0)
    {
        printf("some error happen when read\n");
        close(fd);
        return ;
    }

    msg[len] = '\0';
    printf("recv the client msg: %s", msg);

    struct re_event_loop* loop = (struct re_event_loop*) arg;

    //仅仅是为了动态创建一个event结构体
    re_event_new(loop, fd, EPOLLOUT | EPOLLIN,
                 write_cb, arg);
}


void
write_cb(int fd, short events, void *arg) {
    char reply_msg[4096] = "HTTP/1.1 200 OK\r\n"
                           "Content-Type:text/html;charset=utf-8\r\n"
                           "Content-Length:18\r\n"
                           "\r\n"
                           "Welcome to tinyweb";
    write(fd, reply_msg, strlen(reply_msg) );
}

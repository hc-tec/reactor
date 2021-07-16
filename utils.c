//
// Created by titto on 2021/7/16.
//

#include "utils.h"


int
re_make_socket_nonblocking(int fd)
{
#ifdef _WIN32
    {
		unsigned long nonblocking = 1;
		if (ioctlsocket(fd, FIONBIO, &nonblocking) == SOCKET_ERROR) {
			event_sock_warn(fd, "fcntl(%d, F_GETFL)", (int)fd);
			return -1;
		}
	}
#else
    {
        int flags;
        if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) {
            re_log("[re_make_socket_nonblocking] fcntl(%d, F_GETFL)");
            return -1;
        }
        if (!(flags & O_NONBLOCK)) {
            if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
                re_log("[re_make_socket_nonblocking] fcntl(%d, F_SETFL)");
                return -1;
            }
        }
    }
#endif
    return 0;
}

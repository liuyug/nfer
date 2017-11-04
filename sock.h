#ifndef _SOCK_H_
#define _SOCK_H_

#ifdef _WIN32
#include <winsock2.h>
#endif

#ifdef _LINUX
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// #include <sys/wait.h>
// #include <sys/time.h>
#endif

#ifdef _WIN32
#define  STAT           stat
#define  socklen_t      int
#endif
#ifdef _LINUX
#define  STAT           lstat
#define  closesocket    close
#define  SOCKET         int
#define  INVALID_SOCKET -1
#endif

#define MAX_SOCKBUFFER 8192

struct host_addr {
    char host[256];
    int port;
};


struct host_addrs {
    struct host_addr addr;
    struct host_addrs * next;
};


struct sock_list {
    SOCKET sock;
    int live;
    int type;
    SOCKET listen_sock;
    struct host_addr localaddr;
    struct host_addr peeraddr;
    struct sock_list * next;
};

int sock_init();
int sock_destroy();
struct host_addr string2addr(const char * hostport);

#endif // _SOCK_H_

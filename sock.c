#include <string.h>
#include <stdlib.h>

#include "sock.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif


int sock_init()
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    return 0;
}


int sock_destroy()
{
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}


struct host_addr string2addr(const char * hostport)
{
    struct host_addr ha;
    char * pch;
    char host[256];

    memset(&ha, 0, sizeof(ha));

    if (strlen(hostport) < 256) {
        strcpy(host, hostport);
        pch = strchr(host, ':');
        if (pch == NULL) {
            strcpy(ha.host, "127.0.0.1");
            ha.port = atoi(host);
        } else {
            *pch = '\0';
            strcpy(ha.host, host);
            ha.port = atoi(pch + 1);
        }
    }
    return ha;
}

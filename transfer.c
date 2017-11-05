
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "sock.h"
#include "utils.h"

#include "transfer.h"

extern int verbose;
extern int http_host;

int transfer(struct host_addrs * listen_addrs, struct host_addrs * connect_addrs)
{
    struct host_addrs * hostnode;
    struct sock_list * socks=NULL, * sock_node;
    SOCKET sock;

    // listen address
    hostnode = listen_addrs;
    while (hostnode != NULL) {
        sock = listen_addr(&(hostnode->addr));
        if (sock != INVALID_SOCKET) {
            sock_node = (struct sock_list *) malloc(sizeof(struct sock_list));
            sock_node->sock = 0;
            sock_node->listen_sock = sock;
            sock_node->next = socks;
            sock_node->live = 0;
            sock_node->type = 1;
            sock_node->localaddr = hostnode->addr;
            memset(&(sock_node->peeraddr), 0, sizeof(struct host_addr));
            socks = sock_node;
        }
        hostnode = hostnode->next;
    }

    // connect to
    hostnode = connect_addrs;
    while (hostnode != NULL) {
        sock = connect_addr(&(hostnode->addr));
        if (sock != INVALID_SOCKET) {
            sock_node = (struct sock_list *) malloc(sizeof(struct sock_list));
            sock_node->sock = sock;
            sock_node->listen_sock = 0;
            sock_node->next = socks;
            sock_node->live = 1;
            sock_node->type = 0;
            memset(&(sock_node->localaddr), 0, sizeof(sock_node->localaddr));
            sock_node->peeraddr = hostnode->addr;
            socks = sock_node;
        }
        hostnode = hostnode->next;
    }
    do_transfer(&socks);
    return 0;
}


int do_transfer(struct sock_list ** socks)
{
    struct sock_list * sock_node;
    fd_set fds;
    SOCKET sock;
    int maxfd;
    int count;
    while (1) {
        FD_ZERO(&fds);
        sock_node = *socks;
        maxfd = 0;
        while (sock_node != NULL) {
            if (sock_node->live > 0) {
                // contected socket, data arrived
                FD_SET(sock_node->sock, &fds);
                maxfd = maxfd < sock_node->sock ? sock_node->sock : maxfd;
            } else if (sock_node->listen_sock > 0) {
                // prepre accept new connecting
                FD_SET(sock_node->listen_sock, &fds);
                maxfd = maxfd < sock_node->listen_sock ?
                    sock_node->listen_sock : maxfd;
            } else {
                // connect to...
                sock = connect_addr(&(sock_node->peeraddr));
                if (sock != INVALID_SOCKET) {
                    sock_node->live = 1;
                    sock_node->sock = sock;
                    FD_SET(sock_node->sock, &fds);
                    maxfd = maxfd < sock_node->sock ? sock_node->sock : maxfd;
                }
            }
            sock_node = sock_node->next;
        }
        if (maxfd == 0) {
            break;
        }
        count = select(maxfd + 1, &fds, NULL, NULL, NULL);
        sock_node = *socks;
        while (count > 0 && sock_node != NULL) {
            if (sock_node->live > 0 &&
                    FD_ISSET(sock_node->sock, &fds)) {
                // transfer data
                count --;
                transfer_OneToMany(sock_node->sock, socks);
            } else if (sock_node->listen_sock > 0 &&
                    FD_ISSET(sock_node->listen_sock, &fds)) {
                // accept new connecting
                struct sockaddr_in addr_accept;
                int addrlen = sizeof(struct sockaddr_in);
                sock = accept(sock_node->listen_sock,
                        (struct sockaddr*)&addr_accept, &addrlen);
                if(sock != INVALID_SOCKET) {
                    sock_node->sock = sock;
                    sock_node->live = 1;
                    strcpy(sock_node->peeraddr.host,
                            inet_ntoa(addr_accept.sin_addr));
                    sock_node->peeraddr.port = ntohs(addr_accept.sin_port);
                    printf("connected from %s:%d to %s:%d\n",
                            sock_node->peeraddr.host,
                            sock_node->peeraddr.port,
                            sock_node->localaddr.host,
                            sock_node->localaddr.port
                            );
                } else {
                    printf("accept: %s\n", strerror(errno));
                }
            }
            sock_node = sock_node->next;
        }
    }
    return 0;
}


int transfer_OneToMany(SOCKET sock, struct sock_list ** socks)
{
    struct sock_list * sock_node, * cur_node;
    int nbytes;
    unsigned char buffer[MAX_SOCKBUFFER];
    struct sockaddr_in addr;
    nbytes = recv(sock, buffer, MAX_SOCKBUFFER, 0);
    cur_node = *socks;
    while (cur_node != NULL && cur_node->sock != sock) {
        cur_node = cur_node->next;
    }
    if (nbytes < 1) {
        closesocket(cur_node->sock);
        cur_node->live = 0;
        printf("disconnected %s:%d\n",
                cur_node->peeraddr.host, cur_node->peeraddr.port);
    } else {
        printf("recv %d bytes from %s:%d\n",
                nbytes,
                cur_node->peeraddr.host, cur_node->peeraddr.port);
    }
    sock_node = *socks;
    while (sock_node != NULL) {
        if (sock_node->live > 0 && sock_node != cur_node) {
            if (nbytes > 0) {
                if (http_host > 0 && sock_node->type == 0) {
                    printf("fix http host, origin:\n");
                    buffer[nbytes] = '\0';
                    output_hex(buffer, nbytes);
                    nbytes = fix_http_host(
                            sock_node->peeraddr.host,
                            sock_node->peeraddr.port,
                            buffer, nbytes);
                }
                printf("send %d bytes to %s:%d\n", nbytes,
                        sock_node->peeraddr.host, sock_node->peeraddr.port);
                send(sock_node->sock, buffer, nbytes, 0);
            } else {
                // disconnect all node
                closesocket(sock_node->sock);
                sock_node->live = 0;
                printf("disconnected %s:%d\n",
                        sock_node->peeraddr.host, sock_node->peeraddr.port);
            }
        }
        sock_node = sock_node->next;
    }
    if (nbytes > 0 && verbose > 0) {
        output_hex(buffer, nbytes);
    }
    return 0;
}


SOCKET connect_addr(const struct host_addr * haddr)
{
    printf("connect to %s:%d\n", haddr->host, haddr->port);

    SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("socket: %s\n", strerror(errno));
        closesocket(sock);
        return INVALID_SOCKET;
    }
    struct sockaddr_in addr_target;
    memset(&addr_target, 0, sizeof(addr_target));
    addr_target.sin_family = AF_INET;
    struct hostent *hp;
    if ((hp=gethostbyname(haddr->host)) != NULL) {
        memcpy(&addr_target.sin_addr, hp->h_addr_list[0], hp->h_length);
    } else {
        printf("gethostbyname: %s\n", strerror(errno));
        closesocket(sock);
        return INVALID_SOCKET;
    }
    addr_target.sin_port = htons(haddr->port);

    if (connect(sock,
                (struct sockaddr*)&addr_target, sizeof(addr_target)) != 0) {
        printf("connect: %s\n", strerror(errno));
        closesocket(sock);
        return INVALID_SOCKET;
    }
    return sock;
}


SOCKET listen_addr(const struct host_addr * haddr)
{
    printf("listen on %s:%d\n", haddr->host, haddr->port);

    SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("socket: %s\n", strerror(errno));
        closesocket(sock);
        return INVALID_SOCKET;
    }
    int val=1; /* 0:off 1:on */
    int addrlen;
    struct sockaddr_in addr;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
            (const char *)&val, sizeof(val));
    addr.sin_family = AF_INET;
    struct hostent *hp;
    if ((hp=gethostbyname(haddr->host)) != NULL) {
        memcpy(&addr.sin_addr, hp->h_addr_list[0], hp->h_length);
    } else {
        printf("gethostbyname: %s\n", strerror(errno));
        closesocket(sock);
        return INVALID_SOCKET;
    }
    addr.sin_port = htons(haddr->port);
    addrlen = sizeof(struct sockaddr_in);
    if (bind(sock, (struct sockaddr*)&addr, addrlen) != 0) {
        printf("bind: %s\n", strerror(errno));
        closesocket(sock);
        return INVALID_SOCKET;
    }
    if (listen(sock, 1) != 0) {
        printf("listen: %s\n", strerror(errno));
        closesocket(sock);
        return INVALID_SOCKET;
    }
    return sock;
}

int fix_http_host(const char *host, int port, unsigned char buffer[], int nbytes)
{
    char tmp_buffer[MAX_SOCKBUFFER];
    const char crln[] = "\x0d\x0a";
    char *pch;

    buffer[nbytes] = '\0';
    pch = strstr((char *)buffer, "Host: ");
    if (pch != NULL) {
        *pch = '\0';
        strcpy(tmp_buffer, (char *)buffer);

        sprintf(tmp_buffer + strlen(tmp_buffer), "Host: %s:%d\r\n", host, port);

        pch = strstr(pch + 2, crln);
        strcat(tmp_buffer, pch + 2);
    }

    strcpy((char*)buffer, tmp_buffer);
    return strlen((char *)buffer);
}

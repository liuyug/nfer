#ifndef _TRANSFER_H_
#define _TRANSFER_H_

int transfer(struct host_addrs * listen_addrs, struct host_addrs * connect_addrs);
int do_transfer(struct sock_list ** socks);
int transfer_OneToMany(SOCKET sock, struct sock_list ** socks);
SOCKET listen_addr(const struct host_addr * haddr);
SOCKET connect_addr(const struct host_addr * haddr);

int fix_http_host(const char *host, int port, unsigned char buffer[], int nbytes);
#endif

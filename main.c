#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#include "sock.h"
#include "transfer.h"
#include "utils.h"

#define version "1.0"

int verbose = 0;
int http_host = 0;


void appinfo()
{
    printf("Network Forwarder v%s\n"
            "Copyright (C) LIU Yugang <liuyug@gmail.com>\n\n", version);
}


void usage(const char * prog)
{
    printf(""
            "Usage: %s [options]...\n"
            "   -h, --help          print this message\n"
            "   -v <number>         verbose level, 0-9.\n"
            "\n"
            "options:\n"
            "   -c <host:port>      connect to host:port\n"
            "   -l <host:port>      listen on host:port\n"
            "   --http-host         fix HTTP host\n"
            "\n", prog);
}


int main(int argc, char **argv)
{
    char * prog = *argv;

    appinfo();

    int opt;
    int option_index = 0;
    static struct option long_options[] = {
        {"help", no_argument, 0, 0},
        {"http-host", no_argument, 0, 0},
        {0, 0, 0, 0}
    };
    struct host_addrs * listen_addrs=NULL, * connect_addrs=NULL, * hostnode;

    while (1) {
        opt = getopt_long(argc, argv,
                "hv:l:c:",
                long_options, &option_index);
        if (opt == -1)
            break;

        switch (opt)
        {
        case 0:
            if (strcmp(long_options[option_index].name, "help") == 0) {
                usage(prog);
                return 0;
            }
            if (strcmp(long_options[option_index].name, "http-host") == 0) {
                http_host = 1;
            }
            break;
        case 'h':
            usage(prog);
            return 0;
        case 'l':
            hostnode = (struct host_addrs *) malloc(sizeof(struct host_addrs));
            hostnode->addr = string2addr(optarg);
            hostnode->next = listen_addrs;
            listen_addrs = hostnode;
            break;
        case 'c':
            hostnode = (struct host_addrs *) malloc(sizeof(struct host_addrs));
            hostnode->addr = string2addr(optarg);
            hostnode->next = connect_addrs;
            connect_addrs = hostnode;
            break;
        case 'v':
            verbose = atoi(optarg);
            break;
        default:
            printf("\nTry '%s --help' for more options.\n", prog);
            return 1;
        }
    }
    while (optind < argc) {
        printf("%s", argv[optind++]);
    }

    if (listen_addrs == NULL && connect_addrs == NULL) {
        usage(prog);
        return 1;
    }

    sock_init();
    int ret = transfer(listen_addrs, connect_addrs);
    sock_destroy();
    return ret;
}

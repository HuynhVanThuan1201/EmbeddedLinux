#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>    
#include <sys/socket.h>  
#include <netinet/in.h>   
#include <unistd.h>
#include <arpa/inet.h>

#include "command.h"
#include "server.h"
#include "connection.h"

char local_ip[INET_ADDRSTRLEN] = "0.0.0.0";
int listen_port = 0;

void print_help(void) {
    printf("***************************************************************\n");
    printf("******************* Chat Application *************************\n");
    printf("***************************************************************\n");
    printf("Use the commands below:\n");
    printf("1. help                          : display user interface options\n");
    printf("2. myip                          : display IP address of this app\n");
    printf("3. myport                        : display listening port of this app\n");
    printf("4. connect <destination> <port>  : connect to the app of another computer\n");
    printf("5. list                          : list all the connections of this app\n");
    printf("6. terminate <connection id>     : terminate a connection\n");
    printf("7. send <connection id> <msg>    : send a message to a connection\n");
    printf("8. exit                          : close all connections & terminate this app\n");
    printf("***************************************************************\n\n");
}

void detect_local_ip(void) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return;

    struct sockaddr_in serv = {0};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(53);
    inet_pton(AF_INET, "8.8.8.8", &serv.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv, sizeof(serv)) == 0) {
        struct sockaddr_in name;
        socklen_t len = sizeof(name);
        if (getsockname(sock, (struct sockaddr*)&name, &len) == 0)
            inet_ntop(AF_INET, &name.sin_addr, local_ip, sizeof(local_ip));
    }
    close(sock);
}

void cmd_connect(char *ipstr, char *portstr) {
    if (!ipstr || !portstr) { printf("Usage: connect <ip> <port>\n"); return; }

    int port = atoi(portstr);
    printf("%d/n", port);

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ipstr, &serv_addr.sin_addr);

    // // Check and block self-connection attempt
    // if (strcmp(ipstr, local_ip) == 0 && port == listen_port) {
    //     printf("Refusing to connect to self\n");
    //     return;
    // }

    // // Check and block duplicate connection attempt
    // if (exists_connection_with_addr(&serv_addr)) {
    //     printf("Already connected\n");
    //     return;
    // }

    // int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    // if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    //     perror("connect");
    //     close(sock_fd);
    //     return;
    // }

    int id = add_new_connection(sock_fd, &serv_addr);
    printf("Connected to %s:%d (id=%d)\n", ipstr, port, id);
}
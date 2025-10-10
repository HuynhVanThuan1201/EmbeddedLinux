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
#include "util.h"

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

    struct sockaddr_in serv_addr = {0};

    // Init Server Information
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ipstr, &serv_addr.sin_addr) == -1)
        handle_error("inet_pton()");

    // Check and block self-connection attempt
    if (strcmp(ipstr, local_ip) == 0 && port == listen_port) {
        printf("Refusing to connect to self\n");
        return;
    }

    // Check and block duplicate connection attempt
    if (exists_connection_with_addr(&serv_addr)) {
        printf("Already connected\n");
        return;
    }

    // Create client Socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1)
        handle_error("socket()");

    // Connect to Server
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("connect()");

    int id = add_new_connection(client_fd, &serv_addr);
    int idx = find_index_by_id(id);
    printf("Connected to %s:%d (ID=%d)\n", ipstr, port, id);

    int *pidx = malloc(sizeof(int));
    *pidx = idx;
    if (pthread_create(&conns[idx].thread_id, NULL, recv_thread_fn, pidx) != 0) {
        perror("pthread_create");
        free(pidx);
        return;
    }
    printf("Ready data for transmisstion\n");
}

void cmd_list(void) {
    pthread_mutex_lock(&conns_mutex);

    printf("\n************************************************\n");
    printf("*  ID  |      IP Address       |   Port No.   *\n");
    printf("************************************************\n");

    int has_conn = 0;
    for (int i = 0; i < MAX_CONN; ++i) {
        if (conns[i].active) {
            has_conn = 1;
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &conns[i].addr.sin_addr, ip, sizeof(ip));
            printf(" %-5d | %-21s | %-11d \n",
                   conns[i].id, ip, ntohs(conns[i].addr.sin_port));
        }
    }

    if (!has_conn) {
        printf("*           No active connections.            *\n");
    }
    printf("************************************************\n");

    pthread_mutex_unlock(&conns_mutex);
}

void cmd_send(char *idstr, char *message) {
    if (!idstr || !message) return;
    int id = atoi(idstr);
    int idx = find_index_by_id(id);
    if (idx < 0) { 
        printf("ID not found\n"); 
        return; 
    }

    if (send(conns[idx].sock_fd, message, strlen(message), 0) == -1) {
        perror("Send failed");
    } else {
        printf("Sent message successfully.\n");
    }
}

void cmd_terminate(char *idstr) {
    if (!idstr) return;
    int id = atoi(idstr);
    int idx = find_index_by_id(id);
    if (idx < 0) { printf("ID not found\n"); return; }

    // Gửi tín hiệu yêu cầu peer thoát
    send(conns[idx].sock_fd, "/peer_exit", strlen("/peer_exit"), 0);

    // Đóng socket của mình (để thread recv thoát ra khỏi recv())
    shutdown(conns[idx].sock_fd, SHUT_RDWR);
    
    pthread_join(conns[idx].thread_id, NULL);

    pthread_mutex_lock(&conns_mutex);
    remove_connection_by_index(idx);
    pthread_mutex_unlock(&conns_mutex);

    printf("Terminated connection with ID = %d successfully\n", id);
}

/* Cleanup all connections */
void cleanup_all(void) {
    pthread_mutex_lock(&conns_mutex);
    for (int i = 0; i < MAX_CONN; ++i) {
        if (conns[i].active) {
            // Gửi tín hiệu cho peer biết là mình thoát
            send(conns[i].sock_fd, "/peer_exit", strlen("/peer_exit"), 0);

            // Ngắt luồng recv_thread_fn() bằng cách shutdown
            shutdown(conns[i].sock_fd, SHUT_RDWR);
        }
    }
    pthread_mutex_unlock(&conns_mutex);

    // Sau khi đã shutdown, chờ các thread recv thoát hẳn
    for (int i = 0; i < MAX_CONN; ++i) {
        if (conns[i].active) {
            pthread_join(conns[i].thread_id, NULL);

            pthread_mutex_lock(&conns_mutex);
            remove_connection_by_index(i);
            pthread_mutex_unlock(&conns_mutex);
        }
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>        // errno, perror

#include <sys/types.h>    // data types for sockets
#include <sys/socket.h>   // socket(), bind(), listen(), accept(), connect(), send(), recv()
#include <netinet/in.h>   // struct sockaddr_in, htons(), htonl()
#include <unistd.h>

#include <pthread.h>

#include "server.h"
#include "command.h"
#include "connection.h"
#include "util.h"

#define LISTEN_BACKLOG 10

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <listen_port>\n", argv[0]);
        return 1;
    }

    connection_table_init();

    listen_port = atoi(argv[1]);
    if (listen_port <= 0 || listen_port > 65535) {
        fprintf(stderr, "Invalid port.\n");
        return 1;
    }

    int opt = 1;
    struct sockaddr_in serv_addr = {0};

    /* 01. Create server socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket()");

    // /* Address already in use */
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    //     handle_error("setsockopt()");

    /* Address already in use */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        handle_error("setsockopt()");
    
    /* 01.1 Init server information */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(listen_port); // convert to network byte order (MSB)
    serv_addr.sin_addr.s_addr = INADDR_ANY; // inet_addr("192.168.5.128");

    /* 02. Binding */
    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind()");

    /* 03. Listening */
    if (listen(server_fd, LISTEN_BACKLOG) == -1)
        handle_error("listen()");   

    pthread_t accept_thread_id;
    pthread_create(&accept_thread_id, NULL, accept_thread_fn, NULL);

    print_help();
    detect_local_ip();
    printf("P2P Chat started on %s:%d\n", local_ip, listen_port); 

    char *line = NULL;
    size_t cap = 0;
    while (1) {
        printf("\nEnter your command: ");
        ssize_t len = getline(&line, &cap, stdin);

        if (len <= 0) continue;
        if (line[len - 1] == '\n') line[len - 1] = '\0';
        char *cmd = strtok(line, " ");
        if (!cmd) continue;

        if (strcmp(cmd, "help") == 0) {
            print_help();
        }
        else if (strcmp(cmd, "myip") == 0) {
            printf("Your local IP address is: %s\n", local_ip);
        } 
        else if (strcmp(cmd, "myport") == 0) {
            printf("Your program is currently listening on port: %d\n", listen_port);
        }
        else if (strcmp(cmd, "connect") == 0) {
            char *ip = strtok(NULL, " ");
            char *port = strtok(NULL, " ");
            cmd_connect(ip, port);
        }
        else if (strcmp(cmd, "list") == 0) {
            cmd_list();
        }
        else if (strcmp(cmd, "send") == 0) {
            char *id = strtok(NULL, " ");
            char *message = strtok(NULL, "");
            if (!message) {
                printf("Usage: send <id> <message>\n");
                continue;
            }
            cmd_send(id, message);
        }
        else if (strcmp(cmd, "terminate") == 0) {
            char *id = strtok(NULL, " ");
            cmd_terminate(id);
        }
        else if (strcmp(cmd, "exit") == 0) {
            cleanup_all();
            break;
        }
        else {
            printf("Unknown command: '%s'\n", cmd);
            printf("Type 'help' to see available commands.\n");
        }
    }

    free(line);
    pthread_cancel(accept_thread_id);
    close(server_fd);
    return 0;
}
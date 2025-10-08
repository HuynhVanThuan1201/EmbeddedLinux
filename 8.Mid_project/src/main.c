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

#define LISTEN_BACKLOG 10
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

static void *accept_thread_fn(void *args)
{
    printf("Hi, I'm thread handler1\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <listen_port>\n", argv[0]);
        return 1;
    }

    listen_port = atoi(argv[1]);
    detect_local_ip();

    int server_fd;
    int opt = 1;
    struct sockaddr_in serv_addr = {0};

    /* 01. Create socket */
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

    pthread_join(accept_thread_id, NULL);
    print_help();
    printf("P2P Chat started on %s:%d\n", local_ip, listen_port);

    char *line = NULL;
    size_t cap = 0;
    while (1) {
        printf("\n> ");
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
            cmd_connect(strtok(NULL, " "), strtok(NULL, " "));
        }
        else {
            printf("Unknown command: '%s'\n", cmd);
            printf("Type 'help' to see available commands.\n");
        }
    }

    free(line);
    close(server_fd);
    return 0;
}
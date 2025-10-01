#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define LISTEN_BACKLOG 50
#define BUFF_SIZE 256
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    int port_no, len, opt;
    int server_fd, new_socket_fd;
    struct sockaddr_in serv_addr, client_addr;

    /* Command line argument: port number */
    if (argc < 2) {
        printf("No port provided\ncommand: ./server <port number>\n");
        exit(EXIT_FAILURE);
    } else {
        port_no = atoi(argv[1]);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));

    /* 01. Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket()");

    /* Address already in use */
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    //     handle_error("setsockopt()");

    /* 01.1 Init server information */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no); // convert to network byte order (MSB)
    serv_addr.sin_addr.s_addr = INADDR_ANY; // inet_addr("192.168.5.128");

    /* 02. Binding */
    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind()");

    /* 03. Listening */
    if (listen(server_fd, LISTEN_BACKLOG) == -1)
        handle_error("listen()");

    // Caculated size of client_addr
    len = sizeof(client_addr);
    while (1) {
        printf("Server is listening at port : %d \n....\n", serv_addr.sin_port);
        new_socket_fd = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t *)&len);
        if (new_socket_fd == -1)
            handle_error("accept()");

        printf("Server : got connection \n");
        write(new_socket_fd, "hello\n", sizeof("hello\n"));
        close(new_socket_fd);
    }
    close(server_fd);
    return 0;
}
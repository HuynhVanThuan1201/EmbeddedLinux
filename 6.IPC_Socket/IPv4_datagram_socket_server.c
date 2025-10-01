#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFF_SIZE 256
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    int port_no;
    int server_fd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t len;
    char buffer[BUFF_SIZE];

    /* Command line argument: port number */
    if (argc < 2) {
        printf("No port provided\ncommand: ./server <port number>\n");
        exit(EXIT_FAILURE);
    } else {
        port_no = atoi(argv[1]);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    /* 01. Create UDP socket */
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1)
        handle_error("socket()");

    /* 02. Init server information */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* 03. Binding */
    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind()");

    printf("UDP Server is running at port: %d\n", port_no);

    /* 04. Loop: Receive & Send */
    while (1) {
        len = sizeof(client_addr);
        memset(buffer, 0, BUFF_SIZE);

        // nhận dữ liệu từ client
        int n = recvfrom(server_fd, buffer, BUFF_SIZE, 0,
                         (struct sockaddr*)&client_addr, &len);
        if (n < 0)
            handle_error("recvfrom()");

        buffer[n] = '\0';
        printf("Received from client [%s:%d]: %s\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               buffer);

        // gửi phản hồi cho client
        char reply[] = "Hello from UDP server!";
        sendto(server_fd, reply, strlen(reply), 0,
               (struct sockaddr*)&client_addr, len);
    }

    close(server_fd);
    return 0;
}


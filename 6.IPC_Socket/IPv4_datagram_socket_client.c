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
    int portno;
    int client_fd;
    struct sockaddr_in serv_addr;
    char send_buffer[BUFF_SIZE];
    char recv_buffer[BUFF_SIZE];
    socklen_t serv_len;

    if (argc < 3) {
        printf("command: ./client <server address> <port number>\n");
        exit(1);
    }
    portno = atoi(argv[2]);

    memset(&serv_addr, 0, sizeof(serv_addr));

    /* Init server information */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
        handle_error("inet_pton()");

    /* Create UDP socket */
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd == -1)
        handle_error("socket()");

    printf("UDP Client started. Sending message to %s:%d ...\n", argv[1], portno);

    /* Send data to server */
    strcpy(send_buffer, "Hello from UDP client!");
    serv_len = sizeof(serv_addr);
    if (sendto(client_fd, send_buffer, strlen(send_buffer), 0,
               (struct sockaddr *)&serv_addr, serv_len) == -1)
        handle_error("sendto()");

    /* Receive response from server */
    int n = recvfrom(client_fd, recv_buffer, BUFF_SIZE, 0,
                     (struct sockaddr *)&serv_addr, &serv_len);
    if (n < 0)
        handle_error("recvfrom()");

    recv_buffer[n] = '\0';
    printf("Data from server: %s\n", recv_buffer);

    close(client_fd);
    return 0;
}

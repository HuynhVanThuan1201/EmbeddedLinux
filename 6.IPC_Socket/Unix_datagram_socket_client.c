#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "unix_dgram_server"
#define CLIENT_PATH "unix_dgram_client"
#define BUFF_SIZE 256

int main() {
    int client_fd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFF_SIZE];

    // 1. Tạo socket datagram
    client_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Xóa file socket client cũ nếu còn
    unlink(CLIENT_PATH);

    // 3. Thiết lập địa chỉ client
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, CLIENT_PATH, sizeof(client_addr.sun_path) - 1);

    if (bind(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
        perror("bind");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Thiết lập địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    // 5. Gửi dữ liệu tới server
    strcpy(buffer, "Hello from UNIX datagram client!");
    if (sendto(client_fd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Sent to server: %s\n", buffer);

    // 6. Nhận phản hồi từ server
    memset(buffer, 0, BUFF_SIZE);
    ssize_t n = recvfrom(client_fd, buffer, BUFF_SIZE, 0, NULL, NULL);
    if (n == -1) {
        perror("recvfrom");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Received from server: %s\n", buffer);

    // 7. Đóng socket
    close(client_fd);
    unlink(CLIENT_PATH);

    return 0;
}

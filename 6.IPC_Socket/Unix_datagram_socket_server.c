#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "unix_dgram_server"
#define BUFF_SIZE 256

int main() {
    int server_fd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFF_SIZE];
    socklen_t client_len;

    // 1. Tạo socket datagram
    server_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Xóa file socket cũ nếu còn
    unlink(SERVER_PATH);

    // 3. Thiết lập địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    // 4. Bind socket với đường dẫn
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("UNIX Datagram Server listening at %s...\n", SERVER_PATH);

    // 5. Nhận dữ liệu từ client
    while (1) {
        memset(buffer, 0, BUFF_SIZE);
        client_len = sizeof(client_addr);

        ssize_t n = recvfrom(server_fd, buffer, BUFF_SIZE, 0,
                             (struct sockaddr *)&client_addr, &client_len);
        if (n == -1) {
            perror("recvfrom");
            continue;
        }

        printf("Received from client: %s\n", buffer);

        // 6. Gửi phản hồi lại client
        strcpy(buffer, "Hello from UNIX datagram server!");
        sendto(server_fd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&client_addr, client_len);
    }

    close(server_fd);
    unlink(SERVER_PATH);
    return 0;
}

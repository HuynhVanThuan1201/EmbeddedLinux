#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "unix_sock"
#define BUFF_SIZE 256

int main() {
    int client_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFF_SIZE];

    // 1. Tạo socket
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Thiết lập địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // 3. Kết nối tới server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to UNIX server.\n");

    // 4. Gửi dữ liệu tới server
    strcpy(buffer, "Hello from UNIX client!");
    write(client_fd, buffer, strlen(buffer));

    // 5. Nhận phản hồi từ server
    memset(buffer, 0, BUFF_SIZE);
    read(client_fd, buffer, BUFF_SIZE);
    printf("Received from server: %s\n", buffer);

    // 6. Đóng socket
    close(client_fd);

    return 0;
}

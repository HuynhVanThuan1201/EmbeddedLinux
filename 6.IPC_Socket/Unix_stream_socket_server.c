#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "unix_sock"   // file socket
#define BUFF_SIZE 256

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFF_SIZE];
    socklen_t client_len;

    // 1. Tạo socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Xóa file socket cũ nếu còn tồn tại
    unlink(SOCKET_PATH);

    // 3. Thiết lập địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // 4. Bind
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 5. Listen
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("UNIX Stream Server is listening at %s...\n", SOCKET_PATH);

    // 6. Accept
    client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // 7. Nhận dữ liệu từ client
    memset(buffer, 0, BUFF_SIZE);
    read(client_fd, buffer, BUFF_SIZE);
    printf("Received from client: %s\n", buffer);

    // 8. Gửi phản hồi
    strcpy(buffer, "Hello from UNIX server!");
    write(client_fd, buffer, strlen(buffer));

    // 9. Đóng kết nối
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}

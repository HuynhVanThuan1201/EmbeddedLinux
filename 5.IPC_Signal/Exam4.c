#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/select.h>

// Handler cho SIGINT
void handle_sigint(int sig) {
    printf("SIGINT received.\n");
}

// Handler cho SIGTERM
void handle_sigterm(int sig) {
    printf("SIGTERM received. Exiting...\n");
    exit(0);
}

int main() {
    char buffer[256];

    // Đăng ký signal handler
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigterm);

    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        // Timeout = NULL → chờ vô hạn đến khi có dữ liệu hoặc tín hiệu
        int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL);

        if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    buffer[strcspn(buffer, "\n")] = '\0'; // bỏ dấu xuống dòng
                    printf("You typed: %s\n", buffer);
                }
            }
        }
    }

    return 0;
}

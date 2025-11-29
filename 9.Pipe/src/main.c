#include "../include/pipe_utils.h"

int main(void) {
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // Tiến trình con 1: ls -l
    pid_t pid_ls = create_process("ls", "-l", -1, pipefd[1]);

    // Tiến trình con 2: wc -l
    pid_t pid_wc = create_process("wc", "-l", pipefd[0], -1);

    // Đóng cả hai đầu pipe trong cha
    close(pipefd[0]);
    close(pipefd[1]);

    // Đợi tiến trình con kết thúc
    waitpid(pid_ls, NULL, 0);
    waitpid(pid_wc, NULL, 0);

    return 0;
}

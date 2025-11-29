#include "../include/pipe_utils.h"

pid_t create_process(const char *cmd, const char *arg, int input_fd, int output_fd) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // Tiến trình con
        if (input_fd != -1) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != -1) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        execlp(cmd, cmd, arg, (char *)NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }

    // Tiến trình cha
    return pid;
}

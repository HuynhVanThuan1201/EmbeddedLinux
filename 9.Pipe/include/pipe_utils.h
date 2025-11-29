#ifndef PIPE_UTILS_H
#define PIPE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * @brief Tạo tiến trình thực thi lệnh và kết nối qua pipe
 * 
 * @param cmd Lệnh (vd: "ls")
 * @param arg Tham số (vd: "-l")
 * @param input_fd File descriptor dùng làm stdin (nếu != -1)
 * @param output_fd File descriptor dùng làm stdout (nếu != -1)
 * @return pid_t PID của tiến trình con
 */
pid_t create_process(const char *cmd, const char *arg, int input_fd, int output_fd);

#endif

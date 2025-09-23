#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Hàm xử lý tín hiệu trong process con
void sigusr1_handler(int sig) {
    if (sig == SIGUSR1) {
        printf("Received signal from parent\n");
        fflush(stdout); // đảm bảo in ra ngay
    }
}

int main(int argc, char const *argv[])
{
    int retVal;
    int status;
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        /* Children 1 Process */
        signal(SIGUSR1, sigusr1_handler); // Đăng ký hàm xử lý SIGUSR1
        while (1) {
            pause(); 
        }

    } else if (pid > 0) {
        /* Parent Process (main) */
        for (int i = 0; i < 5; i++) {
            sleep(2);
            kill(pid, SIGUSR1); // gửi tín hiệu cho con
        }        
        printf("Parent finished sending signals\n");
        fflush(stdout);

        // Kết thúc process con luôn
        kill(pid, SIGKILL);
        retVal = wait(&status);
        if (retVal == -1) {
            printf("wait() unsuccessful\n");
        }
        if (WIFEXITED(status)) {
            printf("Normally termination, status = %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Killed by signal, value = %d\n", WTERMSIG(status));
        }

    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int ret, retVal;
    int status;

    ret = fork();
    if (ret == 0) {
        /* Children 1 Process */
        printf("I'm child process. My PID is: %d\n", getpid());
        exit(10);
    } else if (ret > 0) {
        /* Parent Process (main) */
        retVal = wait(&status);
        if (retVal == -1) {
            printf("wait() unsuccessful\n");
        }

        printf("I'm parent process. My PID is: %d and my child PID is: %d\n", getpid(), retVal);
    
        if (WIFEXITED(status)) {
            printf("Normally termination, status = %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Killed by signal, value = %d\n", WTERMSIG(status));
        }

    }

    return 0;
}
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
        printf("My child PID is: %d\n", getpid());
        exit(1);
    } else if (ret > 0) {
        /* Parent Process (main) */
        // while(1)
        sleep(120);
        retVal = wait(&status);
        printf("My parent PID is: %d, The child PID is: %d\n", getpid(), retVal);
    }

    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int ret, retVal;
    int status, count = 0;

    ret = fork();
    if (ret == 0) {
        /* Children 1 Process */
        printf("My child PID is: %d\n", getpid());
        //while(1);
        while(count < 600){   
            count ++;
            printf("The Parent PID is: %d\n", getppid());
            sleep(1);
        }
        exit(1);
    } else if (ret > 0) {
        /* Parent Process (main) */
        retVal = wait(&status);
        printf("My parent PID is: %d, The child PID is: %d\n", getpid(), retVal);
    }

    return 0;
}



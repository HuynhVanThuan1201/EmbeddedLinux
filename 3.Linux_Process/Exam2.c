#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int ret, retVal;
    int status;

    setenv("MY_COMMAND", "ls", 1); // 1: ghi đè nếu đã tồn tại
    ret = fork();
    if (ret == 0) {
        /* Children 1 Process */
        char *cmd = getenv("MY_COMMAND");
        execlp(cmd, cmd, (char *)NULL); // Thực thi lệnh ls (execlp sẽ thay thế tiến trình con)
        exit(10);
    } else if (ret > 0) {
        /* Parent Process (main) */                
        retVal = wait(&status);
    }

    return 0;
}
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

volatile sig_atomic_t sigint_count = 0;

void sig_handler1(int num)
{
    // printf("\nIm signal handler1: %d\n", num);
    sigint_count++;
    printf("\nSIGINT received (%d)\n", sigint_count);

    if (sigint_count >= 3) {
        printf("\nĐã nhận SIGINT 3 lần, chương trình sẽ kết thúc.\n");
        _exit(0); // thoát ngay
    }
}

int main()
{
    if (signal(SIGINT, sig_handler1) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGINT\n");
        exit(EXIT_FAILURE);
    }
    printf("Chương trình đang chạy. Nhấn Ctrl+C để gửi SIGINT.\n");

    while(1);
    return 0;
}
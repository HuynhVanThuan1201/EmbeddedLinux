#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Biến đếm toàn cục, dùng sig_atomic_t để an toàn trong signal handler
volatile sig_atomic_t counter = 0;

// Hàm xử lý tín hiệu SIGALRM
void timer_handler(int sig) {
    counter++;
    printf("Timer: %d seconds\n", counter);

    if (counter >= 10) {
        printf("Đã đếm đến 10 giây, chương trình kết thúc.\n");
        _exit(0);  // thoát ngay lập tức
    }

    // Đặt lại alarm cho 1 giây tiếp theo
    alarm(1);
}

int main() {
    // Đăng ký hàm xử lý SIGALRM
    if (signal(SIGALRM, timer_handler) == SIG_ERR) {
        perror("Không thể xử lý SIGALRM");
        exit(EXIT_FAILURE);
    }

    printf("Bắt đầu đếm thời gian...\n");

    // Bắt đầu timer sau 1 giây
    alarm(1);

    while (1) {
        pause(); // treo tiến trình cho đến khi nhận tín hiệu
    }

    return 0;
}

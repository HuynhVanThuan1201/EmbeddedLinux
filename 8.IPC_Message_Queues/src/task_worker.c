/*
 * task_worker.c
 *
 * Worker: lắng nghe POSIX message queue và xử lý các task theo priority.
 *
 * Biên dịch:
 *   gcc task_worker.c -o task_worker -lrt
 *
 * Chạy:
 *   ./task_worker
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <signal.h>
#include <errno.h>
#include <unistd.h>

const char* QUEUE_NAME = "/my_task_queue";

/* Các tham số queue mặc định (có thể điều chỉnh) */
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 1024
#define BATCH_SIZE 3

static mqd_t mqdes = (mqd_t)-1;
static volatile sig_atomic_t terminate = 0;

static void handle_sigint(int signo) {
    (void)signo;
    terminate = 1;
    /* Không làm nhiều trong signal handler */
}

int main(void) {
    struct mq_attr attr;
    unsigned int prio;

    /* thiết lập thuộc tính queue */
    attr.mq_flags = 0;               /* blocking reads */
    attr.mq_maxmsg = MAX_MESSAGES;   /* số phần tử tối đa trong queue */
    attr.mq_msgsize = MAX_MSG_SIZE;  /* kích thước tối đa 1 message */
    attr.mq_curmsgs = 0;

    /* cài đặt signal handler cho SIGINT */
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        /* vẫn tiếp tục vì không phải fatal, nhưng báo lỗi */
    }

    /* mở (tạo nếu chưa có) queue chỉ đọc */
    mqdes = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &attr);
    if (mqdes == (mqd_t)-1) {
        perror("mq_open (worker)");
        exit(EXIT_FAILURE);
    }

    /* cấp buffer dựa trên mq_msgsize */
    char *buf = malloc(attr.mq_msgsize + 1);
    if (!buf) {
        perror("malloc");
        mq_close(mqdes);
        mq_unlink(QUEUE_NAME);
        exit(EXIT_FAILURE);
    }

    printf("task_worker: Listening on queue %s (msgsize=%ld, maxmsg=%ld)\n",
           QUEUE_NAME, (long)attr.mq_msgsize, (long)attr.mq_maxmsg);

    while (!terminate) {
        ssize_t bytes_read = mq_receive(mqdes, buf, attr.mq_msgsize, &prio);
        if (bytes_read >= 0) {
            /* đảm bảo chuỗi kết thúc null để in */
            if (bytes_read < (ssize_t)(attr.mq_msgsize)) buf[bytes_read] = '\0';
            else buf[attr.mq_msgsize] = '\0';
            printf("Processing task (Priority: %u): %s\n", prio, buf);
            /* mô phỏng xử lý */
            sleep(0);
        } else {
            if (errno == EINTR) {
                /* bị signal interrupt */
                if (terminate) break;
                else continue;
            }
            /* lỗi khác: in và tiếp tục */
            perror("mq_receive");
            /* để tránh vòng lặp nóng nếu lỗi liên tục */
            sleep(1);
        }
    }

    printf("\nShutting down task_worker...\n");

    if (mqdes != (mqd_t)-1) {
        if (mq_close(mqdes) == -1) perror("mq_close");
        if (mq_unlink(QUEUE_NAME) == -1) {
            /* Nếu có nhiều worker, lưu ý mq_unlink sẽ xóa queue toàn cục.
               Ở bài tập này worker là chủ sở hữu nên unlink khi thoát. */
            if (errno != ENOENT) perror("mq_unlink");
        } else {
            printf("Queue %s removed.\n", QUEUE_NAME);
        }
    }

    free(buf);
    return 0;
}

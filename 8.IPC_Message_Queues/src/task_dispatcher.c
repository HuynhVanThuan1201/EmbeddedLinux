/*
 * task_dispatcher.c
 *
 * Dispatcher: gửi task (message) vào POSIX message queue cùng với priority.
 *
 * Sử dụng:
 *   ./task_dispatcher <priority> "<task_description>"
 *
 * Biên dịch:
 *   gcc task_dispatcher.c -o task_dispatcher -lrt
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

const char* QUEUE_NAME = "/my_task_queue";

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <priority> \"<task_description>\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr = NULL;
    unsigned long prio_ul = strtoul(argv[1], &endptr, 10);
    if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "Invalid priority: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    unsigned int prio = (unsigned int)prio_ul;

    const char *task = argv[2];

    /* mở queue (chỉ ghi). Nếu queue chưa được tạo, mq_open sẽ lỗi. */
    mqd_t mqdes = mq_open(QUEUE_NAME, O_WRONLY);
    if (mqdes == (mqd_t)-1) {
        perror("mq_open (dispatcher)");
        fprintf(stderr, "Make sure task_worker is running to create the queue.\n");
        return EXIT_FAILURE;
    }

    struct mq_attr attr;
    
    /* lấy thuộc tính để kiểm tra kích thước message tối đa */
    if (mq_getattr(mqdes, &attr) == -1) {
        perror("mq_getattr");
        mq_close(mqdes);
        return EXIT_FAILURE;
    }

    size_t msglen = strlen(task);
    if (msglen > (size_t)attr.mq_msgsize) {
        fprintf(stderr, "Task too long (%zu bytes). Max allowed is %ld bytes.\n", msglen, attr.mq_msgsize);
        mq_close(mqdes);
        return EXIT_FAILURE;
    }

    if (mq_send(mqdes, task, msglen, prio) == -1) {
        perror("mq_send");
        mq_close(mqdes);
        return EXIT_FAILURE;
    }

    printf("Sent task (Priority: %u): %s\n", prio, task);

    mq_close(mqdes);
    return 0;
}

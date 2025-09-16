#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_t thread_id1, thread_id2;

static void *thr_handler1(void *args)
{
    printf("Hi, I'm thread handler1, my address of thread ID1 is %p\n", (void*)pthread_self());
    pthread_exit(NULL);
}

static void *thr_handler2(void *args)
{
    printf("Hi, I'm thread handler2, my address of thread ID2 is %p\n", (void*)pthread_self());
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int ret;

    if ((ret = pthread_create(&thread_id1, NULL, &thr_handler1, NULL))) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    sleep(2);

    if ((ret = pthread_create(&thread_id2, NULL, &thr_handler2, NULL))) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);

    printf("Hi, I'm main, all threads finished!\n");
    return 0;
}

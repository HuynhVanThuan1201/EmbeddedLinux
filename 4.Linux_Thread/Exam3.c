#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define THRESHOLD 10

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int data; // critical section <=> global resource
int data_ready = 0;

static void *handle_th1(void *args)
{
    // Producer */
    for(int i = 0; i < THRESHOLD; i++){   
        pthread_mutex_lock(&lock);
        data = rand() % 100;
        data_ready = 1; //Turn on data_ready flag
        pthread_cond_signal(&cond);
        printf("Producer is sent the signal\n");
        pthread_mutex_unlock(&lock);
        sleep(1);
    }

    pthread_exit(NULL); // exit or return
}

int main(int argc, char const *argv[])
{
    // Consumer */
    int ret;
    pthread_t thread_id1;

    if (ret = pthread_create(&thread_id1, NULL, &handle_th1, NULL)) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    pthread_mutex_lock(&lock);
    for (int i = 0; i < THRESHOLD; i++){   
        printf("----------- Start %d -----------\n", i);
        printf("Customer is waiting ...\n");
        // ready in advance when pthread_cond_signal is called
        pthread_cond_wait(&cond, &lock); // -> pthread_mutex_unlock -> wait -> wakeup (pthread_cond_signal) -> pthread_mutex_lock
        printf("Customer is received the signal\n");
        printf("Value of data: %d\n", data);
        printf("Turn off data_ready flag = %d.\n\n", data_ready = 0);
        sleep(1);
    }
    
    // used to block for the end of a thread and release
    pthread_join(thread_id1, NULL);

    return 0;
}
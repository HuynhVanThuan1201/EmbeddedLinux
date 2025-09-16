#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
long long shared_counter = 0; // shared variable/shared resources/global variable

#define NUM_THREADS 3
#define INCREMENT_PER_THREAD 1000000

static void* increment_counter(void* arg) {
    pthread_mutex_lock(&lock1);
    // critical section
    for (int i = 0; i < INCREMENT_PER_THREAD; i++) {
        shared_counter++;
    }
    pthread_mutex_unlock(&lock1);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int ret;
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        if ((ret = pthread_create(&threads[i], NULL, &increment_counter, NULL))) {
            printf("pthread_create() error number=%d\n", ret);
            return -1;
        }
    }
    
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    printf("Value of share counter: %lld\n", shared_counter);
    printf("Hi, I'm main, all threads finished!\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int shared_data = 0;              
pthread_rwlock_t rwlock;  // read-write lock

static void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_rwlock_rdlock(&rwlock); 
        printf("Reader %d đọc giá trị: %d\n", id, shared_data);
        pthread_rwlock_unlock(&rwlock); 
        sleep(1);
    }
    return NULL;
}

static void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_rwlock_wrlock(&rwlock); 
        shared_data++;
        printf("Writer %d ghi giá trị mới: %d\n", id, shared_data);
        printf("Writer is holding thread write lock in 5 seconds\n");
        int count = 0;
        while(count < 5){        
            printf("Second %d ...\n", count);
            count++;
            sleep(1);
        }
        pthread_rwlock_unlock(&rwlock);  
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int r_ids[NUM_READERS], w_ids[NUM_WRITERS];

    pthread_rwlock_init(&rwlock, NULL);
    // thread Reader
    for (int i = 0; i < NUM_READERS; i++) {
        r_ids[i] = i;
        if (pthread_create(&readers[i], NULL, reader, &r_ids[i]) != 0) {
            perror("pthread_create reader");
            exit(1);
        }
    }

    // thread Writer
    for (int i = 0; i < NUM_WRITERS; i++) {
        w_ids[i] = i;
        if (pthread_create(&writers[i], NULL, writer, &w_ids[i]) != 0) {
            perror("pthread_create writer");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}

#ifndef CONNECTION_H
#define CONNECTION_H

#include <netinet/in.h>
#include <pthread.h>

#define MAX_CONN 100
#define MAX_MSG_LEN 100

typedef struct connection {
    int id;
    int sock_fd;
    struct sockaddr_in client_addr;
    pthread_t thread_id;
    int active;
} connection_t;

extern connection_t conns[MAX_CONN];
extern pthread_mutex_t conns_mutex;

void connection_table_init(void);
int add_new_connection(int sock_fd, struct sockaddr_in *client_addr);
void remove_connection_by_index(int idx);
int find_index_by_id(int id);
void cleanup_all(void);

#endif

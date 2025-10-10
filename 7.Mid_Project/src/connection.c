#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "connection.h"

connection_t conns[MAX_CONN];
pthread_mutex_t conns_mutex = PTHREAD_MUTEX_INITIALIZER;
static int next_conn_id = 1;

void connection_table_init() {
    memset(conns, 0, sizeof(conns));
    for (int i = 0; i < MAX_CONN; ++i) {
        conns[i].sock_fd = -1;
    }
}

/* Add connection to table. Returns id or -1 on error. */
int add_new_connection(int sock_fd, const struct sockaddr_in *addr)
{
    int free_slot_index = -1;

    pthread_mutex_lock(&conns_mutex);

    // 1. Find the first inactive connection slot
    for (int i = 0; i < MAX_CONN; ++i) {
        if (!conns[i].active) {
            free_slot_index = i;
            break;
        }
    }

    // 2. If no free slot found, unlock and return error
    if (free_slot_index == -1) {
        pthread_mutex_unlock(&conns_mutex);
        return -1;
    }

    // 3. Fill connection info
    conns[free_slot_index].active  = 1;
    conns[free_slot_index].sock_fd = sock_fd;
    conns[free_slot_index].addr    = *addr;
    conns[free_slot_index].id      = next_conn_id++;

    pthread_mutex_unlock(&conns_mutex);

    return conns[free_slot_index].id;
}

/* Find index by connection id. Returns -1 if not found. */
int find_index_by_id(int id) {
    for (int i = 0; i < MAX_CONN; ++i)
        if (conns[i].active && conns[i].id == id) return i;
    return -1;
}

void remove_connection_by_index(int idx) {
    if (idx < 0 || idx >= MAX_CONN) return;
    if (conns[idx].active) {
        conns[idx].active = 0;
        close(conns[idx].sock_fd);
    }
}

int exists_connection_with_addr(struct sockaddr_in *addr) {
    for (int i = 0; i < MAX_CONN; ++i) {
        if (conns[i].active &&
            conns[i].addr.sin_addr.s_addr == addr->sin_addr.s_addr &&
            conns[i].addr.sin_port == addr->sin_port)
            return 1;
    }
    return 0;
}

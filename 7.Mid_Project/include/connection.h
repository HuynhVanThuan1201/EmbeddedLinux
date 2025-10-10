/**
 * @file connection.h
 * @brief Connection management module for the P2P chat application.
 *
 * This module provides data structures and functions for managing
 * active peer connections, including adding, removing, and searching.
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <netinet/in.h>
#include <pthread.h>

#define MAX_CONN 100

typedef struct connection {
    int id;                      /**< Unique connection ID. */
    int sock_fd;                 /**< Socket file descriptor for the peer. */
    struct sockaddr_in addr;     /**< Socket address of the peer. */
    pthread_t thread_id;         /**< Thread handling message reception. */
    int active;                  /**< Connection status flag (1 = active, 0 = inactive). */
} connection_t;

extern connection_t conns[MAX_CONN];
extern pthread_mutex_t conns_mutex;

/**
 * @brief Initializes the global connection table and related resources.
 */
void connection_table_init(void);

/**
 * @brief Adds a new connection to the table.
 *
 * @param sock_fd Socket descriptor for the new connection.
 * @param addr Pointer to the peer's socket address structure.
 * @return Unique ID assigned to the new connection, or -1 on failure.
 */
int add_new_connection(int sock_fd, const struct sockaddr_in *addr);

/**
 * @brief Finds the index in the table by connection ID.
 *
 * @param id Connection ID to search for.
 * @return Table index if found, otherwise -1.
 */
int find_index_by_id(int id);

/**
 * @brief Removes a connection from the table by its index.
 *
 * @param idx Index of the connection to remove.
 */
void remove_connection_by_index(int idx);

/**
 * @brief Checks if a connection with a given address already exists.
 *
 * @param addr Pointer to the address structure to check.
 * @return 1 if connection exists, 0 otherwise.
 */
int exists_connection_with_addr(struct sockaddr_in *addr);

#endif /* CONNECTION_H */

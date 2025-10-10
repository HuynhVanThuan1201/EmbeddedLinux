/**
 * @file server.h
 * @brief Server-side thread functions for accepting and receiving peer connections.
 */

#ifndef SERVER_H
#define SERVER_H

extern int server_fd;

/**
 * @brief Thread function that continuously accepts new incoming connections.
 * 
 * @param arg Unused argument (set to NULL).
 * @return NULL Always returns NULL when thread exits.
 */
void *accept_thread_fn(void *arg);

/**
 * @brief Thread function that receives messages from a specific peer.
 * 
 * @param arg Pointer to the connection index (int *).
 * @return NULL Always returns NULL when thread exits.
 */
void *recv_thread_fn(void *arg);

#endif /* SERVER_H */
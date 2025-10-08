#ifndef SERVER_H
#define SERVER_H

extern int server_fd;
extern int listen_port;

void *accept_thread_fn(void *arg);

#endif
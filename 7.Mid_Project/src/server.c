#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "server.h"
#include "util.h"
#include "connection.h"

int server_fd = -1;

/* Receiver thread for a connection */
void *recv_thread_fn(void *arg) {
    int idx = *(int *)arg;
    free(arg);

    int sock_fd = conns[idx].sock_fd;
    struct sockaddr_in client_addr = conns[idx].addr;
    char peer_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, peer_ip, sizeof(peer_ip));
    int peer_port = ntohs(client_addr.sin_port);

    char buf[1024];

    while (1) {
        ssize_t n = recv(sock_fd, buf, sizeof(buf) - 1, 0);
        if (n > 0) {
            buf[n] = '\0';
            if (strcmp(buf, "/peer_exit") == 0) {
                printf("\n[Peer %s:%d] has exited.\n", peer_ip, peer_port);
                fflush(stdout);
                break;
            }
            printf("\nMessage from %s:%d -> %s\n", peer_ip, peer_port, buf);
            fflush(stdout);
        } else if (n == 0) {
            printf("\nConnection closed by peer %s:%d\n", peer_ip, peer_port);
            fflush(stdout);
            break;
        } else {
            if (errno == EINTR) continue;
            perror("recv");
            break;
        }
    }
    
    pthread_mutex_lock(&conns_mutex);
    remove_connection_by_index(idx);
    pthread_mutex_unlock(&conns_mutex);

    return NULL;
}

void *accept_thread_fn(void *arg) {
    (void)arg;
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr); // Caculated size of client_addr
        int new_socket_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (new_socket_fd == -1)
            handle_error("accept()");

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        int client_port = ntohs(client_addr.sin_port);
        printf("\nAccepted a new connection from address: %s:%d\n", client_ip, client_port);
        fflush(stdout);

        // Add new connection to connection of table
        int id = add_new_connection(new_socket_fd, &client_addr);
        int idx = find_index_by_id(id);
        if (id < 0 || idx < 0) continue;

        int *pidx = malloc(sizeof(int));
        *pidx = idx;
        if (pthread_create(&conns[idx].thread_id, NULL, recv_thread_fn, pidx) != 0) {
            perror("pthread_create");
            free(pidx);
            continue;
        }
    }
    return NULL;
}


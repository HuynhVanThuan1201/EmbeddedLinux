#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "server.h"
#include "connection.h"

int server_fd = -1;
int listen_port = 0;
char local_ip[INET_ADDRSTRLEN] = "0.0.0.0";

void detect_local_ip(void) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return;

    struct sockaddr_in serv = {0};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(53);
    inet_pton(AF_INET, "8.8.8.8", &serv.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv, sizeof(serv)) == 0) {
        struct sockaddr_in name;
        socklen_t len = sizeof(name);
        if (getsockname(sock, (struct sockaddr*)&name, &len) == 0)
            inet_ntop(AF_INET, &name.sin_addr, local_ip, sizeof(local_ip));
    }
    close(sock);
}

/* Receiver thread for a connection */
static void *recv_thread_fn(void *arg) {
    int idx = *(int *)arg;
    free(arg);

    int sock_fd = conns[idx].sock_fd;
    char buf[1024];

    while (1) {
        ssize_t n = recv(sock_fd, buf, sizeof(buf) - 1, 0);
        if (n <= 0) break;
        buf[n] = '\0';
        printf("\nMessage -> %s\n", buf);
        fflush(stdout);
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

        // Add to list
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

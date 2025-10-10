/**
 * @file command.h
 * @brief Command handling functions for P2P chat application.
 */

#ifndef COMMAND_H
#define COMMAND_H

extern char local_ip[];
extern int listen_port;

/**
 * @brief Prints the help menu displaying available commands.
 */
void print_help(void);

/**
 * @brief Detects and updates the local IP address.
 */
void detect_local_ip(void);

/**
 * @brief Connects to a remote peer using given IP and port.
 * 
 * @param ipstr  String containing the target IP address.
 * @param portstr String containing the target port number.
 */
void cmd_connect(char *ipstr, char *portstr);

/**
 * @brief Lists all currently active peer connections.
 */
void cmd_list(void);

/**
 * @brief Sends a message to a specific peer.
 * 
 * @param idstr  String containing the connection ID.
 * @param message String containing the message to send.
 */
void cmd_send(char *idstr, char *message);

/**
 * @brief Terminates a specific peer connection.
 * 
 * @param idstr  String containing the connection ID to terminate.
 */
void cmd_terminate(char *idstr);

/**
 * @brief Closes all active connections and cleans up resources.
 */
void cleanup_all(void);

#endif /* COMMAND_H */
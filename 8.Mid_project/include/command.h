#ifndef COMMAND_H
#define COMMAND_H

extern char local_ip[];
extern int listen_port;

void print_help(void);
void detect_local_ip(void);
void cmd_connect(char *ipstr, char *portstr);


#endif
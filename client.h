#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "common.h"


int InitConn(MySocket_p client, int argc, const char *argv[]);
int StartConnEpoll(MySocket_p client);


int client_dispatch(MySocket_p client);

int cmd_init_request(MySocket_p client);
int cmd_init_option(MySocket_p client, MSG *msg);

int cmd_login_request(MySocket_p client);
int cmd_login_option(MySocket_p client, MSG *msg);
int cmd_login_option_admin(MySocket_p client, MSG *msg);

int cmd_updpassword_request(MySocket_p client);
int cmd_updpassword_result(MySocket_p client, MSG *msg);
int cmd_schself_request(MySocket_p client);
int cmd_schself_result(MySocket_p client, MSG *msg);

int cmd_adduser_request(MySocket_p client);
int cmd_adduser_result(MySocket_p client, MSG *msg);
int cmd_deluser_request(MySocket_p client);
int cmd_deluser_result1(MySocket_p client, MSG *msg);
int cmd_deluser_result2(MySocket_p client, MSG *msg);
int cmd_upduser_request(MySocket_p client);
int cmd_upduser_result1(MySocket_p client, MSG *msg);
int cmd_upduser_result2(MySocket_p client, MSG *msg);
int cmd_schuser_request(MySocket_p client);
int cmd_schuser_result(MySocket_p client, MSG *msg);

void *epoll_thread(void *param);
#endif //!__CLIENT_H__
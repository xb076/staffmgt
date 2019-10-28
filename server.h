#ifndef __SERVER_H__
#define __SERVER_H__

#include "common.h"
#include "db.h"

typedef struct SERVER_PARAM
{
	MySocket_p server;
	sqlite3 *db;
	int *on;
}svrPARAM, *svrPARAM_p;


int InitSocket(MySocket_p server, int argc, const char *argv[]);
int StartServerEpoll(MySocket_p server, sqlite3 *db, int *on);

int server_dispatch(MySocket_p client, sqlite3 *db);


int cmd_init_respsonse(MySocket_p client);
int cmd_login_response(MySocket_p client, sqlite3 *db, MSG *msg);
int cmd_login_response_menu(MySocket_p client);

int cmd_updpassword_reponse(MySocket_p client, sqlite3 *db, MSG *msg);
int cmd_schself_response(MySocket_p client, sqlite3 *db, MSG *msg);


int cmd_adduser_respsonse(MySocket_p client, sqlite3 *db, MSG *msg);
int cmd_deluser_respsonse1(MySocket_p client, sqlite3 *db, MSG *msg);
int cmd_deluser_respsonse2(MySocket_p client, sqlite3 *db, MSG *msg);
int cmd_upduser_respsonse1(MySocket_p client, sqlite3 *db, MSG *msg);
int cmd_upduser_respsonse2(MySocket_p client, sqlite3 *db, MSG *msg);
int cmd_schuser_respsonse(MySocket_p client, sqlite3 *db);

int cmd_addlog_reponse(MySocket_p client, sqlite3 *db, char* content);
int cmd_schlog_respsonse(MySocket_p client, sqlite3 *db);

void *epoll_thread(void *param);

#endif //!__SERVER_H__
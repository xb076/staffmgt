#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>

#include "linklistdual.h"

#define err_log(log)\
	  do{\
	  	perror(log);\
		exit(1);\
	  }while(0)  //出错处理的宏函数

#define MAXCLIENTS 128
#define BUFFSIZE 256


typedef struct NET_MSG{
	int datalength;
	char buff[BUFFSIZE - 4];
}MSG, *MSG_p;


typedef struct mySocket{
	int sock;
	struct sockaddr_in sockAddr;
	int status;
	int buff_recv_len;
	int buff_send_len;
	char buff_send[1024];
	char buff_recv[1024];

	char username[32];
	
}MySocket, * MySocket_p;


enum CMD
{
	CMD_INIT=0,
	CMD_INIT_RES,
	CMD_LOGIN,
	CMD_LOGIN_RES,
	CMD_LOGIN_RES_ADMIN,
	CMD_MENU,
	
	CMD_UPD_PASSWORD,
	CMD_UPD_PASSWORD_RES,
	CMD_SCH_SELF,
	CMD_SCH_SELF_RES,

	CMD_ADD_USER,
	CMD_ADD_USER_RES,
	CMD_DEL_USER_1,
	CMD_DEL_USER_RES_1,
	CMD_DEL_USER_2,
	CMD_DEL_USER_RES_2,
	CMD_UPD_USER_1,
	CMD_UPD_USER_RES_1,
	CMD_UPD_USER_2,
	CMD_UPD_USER_RES_2,
	CMD_SCH_USER,
	CMD_SCH_USER_RES,


	CMD_SCH_LOG,
	CMD_SCH_LOG_RES,
	CMD_ERROR
};

typedef struct USERINFO{
	int id;
	int type;
	char username[32];
	char password[32];
	int age;
	char phone[16];
	char addr[64];
	char date[32];
	int level;
	float salary;
}User, *User_p;

typedef struct LOGINFO{
	char time[32];
	char username[32];
	char content[32];

}Log, *Log_p;

void get_time_now(char *date);
void get_today(char *date);
void NETMSG2STRING(MSG_p msg, char *buf);
void STRING2NETMSG(MSG_p msg, char *buf);

void printf_user(User_p user);
void USER2STRING(User_p user, char *buf);
void STRING2USER(User_p user, char *buf);
void LOG2STRING(Log_p log, char *buf);
void STRING2LOG(Log_p log, char *buf);

//int mysocket_read(MySocket_p client);
int SEND_NETMSG(MySocket_p client, int CMD, char* buf);
int mysocket_send(MySocket_p client);




#endif //!__COMMON_H__

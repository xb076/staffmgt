#include "common.h"

void get_time_now(char *date)//获得时间
{
	time_t mytime;
	struct tm *mytm;
	mytime=time(NULL);//得到秒数
	mytm=localtime(&mytime);//得到当前的时间
	sprintf(date,"%04d-%02d-%02d  %02d:%02d:%02d",mytm->tm_year+1900,mytm->tm_mon+1,mytm->tm_mday,\
			mytm->tm_hour,mytm->tm_min,mytm->tm_sec);

}

void get_today(char *date)//获得时间
{
	time_t mytime;
	struct tm *mytm;
	mytime=time(NULL);//得到秒数
	mytm=localtime(&mytime);//得到当前的时间
	sprintf(date,"%04d-%02d-%02d",mytm->tm_year+1900,mytm->tm_mon+1,mytm->tm_mday);

}

void NETMSG2STRING(MSG_p msg, char *buf)
{
	bzero(buf, msg->datalength);
	sprintf(buf, "%d", msg->datalength);
	strncpy(buf+4, msg->buff, msg->datalength-4);
}


void STRING2NETMSG(MSG_p msg, char *buf)
{
	bzero(msg, sizeof(MSG));
	char str[4]={0};
	strncpy(str, buf, sizeof(str));
	msg->datalength=atoi(str);
	strncpy(msg->buff, buf+4, msg->datalength-4);
}


void printf_user(User_p user)
{
	if(NULL==user) return;
	printf("类型 <%s>\n", user->type==100?"普通用户":"管理员");
	printf("姓名 <%s>\n", user->username);
	printf("密码 <%s>\n", user->password);
	printf("年龄 <%d>\n", user->age);
	printf("电话 <%s>\n", user->phone);
	printf("地址 <%s>\n", user->addr);
	printf("日期 <%s>\n", user->date);
	printf("级别 <%d>\n", user->level);
	printf("工资 <%.2f>\n", user->salary);

}

void USER2STRING(User_p user, char *buf)
{
	if(NULL==user || NULL == buf) return;

	/*
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
	*/
	sprintf(buf, "|%d|%d|%s|%s|%d|%s|%s|%s|%d|%.2f|",
		user->id,
		user->type,
		user->username,
		user->password,
		user->age,
		user->phone,
		user->addr,
		user->date,
		user->level,
		user->salary
		);

}

void STRING2USER(User_p user, char *buf)
{
	if(NULL==user || NULL == buf) return;

	bzero(user, sizeof(User));

	char temp[32]={0};
	char *start = NULL;
	char *end = NULL;

	start = buf+1;
	end = strchr(start, '|');
	if(start && end && end>start){
		strncpy(temp, start, end-start);
		user->id = atoi(temp);
		bzero(temp, sizeof(temp));
	}
	else{
		user->id=-1;
	}

	start = end+1;
	end = strchr(start, '|');
	if(start && end && end>start){
		strncpy(temp, start, end-start);
		user->type = atoi(temp);
		bzero(temp, sizeof(temp));
	}
	else{
		user->type=-1;
	}

	if(user->type==1) user->type=100;
	else if(user->type==2) user->type=200;


	start = end+1;
	end = strchr(start, '|');
	if(start && end && end>start){
		strncpy(user->username, start, end-start);
		bzero(temp, sizeof(temp));
	}
	else{
		strcpy(user->username, "None");
	}

	start = end+1;
	end = strchr(start, '|');
	if(start && end && end>start){
		strncpy(user->password, start, end-start);
		bzero(temp, sizeof(temp));
	}
	else{
		strcpy(user->password, "None");
	}

	start = end+1;
	end = strchr(start, '|');
	if(start && end && end>start){
		strncpy(temp, start, end-start);
		user->age = atoi(temp);
		bzero(temp, sizeof(temp));
	}
	else{
		user->age=-1;
	}

	start = end+1;
	end = strchr(start, '|');
	if(start && end && end>start){
		strncpy(user->phone, start, end-start);
		bzero(temp, sizeof(temp));
	}
	else{
		strcpy(user->phone, "None");
	}

	start = end+1;
	end = strchr(start, '|');
	if(start && end && end>start){
		strncpy(user->addr, start, end-start);
		bzero(temp, sizeof(temp));
	}
	else{
		strcpy(user->addr, "None");
	}

	start = end+1;
	end = strchr(start, '|');
	if(start && end && end>start){
		strncpy(user->date, start, end-start);
		bzero(temp, sizeof(temp));
	}
	else{
		strcpy(user->date, "None");
	}

	start = end+1;
	end = strchr(start, '|');
	if(start && end && end>start){
		strncpy(temp, start, end-start);
		user->level = atoi(temp);
		bzero(temp, sizeof(temp));
	}
	else{
		user->level=-1;
	}

	start = end+1;
	end = strchr(start, '|');
	if(start && end && end>start){
		strncpy(temp, start, end-start);
		user->salary = atof(temp);
		bzero(temp, sizeof(temp));
	}
	else{
		user->salary=-1;
	}

}


int SEND_NETMSG(MySocket_p client, int CMD, char* buf)
{
	int ret =0;

	MSG msg;
	bzero(&msg, sizeof(MSG));

	msg.buff[0]=CMD;
	strcpy(msg.buff+1, buf);
	msg.datalength=strlen(buf)+5;
	client->buff_send_len=msg.datalength;
	NETMSG2STRING(&msg, client->buff_send);
	ret = mysocket_send(client);


	return ret;

}


int mysocket_send(MySocket_p client)
{
	int ret=0;
	if(NULL==client) return ret;

	ret=send(client->sock, client->buff_send, client->buff_send_len, 0); 

	bzero(client->buff_send, sizeof(client->buff_send));
	client->buff_send_len = 0;

	return ret;
}







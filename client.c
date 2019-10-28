#ifndef __CLIENT_C__
#define __CLIENT_C__

#include "client.h"


int InitConn(MySocket_p client, int argc, const char *argv[])
{
	char *ip;
	char *port;
	socklen_t len=sizeof(struct sockaddr_in);
	bzero(&client->sockAddr,len);
	int ret = 0;
	//struct NET_MSG msg;

	if(client && argc>=3){
		ip = (char*)argv[1];
		port = (char*)argv[2];
	}
	else{
		err_log("ip port input error\n");
	}

	if((client->sock=socket(AF_INET,SOCK_STREAM,0))<0)//创建流式套接字
	{
		err_log("fail to socket:");
	}
	//填充服务器的网络信息
	client->sockAddr.sin_family=AF_INET;
	client->sockAddr.sin_port=htons(atoi(port));
	client->sockAddr.sin_addr.s_addr=inet_addr(ip);

	if(connect(client->sock,(struct sockaddr*)&(client->sockAddr),len)<0)//请求链接服务器
	{
		err_log("fail to connect:\n");
	}

	printf("InitConn() success: ip<%s> port<%s>\n", ip, port);

	
#if 0
	while(1)
	{
		/*
		ret=recv(client->sock,client->buff_recv,sizeof(client->buff_recv), 0);
		printf("sent: %s\n",client->buff_recv);

		fgets(client->buff_send,sizeof(client->buff_send),stdin);
		client->buff_send[strlen(client->buff_send)-1]='\0';///\n---》\0
		send(client->sock, client->buff_send,sizeof(client->buff_send),0);
		//bzero(msg.buff,sizeof(msg.buff));
		ret=recv(client->sock,client->buff_recv,sizeof(client->buff_recv), 0);
		printf("sent: %s\n",client->buff_recv);
		*/
		bzero(client->buff_recv,sizeof(client->buff_recv));
		ret=recv(client->sock,client->buff_recv,sizeof(client->buff_recv), 0);
		client->buff_recv_len += ret;
		client_dispatch(client);
		sleep(3);
	}
#endif
	return ret;
}

int StartConnEpoll(MySocket_p client)
{
	int fdCount, ret;
	int epfd;
    struct epoll_event event;
    struct epoll_event events[1];
    //struct NET_MSG msg;
    
    epfd = epoll_create(1); //创建epoll的句柄
    
    event.data.ptr = (void*)client;
    event.events = EPOLLIN|EPOLLET;
    
    //注册epoll事件 epoll_ctl
    epoll_ctl (epfd, EPOLL_CTL_ADD, client->sock, &event);
    printf("Epoll客户端启动成功\n");
    bzero(client->buff_recv, sizeof(client->buff_recv));
    bzero(client->buff_send, sizeof(client->buff_send));
    client->buff_recv_len=0;
    client->buff_send_len=0;

    cmd_init_request(client);
    while(client->status>0)
    {
        //等待事件到来，返回事件的数量
        //printf("server epoll waiting\n");
        fdCount=epoll_wait(epfd, events, 1,100);
        if(fdCount<0)
        {
        	printf("error epoll_wait()<%d>\n", fdCount);
        	client->status=-1;
        }
        else if(fdCount==0)
        {

        }
        else
        {
            if(events[0].events & EPOLLIN)  //判断是数据到来
            {
                //memset(&client->buff_recv,0,sizeof(client->buff_recv));
                ret = read(client->sock, client->buff_recv, sizeof(client->buff_recv)); 
                //printf("recv bytes<%d>\n",ret);
                if(ret <= 0){      //连接关闭，清除关注的事件描述符事件表    
                    epoll_ctl(epfd,EPOLL_CTL_DEL,client->sock,&event); 
                    close(client->sock);
                    break;
                }else {
                	client->buff_recv_len += ret;
                    client_dispatch(client);

                }
            }
        }
        
    }

    close(epfd);
    //free_linklist(&clientlist);

	return 0;

}


int client_dispatch(MySocket_p client)
{
	if(NULL==client) return 0;

	
	while(client->buff_recv_len>0)
	{
		MSG msg;
		STRING2NETMSG(&msg, client->buff_recv);
		//printf("recv CMD <%d>\n", msg.buff[0]);
		//if(client->buff_recv_len<msg.datalength) break;

		switch(msg.buff[0])
		{
			case CMD_INIT:

				break;
			case CMD_INIT_RES:
				//printf("CMD: CMD_INIT_RES\n");
				cmd_init_option(client, &msg);
				
				break;
			case CMD_LOGIN_RES:
				//printf("CMD: CMD_LOGIN_RES\n");
				cmd_login_option(client, &msg);
				
				break;
			case CMD_LOGIN_RES_ADMIN:
				//printf("CMD: CMD_LOGIN_RES_ADMIN\n");
				cmd_login_option_admin(client, &msg);
				
				break;

			case CMD_UPD_PASSWORD_RES:
				cmd_updpassword_result(client, &msg);

				break;
			case CMD_SCH_SELF_RES:
				//printf("CMD: CMD_SCH_SELF_RES\n");
				cmd_schself_result(client, &msg);
				break;
			case CMD_ADD_USER_RES:
				//printf("CMD: CMD_ADD_USER_RES\n");
				cmd_adduser_result(client, &msg);
				break;
			case CMD_DEL_USER_RES_1:
				//printf("CMD: CMD_DEL_USER_RES_1\n");
				cmd_deluser_result1(client, &msg);
				break;
			case CMD_DEL_USER_RES_2:
				//printf("CMD: CMD_DEL_USER_RES_2\n");
				cmd_deluser_result2(client, &msg);
				break;
			case CMD_UPD_USER_RES_1:
				//printf("CMD: CMD_UPD_USER_RES_1\n");
				cmd_upduser_result1(client, &msg);
				break;
			case CMD_UPD_USER_RES_2:
				//printf("CMD: CMD_UPD_USER_RES_2\n");
				cmd_upduser_result2(client, &msg);

				break;
			case CMD_SCH_USER_RES:
				cmd_schuser_result(client, &msg);
				//printf("buff<%d>msg<%d>\n", client->buff_recv_len, msg.datalength);
				break;

			case CMD_SCH_LOG_RES:
				cmd_schlog_result(client, &msg);
				//printf("buff<%d>msg<%d>\n", client->buff_recv_len, msg.datalength);
				break;
			case CMD_ERROR:
				printf("服务器消息：%s\n", msg.buff+1);
				cmd_init_request(client);
				break;
			default:
				break;
		}
		

		if(client->buff_recv_len - msg.datalength >0)
		{
			memcpy(client->buff_recv, client->buff_recv+msg.datalength, client->buff_recv_len - msg.datalength);
		}
		client->buff_recv_len -= msg.datalength;

	}

	bzero(client->buff_recv, sizeof(client->buff_recv));
	client->buff_recv_len=0;

	return 0;

}


int cmd_init_request(MySocket_p client)
{
	int ret =0;

	ret = SEND_NETMSG(client, CMD_INIT, "");
	return ret;
}


int cmd_init_option(MySocket_p client, MSG *msg)
{
	int ret=0, flag=0;
	int option;

	while(1){
		printf("%s\n", msg->buff+1);
		scanf("%d",&option);
		//fgets(&option,sizeof(option),stdin);
		flag=1;
		switch(option){
			case 1:
				cmd_login_request(client);
				break;
			case 2:
				client->status=-1;
				break;
			default:
				printf("输入错误！\n");
				flag=0;
				break;
		}
		if(flag) break;
	}
	return ret;

}


int cmd_login_request(MySocket_p client)
{
	int ret =0;
	char buff[BUFFSIZE]={0};
	User user;

	printf("请输入用户名：");
	scanf("%s",user.username);
	printf("请输入密码：");
	scanf("%s",user.password);

	strcpy(client->username, user.username);
	USER2STRING(&user, buff);
	ret = SEND_NETMSG(client, CMD_LOGIN, buff);



	return ret;
}


int cmd_login_option(MySocket_p client, MSG *msg)
{
	int ret=0, flag=0;
	int option;

	while(1){
		printf("%s\n", msg->buff+1);
		scanf("%d",&option);
		flag=1;
		switch(option){
			case 1:
				cmd_updpassword_request(client);
				break;
			case 2:
				cmd_schself_request(client);
				break;
			case 3:
				cmd_schlog_request(client);
				break;
			case 4:
				cmd_init_request(client);
				break;
			default:
				printf("输入错误！\n");
				flag=0;
				break;
		}
		if(flag) break;
	}
	return ret;

}

int cmd_login_option_admin(MySocket_p client, MSG *msg)
{
	int ret=0, flag=0;
	int option;
	while(1){
		printf("%s\n", msg->buff+1);
		scanf("%d",&option);
		flag=1;
		switch(option){
			case 1:
				cmd_adduser_request(client);
				break;
			case 2:
				cmd_deluser_request(client);
				break;
			case 3:
				cmd_upduser_request(client);
				break;
			case 4:
				cmd_schuser_request(client);
				break;
			case 5:
				cmd_schlog_request(client);
				break;
			case 6:
				cmd_init_request(client);
				break;
			default:
				printf("输入错误！\n");
				flag=0;
				break;
		}
		if(flag) break;
	}
	return ret;

}

int cmd_updpassword_request(MySocket_p client)
{
	int ret=0;

	char buff[BUFFSIZE]={0};
	char password1[32]={0};
	char password2[32]={0};
	char yes='y';
	User user;
	strcpy(user.username, client->username);
	while(1){
		printf("请输入密码：");
		scanf("%s", password1);
		printf("请再次输入密码：");
		scanf("%s", password2);

		if(strcmp(password1, password2)!=0)
		{
			printf("密码输入不一致，是否重试(y/n)？：");
			scanf("%s", &yes);
			if(yes != 'y'){
				break;
			}
		}
		else
		{
			strcpy(user.password, password1);
			break;	
		}
	}

	if(yes=='y')
	{
		printf("是否确认修改密码(y/n)？：");
		scanf("%s", &yes);
		if(yes == 'y'){
			USER2STRING(&user, buff);
			ret=SEND_NETMSG(client, CMD_UPD_PASSWORD, buff);
		}
		else{
			printf("取消修改\n");
			ret=SEND_NETMSG(client, CMD_MENU, buff);
		}
	}
	else
	{
		printf("取消修改\n");
		ret=SEND_NETMSG(client, CMD_MENU, buff);
	}

	return ret;

}
int cmd_updpassword_result(MySocket_p client, MSG *msg)
{
	int ret=0;

	printf("服务器消息：%s\n", msg->buff+1);

	return ret;

}


int cmd_schself_request(MySocket_p client)
{
	int ret=0;

	char buff[BUFFSIZE]={0};
	User user;
	bzero(&user, sizeof(User));
	user.id=0;
	strcpy(user.username, client->username);

	USER2STRING(&user, buff);
	ret=SEND_NETMSG(client, CMD_SCH_SELF, buff);

	return ret;
}

int cmd_schself_result(MySocket_p client, MSG *msg)
{
	int ret=0;

	User user;
	STRING2USER(&user, msg->buff+1);
	printf_user(&user);

	return ret;
}





int cmd_adduser_request(MySocket_p client)
{
	int ret=0;

	char buff[BUFFSIZE]={0};
	char yes='y';
	User user;
	bzero(&user, sizeof(User));
	user.id=0;
	printf("请输入用户类型编号：(1.普通用户 2.管理员)");
	scanf("%d", &user.type);
	printf("请输入用户名：");
	scanf("%s", user.username);
	printf("请输入密码：");
	scanf("%s", user.password);
	printf("请输入用户年龄：");
	scanf("%d", &user.age);
	printf("请输入电话号码：");
	scanf("%s", user.phone);
	printf("请输入地址：");
	scanf("%s", user.addr);
	printf("请输入日期：");
	scanf("%s", user.date);
	printf("请输入级别：");
	scanf("%d", &user.level);
	printf("请输入工资：");
	scanf("%f", &user.salary);

	printf("是否确认添加(y/n)？：");
	scanf("%s", &yes);
	if(yes == 'y'){
		USER2STRING(&user, buff);
		ret=SEND_NETMSG(client, CMD_ADD_USER, buff);
	}
	else{
		printf("取消添加用户\n");
		ret=SEND_NETMSG(client, CMD_MENU, buff);
	}

	return ret;

}

int cmd_adduser_result(MySocket_p client, MSG *msg)
{
	int ret=0;

	printf("服务器消息：%s\n", msg->buff+1);

	return ret;

}

int cmd_deluser_request(MySocket_p client)
{
	int ret=0;

	char buff[BUFFSIZE]={0};
	User user;
	bzero(&user, sizeof(User));
	user.id=0;
	printf("请输入要删除的用户名：");
	scanf("%s", user.username);

	USER2STRING(&user, buff);
	ret=SEND_NETMSG(client, CMD_DEL_USER_1, buff);

	return ret;

}

int cmd_deluser_result1(MySocket_p client, MSG *msg)
{
	int ret=0;

	char buff[BUFFSIZE]={0};
	char yes='y';
	User user;
	STRING2USER(&user, msg->buff+1);
	printf_user(&user);
	
	printf("是否确认删除(y/n)？：");
	scanf("%s", &yes);
	if(yes == 'y'){
		USER2STRING(&user, buff);
		ret=SEND_NETMSG(client, CMD_DEL_USER_2, buff);
	}
	else{
		printf("取消删除\n");
		ret=SEND_NETMSG(client, CMD_MENU, buff);
	}

	return ret;

}

int cmd_deluser_result2(MySocket_p client, MSG *msg)
{
	int ret=0;

	printf("服务器消息：%s\n", msg->buff+1);

	return ret;

}


int cmd_upduser_request(MySocket_p client)
{
	int ret=0;

	char buff[BUFFSIZE]={0};
	User user;
	bzero(&user, sizeof(User));
	user.id=0;
	printf("请输入用户名：");
	scanf("%s", user.username);

	USER2STRING(&user, buff);
	ret=SEND_NETMSG(client, CMD_UPD_USER_1, buff);


	return ret;

}
int cmd_upduser_result1(MySocket_p client, MSG *msg)
{
	int ret=0;

	char buff[BUFFSIZE]={0};
	char yes='y';
	User user;
	STRING2USER(&user, msg->buff+1);
	printf_user(&user);
	printf("请输入修改信息:\n");
	//printf("请输入用户名：");
	//scanf("%s", user.username);
	printf("请输入用户类型编号：(1.普通用户 2.管理员)");
	scanf("%d", &user.type);
	printf("请输入密码：");
	scanf("%s", user.password);
	printf("请输入用户年龄：");
	scanf("%d", &user.age);
	printf("请输入电话号码：");
	scanf("%s", user.phone);
	printf("请输入地址：");
	scanf("%s", user.addr);
	printf("请输入日期：");
	scanf("%s", user.date);
	printf("请输入级别：");
	scanf("%d", &user.level);
	printf("请输入工资：");
	scanf("%f", &user.salary);
	printf("是否确认提交(y/n)？：");
	scanf("%s", &yes);
	if(yes == 'y'){
		USER2STRING(&user, buff);
		ret=SEND_NETMSG(client, CMD_UPD_USER_2, buff);
	}
	else{
		printf("取消提交\n");
		ret=SEND_NETMSG(client, CMD_MENU, buff);
	}

	return ret;

}
int cmd_upduser_result2(MySocket_p client, MSG *msg)
{
	int ret=0;

	printf("服务器消息：%s\n", msg->buff+1);

	return ret;
}

int cmd_schuser_request(MySocket_p client)
{
	int ret=0;
	ret=SEND_NETMSG(client, CMD_SCH_USER, "");

	return ret;

}

int cmd_schuser_result(MySocket_p client, MSG *msg)
{
	int ret=0;

	printf("%s\n", msg->buff+1);

	return ret;
}

int cmd_schlog_request(MySocket_p client)
{
	int ret=0;
	ret=SEND_NETMSG(client, CMD_SCH_LOG, "");

	return ret;

}
int cmd_schlog_result(MySocket_p client, MSG *msg)
{
	int ret=0;

	printf("%s\n", msg->buff+1);

	return ret;
}


void *epoll_thread(void *param)
{
	StartConnEpoll((MySocket_p)param);
	printf("Epoll线程退出……\n");

	return NULL;
}

int main(int argc, const char *argv[])
{
	int ret=0;
	pthread_t conn_thread;
	MySocket client;
	bzero(&client, sizeof(MySocket));
    client.status=1;
	ret=InitConn(&client, argc, argv);
	
	if(ret<0) err_log("服务器初始化失败\n");

	pthread_create(&conn_thread, NULL, epoll_thread, &client);

	while(client.status>0)
	{
		usleep(100000);
	}

	pthread_join(conn_thread, NULL);
	printf("客户端sock关闭<%d>……\n", client.sock);
	close(client.sock);

	return ret;
}


#endif //!__CLIENT_C__
#ifndef __SERVER_C__
#define __SERVER_C__

#include "./server.h"


int InitSocket(MySocket_p server, int argc, const char *argv[])
{
	int ret = 0;
	char *ip;
	char *port;

	if(server && argc>=3){
		ip = (char*)argv[1];
		port = (char*)argv[2];
	}
	else{
		printf("ip port input error\n");
		return -1;
	}


	if((server->sock = socket(AF_INET,SOCK_STREAM,0))<0){
		printf("Failed to create socket \n");
		return -1;
	}

	server->sockAddr.sin_family = AF_INET;    /* 网络类型*/
	server->sockAddr.sin_port = htons(atoi(port));  /* 端口注意字节序转换*/
	server->sockAddr.sin_addr.s_addr = inet_addr(ip);   /* IP地址要转换成32位网络地址*/

	int reuse;// 设置地址重用，克服连接断开需要等待time_wait的时间。
	if ( setsockopt(server->sock, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse)) < 0 ) {
		printf ("setsockopt SO_REUSEADDR failed \n");
		close (server->sock);
		return -1;
	};

	if((ret = bind(server->sock,(struct sockaddr *)&server->sockAddr,sizeof(struct sockaddr)))<0){
		printf("Failed to bind server addr \n");
		return ret;
	}

	if((ret = listen(server->sock,10))<0){
		printf("Failed to listen \n");
		return ret;
	}

	printf("InitSocket() success: ip<%s> port<%s>\n", ip, port);

	return ret;
}


int StartServerEpoll(MySocket_p server, sqlite3 *db, int *on)
{
/*	LinkNode_p clientlist=NULL;
	create_linklist(&clientlist);
	LinkNode_p pNode=NULL;
    init_node(&pNode);
    pNode->elem=(void*)server;
    insert_linklist(clientlist, pNode, 0);
*/
	int i, fdCount, ret;
	int clientFd;
	int epfd;
    struct epoll_event event;
    struct epoll_event events[MAXCLIENTS];
    struct sockaddr_in clientAdd;
    //struct NET_MSG msg;
    socklen_t len = sizeof(clientAdd);

    MySocket* client=NULL;
    
    epfd = epoll_create(1); //创建epoll的句柄
    
    event.data.ptr = (void*)server;
    event.events = EPOLLIN | EPOLLET;
    
    //注册epoll事件 epoll_ctl
    epoll_ctl (epfd, EPOLL_CTL_ADD, server->sock, &event);
    printf("Epoll服务器启动成功\n");
    while(*on)
    {
        //等待事件到来，返回事件的数量
        //printf("server epoll waiting\n");
        fdCount=epoll_wait(epfd,events,MAXCLIENTS,100);

        if(fdCount<0)
        {
        	printf("error epoll_wait()<%d>\n", fdCount);
        	*on=0;
        }
        else if(fdCount==0)
        {

        }
        else
        {
        	for(i = 0; i < fdCount; ++i)   //轮询等待到的事件
	        {
	            client=(MySocket*)events[i].data.ptr;
	            if(client->sock == server->sock)//判断是否是连接事件
	            {
	                clientFd = accept(client->sock,(struct sockaddr *)&clientAdd,&len);  

	                client=malloc(sizeof(MySocket));
	                client->sock=clientFd;
	                client->sockAddr=clientAdd;
	                client->buff_send_len=0;
	                client->buff_recv_len=0;
	                /*pNode=NULL;
	                init_node(&pNode);
	                pNode->elem=(void*)client;
	                insert_linklist(clientlist, pNode, 1);*/

	                event.data.ptr 	= (void*)client;
	                event.events	= EPOLLIN|EPOLLET;
	                epoll_ctl(epfd, EPOLL_CTL_ADD, clientFd, &event);

	            }
	            else if(events[i].events & EPOLLIN)  //判断是数据到来
	            {
	                //memset(&client->buff_recv,0,sizeof(client->buff_recv));
	                ret = read(client->sock, client->buff_recv, sizeof(client->buff_recv)); 
	                printf("recv bytes<%d>\n",ret);
	                if(ret <= 0){      //连接关闭，清除关注的事件描述符事件表    
	                    epoll_ctl(epfd,EPOLL_CTL_DEL,client->sock,&event); 
	                    close(client->sock);
	                    free(client);
	                    client=NULL;
	                    continue;
	                }else {
	                	client->buff_recv_len += ret;
	                    //printf("epoll receive:%s\n", client->buff_recv);
	                    server_dispatch(client, db);

	                    //send(events[i].data.fd,(char*)&msg,sizeof(msg)); 
	                }
	            }
	        }
        
        }
    }

    close(epfd);
    //free_linklist(&clientlist);

	return 0;
}


int server_dispatch(MySocket_p client, sqlite3 *db)
{
	if(NULL==client) return 0;

	
	while(client->buff_recv_len>0)
	{
		MSG msg;
		STRING2NETMSG(&msg, client->buff_recv);
		printf("recv CMD <%d>\n", msg.buff[0]);

		switch(msg.buff[0])
		{
			case CMD_INIT: 
				printf("CMD: CMD_INIT\n");
				cmd_init_respsonse(client);
				break;
			case CMD_LOGIN:
				printf("CMD: CMD_LOGIN\n");
				cmd_login_response(client, db, &msg);
				
				break;
			case CMD_MENU:
				printf("CMD: CMD_MENU\n");
				cmd_login_response_menu(client);
				
				break;

			/*
			CMD_UPD_PASSWORD,
	CMD_UPD_PASSWORD_RES,
	CMD_SCH_SELF,
	CMD_SCH_SELF_RES,
			*/
			case CMD_UPD_PASSWORD:
				cmd_updpassword_reponse(client, db, &msg);
				break;
			case CMD_SCH_SELF:
				cmd_schself_response(client, db, &msg);
				break;
			case CMD_ADD_USER:
				printf("CMD: CMD_ADD_USER\n");
				cmd_adduser_respsonse(client, db, &msg);
				
				break;
			case CMD_DEL_USER_1:
				printf("CMD: CMD_DEL_USER\n");
				cmd_deluser_respsonse1(client, db, &msg);
				break;
			case CMD_DEL_USER_2:
				printf("CMD: CMD_DEL_USER\n");
				cmd_deluser_respsonse2(client, db, &msg);
				break;
			case CMD_UPD_USER_1:
				printf("CMD: CMD_UPD_USER_1\n");
				cmd_upduser_respsonse1(client, db, &msg);
				break;
			case CMD_UPD_USER_2:
				printf("CMD: CMD_UPD_USER_2\n");
				cmd_upduser_respsonse2(client, db, &msg);
				break;
			case CMD_SCH_USER:
				printf("CMD: CMD_SCH_USER\n");
				cmd_schuser_respsonse(client, db);
				cmd_login_response_menu(client);
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



int cmd_init_respsonse(MySocket_p client)
{
	int ret=0;
	if(NULL == client) return ret;

	char buff[BUFFSIZE]={0};

	strcpy(buff, "***************员工管理系统***************\n");
	strcat(buff, "1.登录  2.退出\n");
	strcat(buff, "请输入选择：");

	ret = SEND_NETMSG(client, CMD_INIT_RES, buff);
	return ret;

}



int cmd_login_response(MySocket_p client, sqlite3 *db, MSG *msg)
{
	int ret =0;

	User user;
	//MSG msg;
	//STRING2NETMSG(&msg, client->buff_recv);
	STRING2USER(&user, msg->buff+1);

	ret=db_user_login(db, &user);
	if(ret>0){
		client->status=user.type;
		strcpy(client->username, user.username);
		cmd_login_response_menu(client);

		ret=0;
	}else{
		ret =-1;
		ret = SEND_NETMSG(client, CMD_ERROR, "登录失败");
	}
	

	return ret;
}


int cmd_login_response_menu(MySocket_p client)
{
	int ret;
	char buff[BUFFSIZE]={0};
	
	switch(client->status)
	{
		case 100:
			strcpy(buff, "*********************欢迎登录*********************\n");
			strcat(buff, "1.修改密码 2.查询信息 3.退出\n");
			strcat(buff, "请输入选择：");
			ret=SEND_NETMSG(client, CMD_LOGIN_RES, buff);

			break;
		case 200:
			strcpy(buff, "*********************系统管理*********************\n");
			strcat(buff, "1.添加用户 2.删除用户 3.修改用户 4.查询用户 5.退出\n");
			strcat(buff, "请输入选择：");
			ret=SEND_NETMSG(client, CMD_LOGIN_RES_ADMIN, buff);

			break;

		default:
			strcpy(buff, "登录失败\n");
			ret=SEND_NETMSG(client, CMD_ERROR, buff);
			break;

	}

	return ret;
}

int cmd_updpassword_reponse(MySocket_p client, sqlite3 *db, MSG *msg)
{

	int ret;
	User user1, user2;
	bzero(&user1, sizeof(User));
	bzero(&user2, sizeof(User));
	
	STRING2USER(&user1, msg->buff+1);
	STRING2USER(&user2, msg->buff+1);

	ret=db_user_get(db, &user1);
	char buff[BUFFSIZE]={0};
	if(ret>0)
	{
		strcpy(user1.password, user2.password);
		ret=db_user_upd(db, &user1);
		if(ret>0)
		{
			strcpy(buff, "修改密码成功！");
			ret=SEND_NETMSG(client, CMD_UPD_PASSWORD_RES, buff);
			ret=cmd_init_respsonse(client);
		}
		else
		{
			strcpy(buff, "修改密码失败！");
			ret=SEND_NETMSG(client, CMD_UPD_PASSWORD_RES, buff);
			ret=cmd_login_response_menu(client);
		}
	}
	else{
		strcpy(buff, "用户不存在！");
		ret=SEND_NETMSG(client, CMD_UPD_PASSWORD_RES, buff);
		ret=cmd_login_response_menu(client);
	} 

	
	return ret;

}

int cmd_schself_response(MySocket_p client, sqlite3 *db, MSG *msg)
{
	int ret=0;

	User user;
	bzero(&user, sizeof(User));
	
	STRING2USER(&user, msg->buff+1);

	ret=db_user_get(db, &user);
	char buff[BUFFSIZE]={0};
	if(ret>0){
		USER2STRING(&user, buff);
		ret=SEND_NETMSG(client, CMD_SCH_SELF_RES, buff);
		ret=cmd_login_response_menu(client);
	}
	else{
		strcpy(buff, "用户不存在！");
		ret=SEND_NETMSG(client, CMD_ERROR, buff);
	}


	return ret;

}


int cmd_adduser_respsonse(MySocket_p client, sqlite3 *db, MSG *msg)
{
	int ret=0;

	User user;
	bzero(&user, sizeof(User));

	//MSG msg;
	//STRING2NETMSG(&msg, client->buff_recv);
	
	STRING2USER(&user, msg->buff+1);

	ret=db_user_add(db, &user);
	
	char buff[32]={0};
	if(ret>0)
		strcpy(buff, "添加用户成功！");
	else
		strcpy(buff, "添加用户失败！");

	ret=SEND_NETMSG(client, CMD_ADD_USER_RES, buff);
	ret=cmd_login_response_menu(client);
	return ret;

}


int cmd_deluser_respsonse1(MySocket_p client, sqlite3 *db, MSG *msg)
{
	int ret=0;

	User user;
	bzero(&user, sizeof(User));
	
	STRING2USER(&user, msg->buff+1);

	ret=db_user_get(db, &user);
	char buff[BUFFSIZE]={0};
	if(ret>0){
		USER2STRING(&user, buff);
		ret=SEND_NETMSG(client, CMD_DEL_USER_RES_1, buff);
	}
	else{
		strcpy(buff, "用户不存在！");
		ret=SEND_NETMSG(client, CMD_DEL_USER_RES_2, buff);
		ret=cmd_login_response_menu(client);
	}


	return ret;

}

int cmd_deluser_respsonse2(MySocket_p client, sqlite3 *db, MSG *msg)
{
	int ret=0;

	User user;
	bzero(&user, sizeof(User));
	
	STRING2USER(&user, msg->buff+1);

	ret=db_user_del(db, &user);
	
	char buff[32]={0};
	if(ret>0)
		strcpy(buff, "删除用户成功！");
	else
		strcpy(buff, "删除用户失败！");

	ret=SEND_NETMSG(client, CMD_DEL_USER_RES_2, buff);

	ret=cmd_login_response_menu(client);
	return ret;

}

int cmd_upduser_respsonse1(MySocket_p client, sqlite3 *db, MSG *msg)
{
	int ret=0;

	User user;
	bzero(&user, sizeof(User));
	
	STRING2USER(&user, msg->buff+1);

	ret=db_user_get(db, &user);
	char buff[BUFFSIZE]={0};
	if(ret>0){
		USER2STRING(&user, buff);
		ret=SEND_NETMSG(client, CMD_UPD_USER_RES_1, buff);
	}
	else{
		strcpy(buff, "用户不存在！");
		ret=SEND_NETMSG(client, CMD_UPD_USER_RES_2, buff);
		ret=cmd_login_response_menu(client);
	}
	

	return ret;

}
int cmd_upduser_respsonse2(MySocket_p client, sqlite3 *db, MSG *msg)
{
	int ret=0;

	User user;
	bzero(&user, sizeof(User));
	
	STRING2USER(&user, msg->buff+1);

	ret=db_user_upd(db, &user);
	
	char buff[32]={0};
	if(ret>0)
		strcpy(buff, "修改用户成功！");
	else
		strcpy(buff, "修改用户失败！");

	ret=SEND_NETMSG(client, CMD_UPD_USER_RES_2, buff);
	ret=cmd_login_response_menu(client);

	return ret;

}

int cmd_schuser_respsonse(MySocket_p client, sqlite3 *db)
{
	int ret=0;
	
	LinkList_p pList=NULL;
	linklist_create(&pList);

	db_user_all(db, pList);
	char *data=NULL;
	while(linklist_length(pList)>0)
	{
		linklist_pop(pList, (datatype*)&data);
		//printf("db_line: %s\n", data);
		SEND_NETMSG(client, CMD_SCH_USER_RES, data);
		if(data) free(data);
		data=NULL;
	}
	linklist_destroy(&pList);

	return ret;

}




void *epoll_thread(void *param)
{
	svrPARAM_p p=(svrPARAM_p)param;

	StartServerEpoll(p->server, p->db, p->on);
	printf("Epoll线程退出……\n");

	return NULL;
}




int main(int argc, const char *argv[])
{
	int ret=0;
	int bServerOn=1;
	
	pthread_t svr_thread;
	MySocket server;
	server.status=2;
	sqlite3 *db=NULL;
	db_open(&db);
	
	ret=InitSocket(&server, argc, argv);
	if(ret<0) err_log("服务器初始化失败\n");

	svrPARAM param={&server, db, &bServerOn};
	pthread_create(&svr_thread, NULL, epoll_thread, &param);

	while(bServerOn)
	{
		char cmd[32]={0};
		scanf("%s",cmd);
		if(strcmp(cmd, "exit")==0)
		{
			bServerOn=0;
		}
		if(strcmp(cmd, "quit")==0)
		{
			bServerOn=0;
		}
	}

	pthread_join(svr_thread, NULL);
	close(server.sock);
	printf("服务器sock关闭……\n");
	db_close(&db);
	printf("服务器数据库关闭……\n");
	return ret;
}


#endif //!__SERVER_C__
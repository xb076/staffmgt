#include "db.h"


int db_open(sqlite3 **db)
{
	int ret=0;
	if(sqlite3_open(DB_PATH, db)!=SQLITE_OK)//打开数据库
	{
		printf("%s\n",sqlite3_errmsg(*db));
		ret = -1;
	}
	return ret;
}
int db_close(sqlite3 **db)
{
	int ret=0;
	if(sqlite3_close(*db)==SQLITE_BUSY)
	{
		printf("%s\n",sqlite3_errmsg(*db));
		ret =-1;
	}
	return ret;
}

int convert2user(char **rep, int n_row, int n_column, User *user)
{
	int ret=-1;
	if(NULL==user || NULL==rep || NULL== *rep) return ret;
	ret=0;

	user->id=atoi(*(rep+n_column));
	user->type=atoi(*(rep+n_column+1));
	user->age=atoi(*(rep+n_column+4));
	strcpy(user->phone, *(rep+n_column+5));
	strcpy(user->addr, *(rep+n_column+6));
	strcpy(user->date, *(rep+n_column+7));
	user->level=atoi(*(rep+n_column+8));
	user->salary=atof(*(rep+n_column+9));

	return ret;
}


int db_user_login(sqlite3 *db, User *user)
{
	int ret=-1;

	char sql[256]={0};
	char *errmsg;
	char **rep;
	int n_row;
	int n_column;

	sprintf(sql,"select * from userinfo where username='%s' and password='%s'",user->username,user->password);
	if(sqlite3_get_table(db,sql,&rep,&n_row,&n_column,&errmsg)!=SQLITE_OK)
	{
		printf("%s\n",errmsg);
		ret=-1;
	}
	else
	{
		if(n_row==0)//查不到
		{
			ret=0;
			printf("db_user_login no find\n");
			
		}
		else  //只要行数大于0，无需打印，直接返回成功
		{
			ret = n_row;
			convert2user(rep, n_row, n_column, user);
		}
	}

	return ret;


}

int db_user_add(sqlite3 *db, User *user)
{
	int ret=1;
	char date[64]={0};

	char sql[256]={0};
	char *errmsg;
	char **rep;
	int n_row;
	int n_column;
	sprintf(sql, "SELECT MAX(id) FROM userinfo");
	if(sqlite3_get_table(db,sql,&rep,&n_row,&n_column,&errmsg)!=SQLITE_OK)
	{
		printf("sql<%s>\n<%s>\n", sql, errmsg);
		ret=-1;
	}
	else
	{
		if(n_row==0)//查不到
		{
			ret=0;
			printf("<%s>\nno find\n", sql);
			
		}
		else
		{
			/*
			int i,j;
			ret=n_row;
			
			for(i=0;i<n_row+1;++i)
			{
				for(j=0;j<n_column;++j)	
				{
					printf("row<%d>col<%d>txt<%s>\n", i, j, *rep++);
				}
			}
			*/
			user->id=atoi(*(rep+1))+1;
		}
	}

	get_today(date);//获得当前的日期
	bzero(sql, sizeof(sql));
	sprintf(sql,"insert into userinfo values(%d,%d,'%s','%s', %d, '%s', '%s', '%s',%d,%.2f)",
			user->id,
			user->type,
			user->username,
			user->password,
			user->age,
			user->phone,
			user->addr,
			date,
			user->level,
			user->salary);
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("error: <%s>\n<%s>\n",sql, errmsg);
		ret = -1;
	}


	return ret;
}
int db_user_del(sqlite3 *db, User *user)
{
	int ret=1;

	char sql[256]={0};
	char *errmsg;
	sprintf(sql,"delete from userinfo where username='%s'", user->username);
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("error: <%s>\n<%s>\n",sql, errmsg);
		ret = -1;
	}


	return ret;
}
int db_user_upd(sqlite3 *db, User *user)
{
	int ret=1;

	char sql[256]={0};
	char *errmsg;
	sprintf(sql,"update userinfo set type=%d,password='%s', age=%d, phone='%s', addr='%s', date='%s',level=%d,salary=%.2f where username='%s'",
			user->type,
			user->password,
			user->age,
			user->phone,
			user->addr,
			user->date,
			user->level,
			user->salary,
			user->username);
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("error: <%s>\n<%s>\n",sql, errmsg);
		ret = -1;
	}

	return ret;
}

int db_user_get(sqlite3 *db, User *user)
{
	int ret=0;
	char sql[256]={0};
	char *errmsg;
	char **rep;
	int n_row;
	int n_column;
	sprintf(sql, "SELECT * FROM userinfo WHERE username='%s'", user->username);
	if(sqlite3_get_table(db,sql,&rep,&n_row,&n_column,&errmsg)!=SQLITE_OK)
	{
		printf("sql<%s>\n<%s>\n", sql, errmsg);
		ret=-1;
	}
	else
	{
		if(n_row==0)//查不到
		{
			ret=0;
			printf("<%s>\nno find\n", sql);
			
		}
		else
		{
			ret=n_row;
			convert2user(rep, n_row, n_column, user);
		}
	}



	return ret;
}
int db_user_all(sqlite3 *db, LinkList_p userlist)
{
	int ret=0;

	char sql[256]={0};
	char *errmsg;
	char **rep;
	int n_row;
	int n_column;

	sprintf(sql,"select * from userinfo");
	if(sqlite3_get_table(db,sql,&rep,&n_row,&n_column,&errmsg)!=SQLITE_OK)
	{
		printf("sql<%s>\n<%s>\n", sql, errmsg);
		ret=-1;
	}
	else
	{
		if(n_row==0)//查不到
		{
			ret=0;
			printf("<%s>\nno find\n", sql);
			
		}
		else  //只要行数大于0，无需打印，直接返回成功
		{
			int i,j;
			ret=n_row;
			
			for(i=0;i<n_row+1;++i)
			{
				char *line=(char*)malloc(BUFFSIZE);
				bzero(line, BUFFSIZE);
				for(j=0;j<n_column;++j)	
				{
					strcat(line, *rep++);
					strcat(line, "|");
				}

				linklist_push(userlist, (datatype)line);
			}
		}
	}

	return ret;

}






int db_log_add(sqlite3 *db, Log *log)
{
	int ret=0;

	return ret;
}
int db_log_del(sqlite3 *db, Log *log)
{
	int ret=0;

	return ret;
}
int db_log_upd(sqlite3 *db, Log *log)
{
	int ret=0;

	return ret;
}
int db_log_get(sqlite3 *db, Log *log)
{
	int ret=0;

	return ret;
}
int db_log_all(sqlite3 *db, LinkList_p loglist)
{
	int ret=0;

	return ret;
}
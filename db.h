#ifndef __DB_H__
#define __DB_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sqlite3.h>

#define DB_PATH "./staff.db"

#include "common.h"


int db_open(sqlite3 **db);
int db_close(sqlite3 **db);

int convert2user(char **rep, int n_row, int n_column, User *user);

int db_user_login(sqlite3 *db, User *user);
int db_user_add(sqlite3 *db, User *user);
int db_user_del(sqlite3 *db, User *user);
int db_user_upd(sqlite3 *db, User *user);
int db_user_get(sqlite3 *db, User *user);
int db_user_all(sqlite3 *db, LinkList_p userlist);


int db_log_add(sqlite3 *db, Log *log);
int db_log_del(sqlite3 *db, Log *log);
int db_log_upd(sqlite3 *db, Log *log);
int db_log_get(sqlite3 *db, Log *log);
int db_log_all(sqlite3 *db, LinkList_p loglist);


#endif //!__DB_H__
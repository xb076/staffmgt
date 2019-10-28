#目标：依赖
#	规则    注意：规则前是一个tab键

CC=gcc
CFLAGS=-c -Wall

all:server.o client.o linklistdual.o common.o db.o
	$(CC) server.o linklistdual.o common.o db.o -o server -l sqlite3 -lpthread
	$(CC) client.o linklistdual.o common.o -o client -l sqlite3 -lpthread

%*.o:%*.c
	$(CC) $(CFLAGS) $< -o $@


.PHONY:clean
clean:
	rm *.o server client core

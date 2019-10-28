#ifndef __LINKLISTDUAL_H__
#define __LINKLISTDUAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//双向循环链表

typedef void* datatype;

typedef struct MyNode
{
	datatype data;
	struct MyNode *prev;
	struct MyNode *next;
}DNode, * DNode_p;

typedef struct MyList
{
	DNode_p pHead;
	int length;
}LinkList, * LinkList_p;

int linklist_init_node(DNode_p *ppNode);
int linklist_free_node(DNode_p *ppNode);

int linklist_create(LinkList_p *ppList);
int linklist_push(const LinkList_p pList, datatype val);
int linklist_pop(const LinkList_p pList, datatype *val);
int linklist_insert(const LinkList_p pList, DNode_p pNode, int pos);
int linklist_delete(const LinkList_p pList, datatype val);
int linklist_delete_pos(const LinkList_p pList, int pos);
int linklist_update(const LinkList_p pList, DNode_p pNode, int pos);

int linklist_getnode(const LinkList_p pList, int pos, DNode_p *ppNode);
int linklist_locate(const LinkList_p pList, datatype val, DNode_p *ppNode);
int linklist_locate_r(const LinkList_p pList, datatype val, DNode_p *ppNode);
int linklist_length(const LinkList_p pList);

void linklist_shownode(const DNode_p pNode);
void linklist_showall(const LinkList_p pList);
int linklist_destroy(LinkList_p *ppList);






#endif

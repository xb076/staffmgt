#ifndef __LINKLISTSINGLE_H__
#define __LINKLISTSINGLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void* datatype;

typedef struct MyNode
{
	datatype elem;
	struct MyNode *next;
}LinkNode, * LinkNode_p;

int init_node(LinkNode_p *ppNode);
int free_node(LinkNode_p *ppNode);

int create_linklist(LinkNode_p *ppHead);
int insert_linklist(const LinkNode_p pHead, LinkNode_p p, int pos);
int delete_linklist(const LinkNode_p pHead, int pos);
int update_linklist(const LinkNode_p pHead, LinkNode_p p, int pos);
int getnode_linklist(const LinkNode_p pHead, LinkNode_p *ppNode, int pos);
int locate_linklist(const LinkNode_p pHead, LinkNode_p *pNode, datatype val);

int length_linklist(const LinkNode_p pHead);
void printnode_linklist(const LinkNode_p pNode);
void show_linklist(const LinkNode_p pHead);
int free_linklist(LinkNode_p *ppHead);

int reverse_linklist(const LinkNode_p pHead);

void sort_linklist(const LinkNode_p pHead);

int find_reverse_K_linklist(const LinkNode_p pHead, int rK, datatype *val_p);


#endif

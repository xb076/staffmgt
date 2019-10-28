#include "linklistdual.h"



int linklist_init_node(DNode_p *ppNode)
{
	DNode_p p=(DNode_p)malloc(sizeof(DNode));
	if(NULL==p) return -1;
	
	memset(&(p->data),0,sizeof(datatype));
	p->prev=NULL;
	p->next=NULL;
	*ppNode=p;

	return 0;
}
int linklist_free_node(DNode_p *ppNode)
{
	if(NULL==ppNode||NULL==*ppNode) return -1;

	free(*ppNode);
	*ppNode=NULL;

	return 0;
}

int linklist_create(LinkList_p *ppList)
{
	*ppList=(LinkList_p)malloc(sizeof(LinkList));
	if(NULL==ppList) return -1;

	DNode_p p=NULL;
	if(linklist_init_node(&p)!=0) {
		free(*ppList);
		return -1;
	}
	if(NULL!=p)
	{
		p->prev=p;
		p->next=p;
		(*ppList)->pHead=p;
		(*ppList)->length=0;
	}
	return 0;
}
int linklist_push(const LinkList_p pList, datatype val)
{
	if(NULL==pList) return -1;
	int ret=0;

	DNode_p pNode=NULL;
	linklist_init_node(&pNode);
	if(pNode) {
		pNode->data=val;
		ret = linklist_insert(pList, pNode, pList->length);
	}
	else 
		ret = -1;


	return ret;
}
int linklist_pop(const LinkList_p pList, datatype *val)
{
	if(NULL==pList) return -1;
	int ret=0;

	if(NULL!=val){
		DNode_p pNode=NULL;
		linklist_getnode(pList, 0, &pNode);
		*val=pNode->data;
		pNode->data=0;
	}
	ret=linklist_delete_pos(pList, 0);

	return ret;
}

int linklist_insert(const LinkList_p pList, DNode_p pNode, int pos)
{
	if(NULL==pList || NULL==pNode) return -1;
	
	DNode_p p=pList->pHead;//注意这个是目标节点的前一个节点

	int count=0;
	if(pos < pList->length)
	{
		do{
			if(count==pos)
			{
				break;
			}
			p=p->next;
			++count;
		}while(p != pList->pHead);
	}
	else{
		p = pList->pHead->prev;
	}

	if(NULL!=p)
	{
		pNode->next=p->next;
		pNode->prev=p;
		p->next->prev=pNode;
		p->next=pNode;
		pList->length+=1;
	}

	return 0;
}
int linklist_delete(const LinkList_p pList, datatype val)
{
	if(NULL==pList) return -1;

	DNode_p p=NULL;

	linklist_locate(pList, val, &p);

	if(NULL!=p)
	{
		p->prev->next=p->next;
		p->next->prev=p->prev;
		linklist_free_node(&p);
		pList->length-=1;
	}

	return 0;

}
int linklist_delete_pos(const LinkList_p pList, int pos)
{
	if(NULL==pList) return -1;

	DNode_p p=NULL;

	linklist_getnode(pList, pos, &p);

	if(NULL!=p)
	{
		p->prev->next=p->next;
		p->next->prev=p->prev;
		linklist_free_node(&p);
		pList->length-=1;
	}

	return 0;
}
int linklist_update(const LinkList_p pList, DNode_p pNode, int pos)
{
	if(NULL==pList||NULL==pNode) return -1;
	DNode_p p=NULL;
	linklist_getnode(pList, pos, &p);
	if(NULL!=p)
	{
		pNode->prev=p->prev;
		pNode->next=p->next;
		p->prev->next=pNode;
		p->next->prev=pNode;
		linklist_free_node(&p);
	}


	return 0;
}

int linklist_getnode(const LinkList_p pList, int pos, DNode_p *ppNode)
{
	if(NULL==pList || pos<-1) return -1;

	*ppNode=NULL;
	int count=0;
	DNode_p p=pList->pHead->next;
	while(p != pList->pHead)
	{
		if(count==pos)
		{
			*ppNode=p;
			break;
		}
		p=p->next;
		++count;
	}
	
	return 0;
}
int linklist_locate(const LinkList_p pList, datatype val, DNode_p *ppNode)
{
	if(NULL==pList) return -1;

	*ppNode=NULL;
	DNode_p p=pList->pHead->next;
	while(p!=pList->pHead)
	{
		if(p->data==val)
		{
			*ppNode=p;
			break;
		}
		p=p->next;
	}

	return 0;
}

int linklist_locate_r(const LinkList_p pList, datatype val, DNode_p *ppNode)
{
	if(NULL==pList) return -1;

	*ppNode=NULL;
	DNode_p p=pList->pHead->prev;
	while(p!=pList->pHead)
	{
		if(p->data==val)
		{
			*ppNode=p;
			break;
		}
		p=p->prev;
	}

	return 0;

}

int linklist_length(const LinkList_p pList)
{
	if(NULL==pList) return -1;

	return pList->length;

}

void linklist_shownode(const DNode_p pNode)
{
	if(NULL==pNode) return;

	printf("%d\t", (int)pNode->data);
	printf("%p\n", pNode);
	printf("----------------\n");

}

void linklist_showall(const LinkList_p pList)
{
	if(NULL==pList) return;

	DNode_p p=pList->pHead;
	do
	{
		printf("%d\t", (int)p->data);
		printf("%p\n", p);

		p=p->next;
	}while(pList->pHead!=p);
	printf("----------------\n");

}
int linklist_destroy(LinkList_p *ppList)
{
	if(NULL==ppList || NULL==*ppList) return -1;
	
	DNode_p p=(*ppList)->pHead->next;
	DNode_p pPrev=(*ppList)->pHead;
	while((*ppList)->pHead!=p)
	{
		linklist_free_node(&pPrev);
		pPrev=p;
		p=p->next;
	}
	free(*ppList);

	*ppList=NULL;

	return 0;
}

#include "linklistsingle.h"

int init_node(LinkNode_p *ppNode)
{
	int res=-1;
	*ppNode=(LinkNode_p)malloc(sizeof(LinkNode));
	if(NULL!=*ppNode)
	{
		memset(&((*ppNode)->elem),0,sizeof(datatype));
		(*ppNode)->next=NULL;
		res=0;
	}
	else
	{
		printf("init_node failed\n");
		res=1;
	}
	return res;
}

int free_node(LinkNode_p *ppNode)
{
	int res=-1;
	if(NULL==ppNode || NULL==*ppNode) return res;
	printf("free %p\n", *ppNode);
	free(*ppNode);
	*ppNode=NULL;
	res=0;
	return res;
}


int create_linklist(LinkNode_p *ppHead)
{
	int res=-1;

	if(init_node(ppHead)==0) res=0;
	
	if(NULL==*ppHead)
	{
		printf("create_linklist error\n");
		res=-1;
	}
	return res;
}

int insert_linklist(const LinkNode_p pHead, LinkNode_p p, int pos)
{
	int res=-1;
	if(NULL==pHead || NULL==p) return res;
	if(pos<0) return res;

	LinkNode_p pTarget=NULL;
	if(getnode_linklist(pHead, &pTarget, pos-1)==0)
	{
		if(NULL!=pTarget)
		{
			p->next=pTarget->next;
			pTarget->next=p;
			res=0;
		}

	}


	/*LinkNode_p pTarget=*ppHead;
	int count=0;

	while(NULL!=pTarget)
	{
		if(count==pos || NULL==pTarget->next)
		{
			p->next=pTarget->next;
			pTarget->next=p;
			res=0;
			break;
		}
		++count;
		pTarget=pTarget->next;
	}*/
	return res;
}

int delete_linklist(const LinkNode_p pHead, int pos)
{
	int res=-1;
	if(NULL==pHead || pos<=0 ) return res;
	
	LinkNode_p p=NULL;
	if(getnode_linklist(pHead, &p, pos-1)!=0) {return res;}

	if(NULL==p) return res;
	
	LinkNode_p pNext=p->next;
	if(NULL==pNext) return res;
	
	p->next=pNext->next;
	free_node(&pNext);
	res=0;
	

	return res;
}

int update_linklist(const LinkNode_p pHead, LinkNode_p p, int pos)
{
	int res=-1;
	if(NULL==pHead || NULL==p || pos<=0) return res;

	LinkNode_p pNode=NULL;
	if(getnode_linklist(pHead, &pNode, pos-1)!=0){ return res;}

	if(NULL==pNode) return res;

	LinkNode_p pNext=pNode->next;
	if(NULL==pNext) return res;

	p->next=pNext->next;
	pNode->next=p;
	free_node(&pNext);
	res=0;

	return res;
}


int getnode_linklist(const LinkNode_p pHead, LinkNode_p *ppNode, int pos)
{
	int res=-1;
	if(NULL==pHead) return res;
	if(pos<-1) return res;

	LinkNode_p pTarget=pHead;
	*ppNode=NULL;
	int count=-1;
	while(NULL!=pTarget)
	{
		if(count==pos)
		{
			*ppNode=pTarget;
			res=0;
			break;
		}
		pTarget=pTarget->next;
		++count;
	}

	return res;
}

int locate_linklist(const LinkNode_p pHead, LinkNode_p *pNode, datatype val)
{
	int res=-1;
	if(NULL==pHead) return res;

	LinkNode_p p=pHead->next;

	while(NULL!=p && p->elem!=val)
	{
		p=p->next;
	}
	if(NULL==p) 
	{
		*pNode=NULL;
		res=-1;
	}
	else 
	{
		*pNode=p;
		res=0;
	}
	return res;
}

int length_linklist(const LinkNode_p pHead)
{
	if(NULL==pHead) return -1;
	int len=0;

	LinkNode_p p=pHead->next;
	while(NULL!=p)
	{
		++len;
		p=p->next;
	}
	return len;
}

void printnode_linklist(const LinkNode_p pNode)
{
	if(NULL==pNode) return;
	printf("%d\t", (int)pNode->elem);
	printf("%p\n", pNode);
	printf("----------------\n");
}


void show_linklist(const LinkNode_p pHead)
{
	LinkNode_p p;
	p=pHead;
	while(NULL!=p)
	{
		printf("%d\t", (int)p->elem);
		printf("%p\n", p);
		p=p->next;
	}
	printf("----------------\n");
}

int free_linklist(LinkNode_p *ppHead)
{
	int res=-1;
	if(NULL==ppHead || NULL==*ppHead) return res;

	LinkNode_p p1,p2;
	p1=p2=*ppHead;
	res=0;
	while(NULL!=p1)
	{
		p2=p1;
		p1=p1->next;
		p2->next=NULL;
		if(free_node(&p2)==0) 
		{
			p2=NULL;
		}
		else 
		{
			printf("free node error");
			res=-1;
			break;
		}
	}
	if(res==0) *ppHead=NULL;
	//if(NULL!=*ppHead) free(*ppHead);
	return res;
}

int reverse_linklist(const LinkNode_p pHead)
{
	int res=-1;

	if(NULL==pHead) return res;

	LinkNode_p p1=pHead->next;
	LinkNode_p p2=p1;
	pHead->next=NULL;

	while(NULL!=p2)
	{
		p1=p1->next;
		if(insert_linklist(pHead, p2, 0)==0)
		{
			p2=p1;
		}
		else
		{
			res=-1;
			break;
		}
	}

	
	/*

	LinkNode_p p1=pHead->next;
	LinkNode_p p2=NULL;
	LinkNode_p p3=pHead;
	LinkNode_p p2Prev=NULL;

	int count=0;
	while(NULL!=p1)
	{
		p2=p1;
		p2Prev=p1;
		count=0;
		while(NULL!=p2)
		{
			if(NULL==p2->next)
			{
				p3->next=p2;
				p3=p3->next;
				if(NULL!=p2Prev) p2Prev->next=NULL;
				break;
			}
			p2Prev=p2;
			p2=p2->next;
			++count;		
		}
		if(p2==p2Prev) break;
	}

	res=0;*/

	return res;
}


void sort_linklist(const LinkNode_p pHead)
{
	if(NULL==pHead) return;	

	//这个方法是把头节点单拿出来，和后续斩断链接，在剩下节点中依次选出插回头结点
	LinkNode_p p1=pHead->next;
	if(NULL==p1) return; 
	LinkNode_p p2=p1;
	p1=p1->next;
	LinkNode_p p3=p2;

	LinkNode_p pTarget=p2;
	LinkNode_p pPrev=p2;
	pHead->next=NULL;

	while(NULL!=p3->next)
	{
		while(NULL!=p1)
		{
			if(pTarget->elem<p1->elem)
			{
				pPrev=p2;
				pTarget=p1;
			}
			p2=p1;
			p1=p1->next;
		}
		if(pTarget!=pPrev)
		{
			pPrev->next=pTarget->next;
			pTarget->next=NULL;
		}
		else
		{
			p3=p3->next;
			pTarget->next=NULL;
		}
		if(insert_linklist(pHead, pTarget,0)==0) 
		{
			p2=p3;
			p1=p2->next;
			pTarget=p2;
			pPrev=p2;
		}
	}

	if(NULL!=p3)
	{
		p3->next=NULL;
		if(insert_linklist(pHead, p3, 0)==0) {}
	}
	
	/*
	//这个方法是依次遍历找到最大或最小节点，再用头插法
	LinkNode_p p1=pHead->next;
	LinkNode_p p2=pHead;
	LinkNode_p p3=pHead;
	LinkNode_p pTarget=p1;
	LinkNode_p pPrev=pHead;

	//int len=length_linklist(pHead)-1;
	//int n=pTarget->elem;
	int n=0;
	while(NULL!=p3->next)
	xt;
		}
		pPrev->next=pTarget->next;
		insert_linklist(pHead, pTarget, 1);
		if(n==0)
			{p3=p3->next;n=-1;}
		p2=p3;
		p1=p3->next;
		pTarget=p1;
		pPrev=p2;

	}	
	*/
}

int find_reverse_K_linklist(const LinkNode_p pHead, int rK, datatype *val_p)
{
	int res=-1;
	if(NULL==pHead || NULL==val_p) return res;
	
	LinkNode_p pNode=pHead->next;
	LinkNode_p pNodeK=pNode;

	int len=0;
	while(NULL!=pNode)
	{
		++len;
		pNode=pNode->next;
		if(rK==len) break;
	}

	while(NULL!=pNode)
	{
		pNode=pNode->next;
		pNodeK=pNodeK->next;
	}

	if(NULL!=val_p) {*val_p=pNodeK->elem;res=0;}

/*	if(len==0 || rK>len || rK<=0 ) return res;

	int target=len-rK;

	if(getnode_linklist(pHead, &pNode, target)==0)
	{
		if(NULL!=pNode) *val_p=pNode->elem;
		res=0;
	}
*/
	return res;

}

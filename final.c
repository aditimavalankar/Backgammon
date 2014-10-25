#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node
{
	int pos1;
	int pos2;
	int evaluation;
	int boardstate[24];
	struct node *next[15];
}node;
node *head1=NULL,*head2=NULL;
int board[24];
int r1,r2,bar;
int r1flag=0;
int r2flag=0;
int sequence;
int save1,save2;
int ind;
int opr1,opr2;
int saveop1=23,saveop2=23;
int saver1,saver2;
int savesave1,savesave2;
int home=6;
int lone;
int blot;
int distanceweight=8;
int blotweight=1;
int loneweight=3;
int min(node *n)
{
	if(n==NULL)
		return 1000;
	int i,minimum=1000;
	for(i=0;i<15;i++)
	{
		if(n->next[i]==NULL)
			break;
		if((n->next[i])->evaluation<minimum)
			minimum=(n->next[i])->evaluation;
	}
	return minimum;
}
int evaluate(node *n,node *parent)
{
	int dist_from_home=n->pos2-home;
	int i;
	lone=0;
	blot=0;
	for(i=0;i<24;i++)
	{
		if(n->boardstate[i]==1)
			lone++;
		if(parent->boardstate[i]==-1)
			blot++;
	}
	n->evaluation=distanceweight*dist_from_home+loneweight*lone+blotweight*blot;
}
void print(node *n)
{
	if(n==NULL)
		return;
	if(n->pos1==-1)
		printf("Z ");
	else
		printf("%d ",n->pos1+1);
	if(n->pos2>=24)
		printf("O\n");
	else
		printf("%d\n",n->pos2+1);
	return;
}
node* formopponenttree(node *,node *,int);
node* form(node *,node *,int);
node* formtree(node *n,node *parent,int dicethrow)
{
	int i,j;
	if(n==NULL)
	{
		if(dicethrow==r1)
			ind=save1;
		else
			ind=save2;
		while(ind<24 && parent->boardstate[ind]<=0)
			ind++;
		if(ind==24)
		{
			if(dicethrow==r1)
				save1=ind;
			else
				save2=ind;
			return NULL;
		}
		while(1)
		{
			while(ind<24 && parent->boardstate[ind]<=0)
				ind++;
			if(ind+dicethrow<24 && parent->boardstate[ind+dicethrow]>=-1)
			{
				n=(node *)malloc(sizeof(node));
				n->pos1=ind;
				n->pos2=ind+dicethrow;
				for(i=0;i<24;i++)
					n->boardstate[i]=parent->boardstate[i];
				n->boardstate[ind]--;
				if(n->boardstate[ind+dicethrow]>=0)
				{
					n->boardstate[ind+dicethrow]++;
				}
				else
				{
					n->boardstate[ind+dicethrow]=1;
				}
				evaluate(n,parent);
	//			printf("moving from pos %d to pos %d\n",n->pos1,n->pos2);
				for(i=0;i<15;i++)
					n->next[i]=NULL;
				ind++;
				if(dicethrow==r1)
					save1=ind;
				else
					save2=ind;
				return n;
			}
			if(ind==24)
			{
				if(dicethrow==r1)
					save1=ind;
				else
					save2=ind;
				return NULL;
			}
			ind++;
		}
		return NULL;
	}
	int k;
	if(dicethrow==r1)
	{
		for(i=0;i<15;i++)
		{
			n->next[i]=formtree(n->next[i],n,r1);
			if(sequence==1 && n->next[i]!=NULL && r2)
			{
				for(j=0;j<15;j++)
				{
					(n->next[i])->next[j]=formtree((n->next[i])->next[j],n->next[i],r2);
							if(save2==24)
					{
						save2=0;
						break;
					}
				}
				(n->next[i])->evaluation=min(n->next[i]);
			}
			save2=0;
		}
		n->evaluation=min(n);
	}
	if(dicethrow==r2)
	{
		for(i=0;i<15;i++)
		{
			n->next[i]=formtree(n->next[i],n,r2);
			if(sequence==2 && n->next[i]!=NULL && r1)
			{
				for(j=0;j<15;j++)
				{
					(n->next[i])->next[j]=formtree((n->next[i])->next[j],n->next[i],r1);
					if(save1==24)
					{
						save1=0;
						break;
					}
				}
				(n->next[i])->evaluation=min(n->next[i]);
			}
			save1=0;
		}
		n->evaluation=min(n);
	}
}
int main()
{
	char bar[10];
	int i,j;
	for(i=0;i<24;i++)
		scanf("%d",&board[i]);
	getchar();
	int length=0;
	char c='x';
	while(c!='\n')
	{
		c=getchar();
		if(c!='\n')
			bar[length++]=c;
	}
	bar[length]='\0';
	length=strlen(bar);
	int mybar=0,opbar=0;
	for(i=0;i<length;i++)
	{
		if(bar[i]=='a')
			mybar++;
		else if(bar[i]=='b')
			opbar++;
	}
	scanf("%d",&r1);
	scanf("%d",&r2);
	node *bar1move=NULL;
	node *bar2move=NULL;
	int moves=2;
	if(mybar && moves)
	{
		if(r1!=0 && board[r1-1]==-1)
		{
			bar1move=(node *)malloc(sizeof(node));
			bar1move->pos1=-1;
			bar1move->pos2=r1-1;
			board[r1-1]=1;
			opbar++;
			mybar--;
			moves--;
			r1=0;
			print(bar1move);
		}
		if(moves && mybar && r2!=0 && board[r2-1]==-1)
		{
			bar2move=(node *)malloc(sizeof(node));
			bar2move->pos1=-1;
			bar2move->pos2=r2-1;
			board[r2-1]=1;
			opbar++;
			mybar--;
			moves--;
			r2=0;
			print(bar2move);
		}
		if(moves && mybar && r1!=0 && board[r1-1]>0)
		{
			bar1move=(node *)malloc(sizeof(node));
			bar1move->pos1=-1;
			bar1move->pos2=r1-1;
			board[r1-1]++;
			opbar++;
			mybar--;
			moves--;
			r1=0;
			print(bar1move);
		}
		if(moves && mybar && r2!=0 && board[r2-1]>0)
		{
			bar2move=(node *)malloc(sizeof(node));
			bar2move->pos1=-1;
			bar2move->pos2=r2-1;
			board[r2-1]++;
			opbar++;
			mybar--;
			moves--;
			r2=0;
			print(bar2move);
		}
		if(moves && mybar && r1!=0 && board[r1-1]==0)
		{
			bar1move=(node *)malloc(sizeof(node));
			bar1move->pos1=-1;
			bar1move->pos2=r1-1;
			board[r1-1]++;
			opbar++;
			mybar--;
			moves--;
			r1=0;
			print(bar1move);
		}
		if(moves && mybar && r2!=0 && board[r2-1]==0)
		{
			bar2move=(node *)malloc(sizeof(node));
			bar2move->pos1=-1;
			bar2move->pos2=r2-1;
			board[r2-1]++;
			opbar++;
			mybar--;
			moves--;
			r2=0;
			print(bar2move);
		}
		if(mybar && moves)
		{
			if(bar1move==NULL && bar2move==NULL)
			{
				printf("pass\npass\n");
				return 0;
			}
			if(bar1move==NULL)
			{
				printf("pass\n");
				return 0;
			}
			if(bar2move==NULL)
			{
				printf("pass\n");
				return 0;
			}
		}
		if(!mybar && !moves)
			return 0;
		if(mybar && !moves)
			return 0;
	}
	int bearoff=0;
	for(i=0;i<18;i++)
		if(board[i]>0)
			bearoff=1;
	if(bearoff==0)
	{
		node *t1=NULL,*t2=NULL;
		//printf("board[24-r1]=%d\nboard[24-r2]%d\n",board[24-r1],board[24-r2]);
		if(board[24-r1]>0)
		{
			board[24-r1]--;
			moves--;
			t1=(node *)malloc(sizeof(node));
			t1->pos1=24-r1;
			t1->pos2=24;
			print(t1);
			r1=0;
		}
		if(board[24-r2]>0)
		{
			board[24-r2]--;
			moves--;
			t2=(node *)malloc(sizeof(node));
			t2->pos1=24-r2;
			t2->pos2=24;
			print(t2);
			r2=0;
		}
		for(i=18;i<24;i++)
		{
			if(moves && board[i]>0)
			{
				if(r1 && i+r1<24)
				{
					board[i]--;
					board[i+r1]++;
					t1=(node *)malloc(sizeof(node));
					t1->pos1=i;
					t1->pos2=i+r1;
					moves--;
					r1=0;
					print(t1);
				}
				if(r2 && i+r2<24 && moves)
				{
					board[i]--;
					board[i+r2]++;
					t2=(node *)malloc(sizeof(node));
					t2->pos1=i;
					t2->pos2=i+r2;
					moves--;
					r2=0;
					print(t2);
				}
			}
		}
		if(moves)
		{
			for(i=17;i<24;i--)
			{
				if(moves && board[i]>0)
				{
					if(r1 && r1>24-i)
					{
						board[i]--;
						r1=0;
						t1=(node *)malloc(sizeof(node));
						t1->pos1=i;
						t1->pos2=24;
						moves--;
						print(t1);
					}
					if(moves && r2 && board[i] && r2>24-i)
					{
						board[i]--;
						r2=0;
						t2=(node *)malloc(sizeof(node));
						t2->pos1=i;
						t2->pos2=24;
						moves--;
						print(t2);
					}
				}
			}
		}
		if(moves==0)
		{
			return 0;
		}
	}
	if(r1)
		head1=(node *)malloc(sizeof(node));
	if(r2)
		head2=(node *)malloc(sizeof(node));
	for(i=0;i<15;i++)
	{
		if(r1)
			head1->next[i]=NULL;
		if(r2)
			head2->next[i]=NULL;
		for(i=0;i<24;i++)
		{
			if(r1)
				head1->boardstate[i]=board[i];
			if(r2)
				head2->boardstate[i]=board[i];
		}
	}
	sequence=1;
	//printf("Forming tree 1\n\n");
	if(r1)
	{
		formtree(head1,head1,r1);
	}
	save1=0;
	save2=0;
	sequence=2;
	//printf("\n\nForming tree 2\n\n");
	if(r2)
	{
		formtree(head2,head2,r2);
	}
	if(r1 && r2)
	{
		if(head1->next[0]==NULL && head2->next[0]==NULL)
		{
			printf("pass\npass\n");
			return 0;
		}
		if((head2->next[0]==NULL && head1->next[0]!=NULL) || head1->evaluation<head2->evaluation)
		{
			for(i=0;i<15;i++)
			{
				if(head1->next[i]==NULL)
					break;
				if(head1->evaluation==(head1->next[i])->evaluation)
				{
					print(head1->next[i]);
					for(j=0;j<15;j++)
					{
						if((head1->next[i])->next[0]==NULL)
						{
							printf("pass\n");
							return 0;
						}
						if((head1->next[i])->next[j]==NULL)
							break;
						if(head1->evaluation==((head1->next[i])->next[j])->evaluation)
						{
							print((head1->next[i])->next[j]);
							return 0;
						}
					}
				}
			}
		}
		else
		{
			for(i=0;i<15;i++)
			{
				if(head2->next[i]==NULL)
						break;
				if(head2->evaluation==(head2->next[i])->evaluation)
				{
					print(head2->next[i]);
					for(j=0;j<15;j++)
					{
						if((head2->next[i])->next[0]==NULL)
						{
							printf("pass\n");
							return 0;
						}
						if((head2->next[i])->next[j]==NULL)
							break;
						if(head2->evaluation==((head2->next[i])->next[j])->evaluation)
						{
							print((head2->next[i])->next[j]);
							return 0;
						}
					}
				}
			}
		}
	}
	if(r1)
	{
		if(head1->next[0]==NULL)
		{
			printf("pass\n");
			return 0;
		}
		for(i=0;i<15;i++)
		{
			if(head1->next[i]==NULL)
				break;
			if(head1->evaluation==(head1->next[i])->evaluation)
			{
				print(head1->next[i]);
				for(j=0;j<15;j++)
				{
					if((head1->next[i])->next[j]==NULL)
						break;
					if(head1->evaluation==((head1->next[i])->next[j])->evaluation)
					{
						print((head1->next[i])->next[j]);
						return 0;
					}
				}
			}
		}
	}
	if(r2)
	{
		if(head2->next[0]==NULL)
		{
			printf("pass\n");
			return 0;
		}
		for(i=0;i<15;i++)
		{
			if(head2->next[i]==NULL)
				break;
			if(head2->evaluation==(head2->next[i])->evaluation)
			{
				print(head2->next[i]);
				for(j=0;j<15;j++)
				{
					if((head2->next[i])->next[j]==NULL)
						break;
					if(head2->evaluation==((head2->next[i])->next[j])->evaluation)
					{
						print((head2->next[i])->next[j]);
						return 0;
					}
				}
			}
		}
	}
	return 0;
}

# include "stdio.h"
#include <stdlib.h>
#include<string.h>
#if !defined(__APPLE__) && !defined(__FreeBSD__)
  #include <malloc.h> // for memalign
#endif

/*
Алгоритм Флойда-Уоршела для сколь угодно больших графов. Единстеввное ограничение - длина максимально пути должна быть < 10^19.
При необходимости, это можно исправить за один вечер. Для решения используется динамически создаваемый и полностью связный двумерный массив.
Правильность реализации была проверена  на задаче http://informatics.mccme.ru/mod/statements/view.php?id=25, где были успешно пройдены все тесты.
Для проверки предлагается первый тест
Матрица смежности (4 вершины)
0 5 9 100
100 0 2 8
100 100 0 7
4 100 100 0
Ответ
0 5 7 13
12 0 2 8
11 16 0 7
4 9 11 0
Компилировано на GNU C 4
*/


typedef struct cell
{
    struct cell *right;
    struct cell *down;
    struct cell *up;
    struct cell *left;
    long long value;
} cell ;


int min(long long a, long long b)
{
    return a < b ? a : b;
}
struct cell *rowCell;
struct cell *head=NULL;
struct cell *rightEnd;
struct cell *downEnd;
struct cell *prev;
struct cell* getNewCell(void)
{
	 struct cell* c=(cell *) malloc( sizeof(cell));
	 c->down=NULL;
	 c->left=NULL;
	 c->right=NULL;
	 c->up=NULL;
	 return c;
}
void constructCell(struct cell* curr, struct cell *prev, int isFirstRow, int isLastColumn)
{
	curr->down=getNewCell();
	curr->down->up=curr;
	if(isFirstRow||isLastColumn)
	{
		curr->right=getNewCell();
		curr->right->left=curr;
		if(!isFirstRow)
		{
			curr->right->up=curr->up->right;
			curr->up->right->down=curr->right;
		}
	}
	if(prev!=NULL)
	{
		prev->down->right=curr->down;
		curr->down->left=prev->down;
		prev->down->right->up=curr;
	}
}

void addNewVertex(int cnt)
{
	if(head==NULL)
	{
		head=getNewCell();
		struct cell *curr=head;
		rowCell=head;
		rightEnd=head;
		downEnd=head;
	    int i;
        for( i=0; i<cnt; i++)              //первое добавление вершин, формируется начальная квадратная матрица, которая в дальнейшем будет расширяться.
        {
            int j;
            for(j=0; j<cnt; j++)
            {
				int ifr=0;
				if(i==0)
					ifr=1;
				int ilc=0;
				if(j==cnt-1)
					ilc=1;
                constructCell(curr,prev,ifr,ilc);
                prev=curr;
                curr=curr->right;
                if(i==0)
                    rightEnd=curr;
            }
			curr=rowCell->down;
			rowCell=rowCell->down;
			prev=NULL;
			downEnd=rowCell;
		 }

	 }
	else
	{
			struct cell *newHead=getNewCell();
            struct cell *newCurr=newHead;
            struct cell *newRowCell=newHead;
            struct cell *newPrev=NULL;
            struct cell *newRightEnd=NULL;
            struct cell *height=head;
            int i=0;
            int wf=0;
            while(1)
            {
                int j;
            for(j=0; j<cnt; j++)                   // создание матрицы прямоуголька NxM , где М- количество вериш для добавления, N - текущее количество вершин
            {
				int ifr=0;
				if(!wf)
					ifr=1;
				int ilc=0;
				if(j==cnt-1)
					ilc=1;
				constructCell(newCurr,newPrev,ifr,ilc);
                newPrev=newCurr;
                newCurr=newCurr->right;
                if(!wf)
                    newRightEnd=newCurr;
            }
			newCurr=newRowCell->down;
			newRowCell=newRowCell->down;
			newPrev=NULL;
			height=height->down;
			if(height->down==NULL)
				break;
			wf=1;
        }
        rightEnd=rightEnd->left;
        while (1)                                                  // "cшивание" двух матриц, существующая слева, сгенерированная на предыдущем этапе справа
        {                                                           // в результате имеем половину (верхнюю) итоговой матрицы
            rightEnd->right=newHead;
            newHead->left=rightEnd;
            if(newHead->down->down==NULL)
                break;
            newHead=newHead->down;
            rightEnd=rightEnd->down;
        }
        rightEnd=newRightEnd;
        newHead=getNewCell();                                 // генерация нижней половины матрицы. Данная половина будет иметь размер XxY где X - количество вершин для добавления
        newCurr=newHead;                                   // Y = Х + Old, где Old - количество вершин в графе до добавления вершин (Y - новый размер матрицы)
        newRowCell=newHead;
        newPrev=NULL;
        struct cell *newDownEnd=NULL;
        wf=0;
        struct cell *width=head;
        for(i=0; i<cnt; i++)
        {
			while(1)
            {
				int ifr=0;
				if(i==0)
					ifr=1;
				int ilc=0;
				if(width==rightEnd->left)
					ilc=1;
				constructCell(newCurr,newPrev,ifr,ilc);
                newPrev=newCurr;
                newCurr=newCurr->right;
                width=width->right;
                if(width==rightEnd)
                        break;
            }
            width=head;
            newCurr=newRowCell->down;
            newRowCell=newRowCell->down;
            newDownEnd=newRowCell;
            newPrev=NULL;
		}
		downEnd=downEnd->up;
        while (1)                                                // "Сшивание" верхней и нижней части
        {
            downEnd->down=newHead;
            newHead->up=downEnd;
            if(newHead->right->right==NULL)
            {
                break;
            }
            downEnd=downEnd->right;
            newHead=newHead->right;
        }
		downEnd=newDownEnd;
	}
}
void calculate(struct cell *hd)
{
    struct cell *currIK=head;
    struct cell *currColumnIK=head;
    struct cell *currKJ=head;
    struct cell *currColumnKJ=head;
    struct cell *widK=head;
    struct cell *widI=head;
    struct cell *widJ=head;
	
    while(1)                                 // Непосредственная реализация алгоритма в условиях особоый структуры хранения матрицы смежности
    {
       struct cell *curr=head;
        struct cell *rowCell=head;
        widI=head;
        while(1)
        {
            widJ=head;
            while(1)
            {
                curr->value=min(curr->value,currIK->value+currKJ->value);
                currKJ=currKJ->right;
                curr=curr->right;
                widJ=widJ->right;
                if(widJ->right==NULL)
                    break;

            }
            rowCell=rowCell->down;
            curr=rowCell;
            currIK=currIK->down;
            currKJ=currColumnKJ;
            widI=widI->right;
            if(widI->right==NULL)
                break;

        }
        currIK=currColumnIK->right;
        currColumnIK=currColumnIK->right;
        currColumnKJ=currColumnKJ->down;
        currKJ=currColumnKJ;
        widK=widK->right;
        if(widK->right==NULL)
            break;


    }
}
void printMatrix(struct cell *head)
{
	struct cell *widI=head;
    struct cell *widJ=head;
    struct cell *curr=head;
    struct cell *rowCell=head;
    while(1)
    {
        widJ=head;
        while(1)
		{
            printf("%i ", curr->value);
            curr=curr->right;
            widJ=widJ->right;
            if(widJ->right==NULL)
                break;
        }

		printf("\n");
		curr=rowCell->down;
		rowCell=rowCell->down;
		widI=widI->right;
		if(widI->right==NULL)
			break;
    }
}
long long ToInt64(char *s)
{
	int len=strlen(s);
	long long tens=1;
	long long rs=0;
	int i;
	for(i=len-1; i>=0; i--)
	{
		rs+=(s[i]-'0')*tens;
		tens*=10;
	}
	return rs;
}
void matrixFromFile(FILE *f)
{
	head=getNewCell();
	int ch=1;
	int i=0;
	char *p=(char *) malloc( sizeof(char));
	struct cell *curr=head;
	downEnd=head;
	int isFirst=1;
	prev=NULL;
	while(ch!=EOF)
	{
		ch=fgetc(f);
		if(ch==' ')
		{
			p[i]='\0';
			long long val=ToInt64(p);
			constructCell(curr,prev,isFirst,0);
			curr->value=val;
			prev=curr;
			curr=curr->right;
			i=0;
			continue;
		}
		if(ch=='\n'|| ch=='\r')
		{
			
			p[i]='\0';
			long long val=ToInt64(p);
			constructCell(curr,prev,isFirst,1);
			curr->value=val;
			prev=NULL;
			curr=downEnd->down;
			downEnd=downEnd->down;
			i=0;
			isFirst=0;
			continue;
		}
		p[i]=ch;
		i++;
	}
	p[i-1]='\0';
	long long val=ToInt64(p);
	constructCell(curr,prev,isFirst,1);
	curr->value=val;
}
int main(int argc, char * argv[])
{
	
	if(argc==1)
	{
		while(1)                // Добоавление вершин в матрицу смежности. Каждый раз, имеющаяся матрица смежности увеличивает количество столбцов и строк на указаное число.
		{
			int b;
			printf("Укажите число вершин, которое желаете добавить ( -1  закончить ввод)\n");
			scanf("%i", &b);
			if(b==-1)
				break;
			addNewVertex(b);
		}
		struct cell *curr=head;
		rowCell=head;
		printf("введите корректную и соответствующую указанным размерам графа матрицу смежности\n");
		while(curr->down!=NULL)
		{
			while(curr->right!=NULL)
			{
				int x;
				scanf("%i", &x);
				curr->value=x;
				curr=curr->right;
			}
			rowCell=rowCell->down;
			curr=rowCell;

		}
	}
	else 
	{
		FILE *f=fopen(argv[1],"r");
		matrixFromFile(f);
	}
	calculate(head);
    printf("Результат\n");
	printMatrix(head);
    return 0;
}

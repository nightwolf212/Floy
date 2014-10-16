﻿# include "stdio.h"
#include <stdlib.h>

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

int main()
{
    int n;
    //scanf("%i", &n);
    struct cell *head=(cell *) malloc( sizeof(cell));
    head->down=NULL;
    head->left=NULL;
    head->right=NULL;
    head->up=NULL;
    struct cell *curr=head;
    struct cell *rowCell=head;
    struct cell *prev=NULL;
    struct cell *rightEnd=head;
    struct cell *downEnd=head;
    int wasFirst=0;
    while(1)                // Добоавление вершин в матрицу смежности. Каждый раз, имеющаяся матрица смежности увеличивает количество столбцов и строк на указаное число.
    {
        int b;
        printf("Укажите число вершин, которое желаете добавить ( -1  закончить ввод)\n");
        scanf("%i", &b);
        if(b==-1)
            break;
        curr=rightEnd;
        prev=NULL;
        rowCell=rightEnd;
        int cnt=0;
        if(wasFirst)                               // Случай, если уже было произведено добавление хотя бы одной вершины.
        {
            struct cell *newHead=(cell *)malloc( sizeof(cell));
            newHead->down=NULL;
            newHead->left=NULL;
            newHead->right=NULL;
            newHead->up=NULL;

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
            for(j=0; j<b; j++)                   // создание матрицы прямоуголька NxM , где М- количество вериш для добавления, N - текущее количество вершин
            {
                newCurr->down=(cell *)malloc( sizeof(cell));
                newCurr->down->down=NULL;
                newCurr->down->left=NULL;
                newCurr->down->right=NULL;
                newCurr->down->up=NULL;

                newCurr->down->up=newCurr;
                if(!wf||j==b-1)
                {
                    newCurr->right=(cell *)malloc( sizeof(cell));
                    newCurr->right->down=NULL;
                    newCurr->right->left=NULL;
                    newCurr->right->right=NULL;
                    newCurr->right->up=NULL;

                    newCurr->right->left=newCurr;
                    if(wf)
                    {
                        newCurr->right->up=newCurr->up->right;
                        newCurr->up->right->down=newCurr->right;
                    }
                }
                if(newPrev!=NULL)
                {
                    newPrev->down->right=newCurr->down;
                    newCurr->down->left=newPrev->down;
                    newPrev->down->right->up=newCurr;
                }
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


             newHead=(cell *)malloc( sizeof(cell));                                 // генерация нижней половины матрицы. Данная половина будет иметь размер XxY где X - количество вершин для добавления
             newCurr=newHead;                                   // Y = Х + Old, где Old - количество вершин в графе до добавления вершин (Y - новый размер матрицы)
             newRowCell=newHead;
             newPrev=NULL;
             newHead->down=NULL;
             newHead->left=NULL;
             newHead->right=NULL;
             newHead->up=NULL;
             struct cell *newDownEnd=NULL;
             wf=0;
            struct cell *width=head;

            for(i=0; i<b; i++)
            {
                while(1)
                {
                    newCurr->down=(cell *)malloc( sizeof(cell));
                    newCurr->down->down=NULL;
                    newCurr->down->left=NULL;
                    newCurr->down->right=NULL;
                    newCurr->down->up=NULL;
                    newCurr->down->up=newCurr;
                    if(i==0||width==rightEnd->left)
                    {
                        newCurr->right=(cell *)malloc( sizeof(cell));
                        newCurr->right->down=NULL;
                        newCurr->right->left=NULL;
                        newCurr->right->right=NULL;
                        newCurr->right->up=NULL;
                        newCurr->right->left=newCurr;
                        if(i!=0)
                        {
                            newCurr->right->up=newCurr->up->right;
                            newCurr->up->right->down=newCurr->right;
                        }
                    }
                    if(newPrev!=NULL)
                    {
                        newPrev->down->right=newCurr->down;
                        newCurr->down->left=newPrev->down;
                        newPrev->down->right->up=newCurr;
                    }
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


        else
        {
            int i;
        for( i=0; i<b; i++)              //первое добавление вершин, формируется начальная квадратная матрица, которая в дальнейшем будет расширяться.
        {
            int j;
            for(j=0; j<b; j++)
            {
                curr->down=(cell *)malloc( sizeof(cell));
                curr->down->down=NULL;
                curr->down->left=NULL;
                curr->down->right=NULL;
                curr->down->up=NULL;
                curr->down->up=curr;
                if(i==0||j==b-1)
                {
                    curr->right=(cell *)malloc( sizeof(cell));
                    curr->right->down=NULL;
                    curr->right->left=NULL;
                    curr->right->right=NULL;
                    curr->right->up=NULL;
                    curr->right->left=curr;
                    if(i!=0)
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
                    prev=curr;
                curr=curr->right;
                if(i==0)
                    rightEnd=curr;
            }
        curr=rowCell->down;
        rowCell=rowCell->down;
        prev=NULL;
        if(!wasFirst)
            downEnd=rowCell;
        }
        wasFirst=1;

    }
    }
    curr=head;
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
    int t=0;
    t++;


    struct cell *currIK=head;
    struct cell *currColumnIK=head;
    struct cell *currKJ=head;
    struct cell *currColumnKJ=head;
    struct cell *widK=head;
    struct cell *widI=head;
    struct cell *widJ=head;

    while(1)                                 // Непосредственная реализация алгоритма в условиях особоый структуры хранения матрицы смежности
    {
        curr=head;
        rowCell=head;
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
    curr=head;
    rowCell=head;
    widI=head;
    widJ=head;
    printf("Результат\n");
    while(1)
    {
        widJ=head;
        while(1){
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

    return 0;
}

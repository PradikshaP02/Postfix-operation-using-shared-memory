/*Client*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define  NOT_READY  -1
#define  FILLED     0
#define  TAKEN      1

struct Memory {
     int  status;
     char  data[20];
};

//For Staxk
int MAXSIZE = 10;
int stack[10];
int top = -1;

int isempty()
{
    if(top == -1)
        return 1;
    else
        return 0;
}

int isfull()
{
    if(top == MAXSIZE)
        return 1;
    else
        return 0;
}

int pop()
{
    int data;
    if(!isempty())
    {
          data = stack[top];
          top = top - 1;
          return data;
    }
    else
      printf("Stack is empty\n");
}

int push(int data)
{
    if(!isfull())
    {
        top = top + 1;
        stack[top] = data;
    }
    else
        printf("Stack is full\n");
}

void postfix(char* arr)
{
    for (int i=0;i<strlen(arr);i++)
    {
        if (isdigit(arr[i]))
            push(arr[i] - '0');
        else
        {
            int val1 = pop();
            int val2 = pop();
            switch (arr[i])
            {
                case '+': push( val2 + val1); break;
                case '-': push( val2 - val1); break;
                case '*': push( val2 * val1); break;
                case '/': push( val2/val1); break;
            }
        }
    }
    printf("\nAnswer after evaluation of Postfix Operation is : %d\n",pop());
}

void  main()
{
    key_t ShmKEY;
    int ShmID;
    struct Memory *ShmPTR;

    ShmKEY = 765;
    ShmID = shmget(ShmKEY, sizeof(struct Memory), 0666);
    if (ShmID < 0)
    {
        printf("*** shmget error (client) ***\n");
        exit(1);
    }

    ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
    if ((int) ShmPTR == -1)
    {
        printf("*** shmat error (client) ***\n");
        exit(1);
    }

    while (ShmPTR->status != FILLED);//Wait till Fill

    ShmPTR->status = TAKEN;

    postfix(ShmPTR->data);

    shmdt((void *) ShmPTR);

    exit(0);
}

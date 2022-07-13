/*Serevr*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

int  main()
{
    key_t ShmKEY;
    int ShmID;
    struct Memory *ShmPTR;
    char oper[20];

    ShmKEY =  765;
    ShmID = shmget(ShmKEY, sizeof(struct Memory), IPC_CREAT | 0666);
    if (ShmID < 0)
    {
        printf("*** shmget error (server) ***\n");
        exit(1);
    }

    ShmPTR = (struct Memory *)shmat(ShmID, NULL, 0);
    if ((int) ShmPTR == -1)
    {
        printf("*** shmat error (server) ***\n");
        exit(1);
    }

    printf("Enter the Postfix Operation : ");
    scanf("%s",oper);

    ShmPTR->status  = NOT_READY;
    //Fill Data
    for(int i=0;i<strlen(oper);i++)
    {
        ShmPTR->data[i] = oper[i];
    }
    ShmPTR->status = FILLED;

    while (ShmPTR->status != TAKEN)
        sleep(1);

    shmdt((void *) ShmPTR);
    shmctl(ShmID, IPC_RMID, NULL);

    exit(0);
}

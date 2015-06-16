/*
 * =====================================================================================
 *
 *       Filename:  semaphore.c
 *
 *    Description:  semaphore packaging
 *
 *        Version:  1.0
 *        Created:  06/16/2015 10:40:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (zsir), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaphore.h" 

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};


int sem_id;

void create_sem(void)
{
    sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);    
}

int set_semvalue(void)
{
    union semun sem_union;

    sem_union.val = 1;
    if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
        return -1;

    return 0;
}

void del_semvalue(void)
{
    union semun sem_union;

    sem_union.val = 1;
    if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
        printf("del semvalue fail\r\n");
}

int semaphore_p(void)
{
    struct sembuf sem_b;

    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    sem_b.sem_flg = SEM_UNDO;
    if(semop(sem_id, &sem_b, 1) == -1) {
        printf("semaphore_p failed\r\n");
        return -1;
    }

    return 0;
}

int semaphore_v(void)
{
    struct sembuf sem_b;

    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    sem_b.sem_flg = SEM_UNDO;
    if(semop(sem_id, &sem_b, 1) == -1) {
        printf("semaphore_v failed\r\n");
        return -1;
    }

    return 0;
}



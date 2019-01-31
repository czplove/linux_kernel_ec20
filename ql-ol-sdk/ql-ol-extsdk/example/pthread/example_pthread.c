/*
* =====================================================================================
*
* Filename: pthread1.c
*
* Description: A Simple program of showing What pthread is
*
* Version: 1.0
* Created: 06/07/2016
*
* Author: Quectel
*
* =====================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void* thrd_func1(void* arg);
void* thrd_func2(void* arg);

int main(void)
{
    pthread_t tid1;
    pthread_t tid2;
    void* tret;

    printf("OpenLinux: thread example \n");
    printf("Main process: PID: %d, TID: %lu\n\n", getpid(), pthread_self()); 
    
    // Create thread 1
    if (pthread_create(&tid1, NULL, thrd_func1, NULL) != 0)
    {
        printf("Create thread 1 error!\n");
        exit(1);
    }
    printf("Create thread 1, TID: %lu\n", tid1);

    // Create thread 2
    if (pthread_create(&tid2, NULL, thrd_func2, NULL) != 0)
    {
        printf("Create thread 2 error!\n");
        exit(1);
    }
    printf("Create thread 2, TID: %lu\n", tid2);
    

    // Wait for the end of thread 1, and put the return value in "tret"
    if (pthread_join(tid1, &tret) != 0)
    {
        printf("Join thread 1 error!\n");
        exit(1);
    }
    printf("Thread 1 exit code: %d\n", *((int *)tret));

    // Wait for the end of thread 2, and put the return value in "tret"
    if (pthread_join(tid2, &tret) != 0)
    {
        printf("Join thread 2 error!\n");
        exit(1);
    }
    printf("Thread 2 exit code: %d\n", *((int *)tret));

    return 0;
}

void* thrd_func1(void *arg)
{
    printf("\nThread 1 returning!\n");
    printf("PID: %u, TID: %lu\n", getpid(), pthread_self());
    return ((void *)1); // Exit the thread
}

void* thrd_func2(void *arg)
{
    printf("\nThread 2 exiting!\n");
    printf("PID: %u, TID: %lu\n", getpid(), pthread_self());
    pthread_exit((void *)2);  // Explicitly exit the thread, and return (void *)2
}

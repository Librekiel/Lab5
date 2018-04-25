#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t res_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t res_2 = PTHREAD_MUTEX_INITIALIZER;

void *proc_1()
{
    pthread_mutex_lock(&res_1);
    printf("Process 1 using resuorce 1\n");
    sleep(1);
    printf("Process 1 trying to get resource 2\n");
    pthread_mutex_lock(&res_2);
    printf("Process 1 get resource 2\n");
    
    pthread_mutex_unlock(&res_1);
    pthread_mutex_unlock(&res_2);
}

void *proc_2()
{
    pthread_mutex_lock(&res_2);
    printf("Process 2 using resuorce 2\n");
    sleep(1);
    printf("Process 2 trying to get resource 1\n");
    pthread_mutex_lock(&res_1);
    printf("Process 2 get resource 1\n");
    
    pthread_mutex_unlock(&res_2);
    pthread_mutex_unlock(&res_1);
}

int main(void)
{
    pthread_t thread_1;
    pthread_t thread_2;
    if (pthread_create(&thread_1, NULL, proc_1, NULL)!=0) {
        printf("Error: pthread_create failed!\n");
        return 1;
    }
    if (pthread_create(&thread_1, NULL, proc_2, NULL)!=0) {
        printf("Error: pthread_create failed!\n");
        return 1;
    }
    if (pthread_join(thread_1, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }
    if (pthread_join(thread_2, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }
    
    return 0;
}
#include <pthread.h>
#include <stdio.h>
#define MAX_NUM 100

typedef struct
{
    int num[5];
    int length;
}BUFFER;

BUFFER buf;
pthread_mutex_t pc_mutex;
pthread_cond_t pc_condp, pc_condc;


void * producer(void * nul)
{
    int i;
    for (i = 1; i <= MAX_NUM; i++)
    {
        pthread_mutex_lock(&pc_mutex);


        while(buf.length >= 5)
        {
            pthread_cond_wait(&pc_condp, &pc_mutex);
        }

        buf.num[buf.length] = i;
        printf("producer produces %d, buffer length: %d\n", buf.num[buf.length], buf.length+1);
        buf.length += 1;

        pthread_cond_signal(&pc_condc);
        pthread_mutex_unlock(&pc_mutex);
        // usleep(1);
    }
    pthread_exit(NULL);

}


void * consumer(void * nul)
{
    int i;
    for (i = 1; i <= MAX_NUM; i++)
    {
        pthread_mutex_lock(&pc_mutex);

        while(buf.length == 0) {
            pthread_cond_wait(&pc_condc, &pc_mutex);
        }

        printf("consumer consumes %d, buffer length: %d\n", buf.num[buf.length-1], buf.length-1);
        buf.length -= 1;

        pthread_cond_signal(&pc_condp);
        pthread_mutex_unlock(&pc_mutex);
    }

    pthread_exit(NULL);
}



int main(int argc, char const *argv[])
{
    pthread_t thread[2];
    pthread_attr_t attr;

    buf.length = 0;

    pthread_mutex_init(&pc_mutex, NULL);
    pthread_cond_init(&pc_condp, NULL);
    pthread_cond_init(&pc_condc, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&thread[0], &attr, producer, NULL);
    pthread_create(&thread[1], &attr, consumer, NULL);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    pthread_mutex_destroy(&pc_mutex);
    pthread_cond_destroy(&pc_condc);
    pthread_cond_destroy(&pc_condp);
    pthread_attr_destroy(&attr);

    pthread_exit(NULL);
    return 0;
}

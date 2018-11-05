#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

typedef struct work_entry
{
	char cmd;
	int key;
}work_entry;

typedef struct package
{
    work_entry work_queue[5];
    int workQueueLength;
    bool endFile;
    FILE *fp;
    pthread_mutex_t mutex;
}package;

/*The producer function is used to read data from target files, discard those which are illegal,
encode the legal ones and put them in the work_queue buffer until the queue is full.*/
void *producer(void *arg)
{
	package *p;
	p = (package *)arg;

	char f_cmd;
	int f_key;

	while(true){
        pthread_mutex_lock(&(p -> mutex));

        while(p -> workQueueLength >= 5){
            pthread_cond_wait(&(p -> producerC), &(p -> mutex));
        }

        fscanf(p -> fp, "%c%d", &f_cmd, &f_key);
        /*Keep reading data form the file until it captures a legal one.*/
        while((f_cmd != 'A' && f_cmd != 'B' && f_cmd != 'C' && f_cmd != 'D' && f_cmd != 'X') || f_key < 0 || f_key > 1000){
            fscanf(p -> fp, "%c%d", &f_cmd, &f_key);
        }
        /*If the reading reaches its end, turn endFile to True.*/
        if(f_cmd == 'X'){
            printf("produce endFile true -- len:%d\n", p->workQueueLength);
            p -> endFile = true;
            pthread_cond_signal(&(p -> consumerC));
            pthread_mutex_unlock(&(p -> mutex));
            break;
        }
        (p -> work_queue[p -> workQueueLength]).cmd = f_cmd;
        (p -> work_queue[p -> workQueueLength]).key = f_key;
        p -> workQueueLength += 1;
        printf("produce -- len:%d, cmd:%c, key:%d\n", p->workQueueLength, p->work_queue[p->workQueueLength-1].cmd, p->work_queue[p->workQueueLength-1].key);

        pthread_cond_signal(&(p -> consumerC));
        pthread_mutex_unlock(&(p ->mutex));
	}

    pthread_exit(NULL);
}

/*The consumer function is used to read every data from the buffer, decode them and print the required results.*/
void *consumer(void *arg)
{
    package *p;
	p = (package *)arg;

    while(!(p -> endFile && p -> workQueueLength == 0)){
        pthread_mutex_lock(&(p ->mutex));

        while(p -> workQueueLength == 0){
            pthread_cond_wait(&(p -> consumerC), &(p -> mutex));
        }
        printf("consumeQ:%-4d\t%-4c\t%-7d\n", p -> workQueueLength - 1, (p -> work_queue[p -> workQueueLength - 1]).cmd, (p -> work_queue[p -> workQueueLength - 1]).key);
        p -> workQueueLength -= 1;

        pthread_cond_signal(&(p -> producerC));
        pthread_mutex_unlock(&(p ->mutex));
    }

    pthread_exit(NULL);
}

int main()
{
    package *p;
    p = (package *)malloc(sizeof(package));
    p -> workQueueLength = 0;
    p -> endFile = false;
    p -> fp = fopen("PC_data_t00100", "r");

    pthread_t producerT;
    pthread_t consumerT;
    // pthread_attr_t attr;

    pthread_mutex_init(&(p -> mutex), NULL);
    pthread_cond_init(&(p -> producerC), NULL);
    pthread_cond_init(&(p -> consumerC), NULL);

    pthread_create(&producerT, NULL, producer, (void *)p);
    pthread_create(&consumerT, NULL, consumer, (void *)p);

	pthread_join(producerT, NULL);
	pthread_join(consumerT, NULL);

    pthread_mutex_destroy(&(p -> mutex));
    pthread_cond_destroy(&(p -> producerC));
    pthread_cond_destroy(&(p -> consumerC));
    // pthread_attr_destroy(&attr);

    pthread_exit(NULL);
    free(p);

	return 0;
}

/* This program is designed for lab2 by Yifei Diao. */
/* CSE 5441 Introduction to Parallel Computing. Class 27372. TuTh 12:45 - 14:05 */
/* Programming Assignment 2 */
/* This program provides a basic solution for the producer-consumer problem using single thread, which serves as
   a reference for future multi-threaded implementations. The program consists of three functions. The producer
   function is used to read data from target files, discard those which are illegal, encode the legal ones and put
   them in the work_queue buffer until the queue is full. The consumer function is used to read every data from
   the buffer, decode them and print the required results. The main function is used to provide loops in order to
   let producer function read all data from the target file and consumer function read all legal data that fills
   the queue. Also, the main function is used to count the total running times of producer module and consumer module. */
/* Compiles with GNU GCC Compiler/icc. */
/* Approximate submission date: 09/26/2018 Fall Semester. */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

/*In order to compile lab1.c with transform.o*/
extern uint16_t transformA(uint16_t input_val);
extern uint16_t transformB(uint16_t input_val);
extern uint16_t transformC(uint16_t input_val);
extern uint16_t transformD(uint16_t input_val);

typedef struct work_entry
{
	char cmd;
	uint16_t key;
}work_entry;

typedef struct package
{
    work_entry work_queue[5];
    int workQueueLength;
    bool endFile;
    pthread_mutex_t mutex;
    pthread_cond_t producerC, consumerC;
    double prod_time, cons_time;
    double prod_clock, cons_clock;
}package;

/*The producer function is used to read data from target files, discard those which are illegal,
encode the legal ones and put them in the work_queue buffer until the queue is full.*/
void *producer(void *arg)
{
	clock_t prod_startC, prod_endC;
	time_t prod_startT, prod_endT;

	package *p;
	p = (package *)arg;
	char f_cmd;
	uint16_t f_key, encode;

	while(true){
        pthread_mutex_lock(&(p -> mutex));

        while(p -> workQueueLength >= 5){
            pthread_cond_wait(&(p -> producerC), &(p -> mutex));
        }

        time(&prod_startT);
        prod_startC = clock();

        fscanf(stdin, "%c%hu", &f_cmd, &f_key);
        /*Keep reading data form the file until it captures a legal one.*/
        while((f_cmd != 'A' && f_cmd != 'B' && f_cmd != 'C' && f_cmd != 'D' && f_cmd != 'X') || f_key < 0 || f_key > 1000){
            fscanf(stdin, "%c%hu", &f_cmd, &f_key);
        }
        /*If the reading reaches its end, turn endFile to True.*/
        if(f_cmd == 'X'){
            p -> endFile = true;
            pthread_cond_signal(&(p -> consumerC));
            pthread_mutex_unlock(&(p -> mutex));
            break;
        }
        if(f_cmd == 'A'){
            encode = transformA(f_key);
        }
        if(f_cmd == 'B'){
            encode = transformB(f_key);
        }
        if(f_cmd == 'C'){
            encode = transformC(f_key);
        }
        if(f_cmd == 'D'){
            encode = transformD(f_key);
        }
        (p -> work_queue[p -> workQueueLength]).cmd = f_cmd;
        (p -> work_queue[p -> workQueueLength]).key = encode;
        p -> workQueueLength += 1;

        pthread_cond_signal(&(p -> consumerC));
        pthread_mutex_unlock(&(p -> mutex));

        time(&prod_endT);
        prod_endC = clock();
        p -> prod_time += difftime(prod_endT, prod_startT);
        p -> prod_clock += ((double)(prod_endC - prod_startC))/CLOCKS_PER_SEC;
	}

    pthread_exit(NULL);
}

/*The consumer function is used to read every data from the buffer, decode them and print the required results.*/
void *consumer(void *arg)
{
    clock_t cons_startC, cons_endC;
	time_t cons_startT, cons_endT;

    package *p;
	p = (package *)arg;
    uint16_t decode;

    while(!(p -> endFile && p -> workQueueLength == 0)){
        pthread_mutex_lock(&(p -> mutex));

        while(p -> workQueueLength <= 0){
            pthread_cond_wait(&(p -> consumerC), &(p -> mutex));
        }

        time(&cons_startT);
        cons_startC = clock();

        if((p -> work_queue[p -> workQueueLength - 1]).cmd == 'A'){
            decode = transformA((p -> work_queue[p -> workQueueLength - 1]).key);
        }
        if((p -> work_queue[p -> workQueueLength - 1]).cmd == 'B'){
            decode = transformB((p -> work_queue[p -> workQueueLength - 1]).key);
        }
        if((p -> work_queue[p -> workQueueLength - 1]).cmd == 'C'){
            decode = transformC((p -> work_queue[p -> workQueueLength - 1]).key);
        }
        if((p -> work_queue[p -> workQueueLength - 1]).cmd == 'D'){
            decode = transformD((p -> work_queue[p -> workQueueLength - 1]).key);
        }

        fprintf(stdout, "Q:%-4d\t%-4c\t%-7hu\t%-7hu\n", p -> workQueueLength - 1, (p -> work_queue[p -> workQueueLength - 1]).cmd, (p -> work_queue[p -> workQueueLength - 1]).key, decode);
        p -> workQueueLength -= 1;

        pthread_cond_signal(&(p -> producerC));
        pthread_mutex_unlock(&(p -> mutex));

        time(&cons_endT);
        cons_endC = clock();
        p -> cons_time += difftime(cons_endT, cons_startT);
        p -> cons_clock += ((double)(cons_endC - cons_startC))/CLOCKS_PER_SEC;
    }

    pthread_exit(NULL);
}

int main()
{
    /* Initialize package p */
    package *p;
    p = (package *)malloc(sizeof(package));
    p -> workQueueLength = 0;
    p -> endFile = false;
    p -> prod_time = 0;
    p -> cons_time = 0;
    p -> prod_clock = 0.000;
    p -> cons_clock = 0.000;

    pthread_t producerT;
    pthread_t consumerT;
    pthread_attr_t attr;

    pthread_mutex_init(&(p -> mutex), NULL);
    pthread_cond_init(&(p -> producerC), NULL);
    pthread_cond_init(&(p -> consumerC), NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&producerT, &attr, producer, (void *)p);
    pthread_create(&consumerT, &attr, consumer, (void *)p);

	pthread_join(producerT, NULL);
	pthread_join(consumerT, NULL);

	fprintf(stdout, "\n");
    fprintf(stdout, "TIME(2): The accumulated run time of producer module is %.f seconds.\n", p -> prod_time);
	fprintf(stdout, "TIME(2): The accumulated run time of consumer module is %.f seconds.\n", p -> cons_time);
	fprintf(stdout, "CLOCK(3): The accumulated run time of producer module is %.3f seconds.\n", p -> prod_clock);
	fprintf(stdout, "CLOCK(3): The accumulated run time of consumer module is %.3f seconds.\n", p -> cons_clock);

    pthread_mutex_destroy(&(p -> mutex));
    pthread_cond_destroy(&(p -> producerC));
    pthread_cond_destroy(&(p -> consumerC));
    pthread_attr_destroy(&attr);

    pthread_exit(NULL);
    free(p);

	return 0;
}

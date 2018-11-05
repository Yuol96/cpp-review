/* This program is designed for lab1 by Yifei Diao. */
/* CSE 5441 Introduction to Parallel Computing. Class 27372. TuTh 12:45 - 14:05 */
/* Programming Assignment 1 */
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

/*Declare work_queue as a global variable in order to pass data from producer to consumer.*/
work_entry work_queue[5];
int workQueueLength = 0;

/*endFile is a symbol showing whether the file reading reaches the end.*/
bool endFile = false;


/*The producer function is used to read data from target files, discard those which are illegal,
encode the legal ones and put them in the work_queue buffer until the queue is full.*/
void producer()
{
	char f_cmd;
	uint16_t f_key, encode;
    while(workQueueLength < 5) {
        fscanf(stdin, "%c%hu", &f_cmd, &f_key);
        /*Keep reading data form the file until it captures a legal one.*/
        while((f_cmd != 'A' && f_cmd != 'B' && f_cmd != 'C' && f_cmd != 'D' && f_cmd != 'X') || f_key < 0 || f_key > 1000){
            fscanf(stdin, "%c%hu", &f_cmd, &f_key);
        }
        /*If the reading reaches its end, turn endFile to True.*/
        if(f_cmd == 'X'){
            endFile = true;
            return;
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
        work_queue[workQueueLength].cmd = f_cmd;
        work_queue[workQueueLength].key = encode;
        workQueueLength += 1;
    }
}

/*The consumer function is used to read every data from the buffer, decode them and print the required results.*/
void consumer()
{
    uint16_t decode;
    for(int i = 0; i < workQueueLength; i++){
        if(work_queue[i].cmd == 'A'){
            decode = transformA(work_queue[i].key);
        }
        if(work_queue[i].cmd == 'B'){
            decode = transformB(work_queue[i].key);
        }
        if(work_queue[i].cmd == 'C'){
            decode = transformC(work_queue[i].key);
        }
        if(work_queue[i].cmd == 'D'){
            decode = transformD(work_queue[i].key);
        }
        fprintf(stdout, "Q:%-4d\t%-4c\t%-7hu\t%-7hu\n", i, work_queue[i].cmd, work_queue[i].key, decode);
    }
    workQueueLength = 0;
}

int main()
{
	void producer();
	void consumer();

	/*Count running time by using clock_t, the returned result has three digits after the decimal point.*/
	clock_t pro_startC, pro_endC, con_startC, con_endC;
	double sumpro_clock = 0.000, sumcon_clock = 0.000;

	/*Count running time by using time_t, the returned result has no digits after the decimal point.*/
	time_t pro_startT, pro_endT, con_startT, con_endT;
	double sumpro_time = 0, sumcon_time = 0;

	while(!endFile){
        time(&pro_startT);
        pro_startC = clock();
		producer();
        pro_endC = clock();
        time(&pro_endT);
        sumpro_clock += ((double)(pro_endC - pro_startC))/CLOCKS_PER_SEC;  //clock time
        sumpro_time += difftime(pro_endT, pro_startT);  // unix time

		time(&con_startT);
		con_startC = clock();
		consumer();
		con_endC = clock();
		time(&con_endT);
		sumcon_clock += ((double)(con_endC - con_startC))/CLOCKS_PER_SEC; //clock time
		sumcon_time += difftime(con_endT, con_startT);  // unix time
	}
	printf("\n");
	printf("CLOCK(2): The accumulated run time of producer module is %.3f seconds.\n", sumpro_clock);
	printf("CLOCK(2): The accumulated run time of consumer module is %.3f seconds.\n", sumcon_clock);
	printf("TIME(2): The accumulated run time of producer module is %.f seconds.\n", sumpro_time);
	printf("TIME(2): The accumulated run time of consumer module is %.f seconds.\n", sumcon_time);
	return 0;
}

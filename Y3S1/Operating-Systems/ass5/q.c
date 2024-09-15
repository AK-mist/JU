/***********************************************************************
 *
 * Name: 
 * Roll: 
 * 
 * Date:
 *
 * Assignment: 5, Creating DEADLOCK using Thread Programming
 * 
 *      Objectives:
 *          Define two global variables (Total_1 and Total_2) and initialize them to 1000.
 *          Use two mutexes to protect these two global variables.
 *          Also create two threads whose, function is to generate a 'random quantity' (not more than 50) and subtract it from one of the Totals and add it to the other one.
 *          While manipulating the Totals, each Thread should lock both the mutex and then unlock it after changing the Totals, to avoid deadlocks. 
 *          Also, include that information (as comment in your source file) how to avoid this kind of Deadlock.
 * 
 *      If these two (Threads, Totals and Mutex) are not good enough for Deadlock; Then you need to create another set of Thread, Total and Mutex; and carry on the same experiment.
 *      Make sure there are enough printf in your program so that it can be clearly understood that there is a Deadlock. One way to ensure enough printf is to display the total of Total_1 and Total_2 all the time after every operation on it.
 * 
 *
 * Input Description: None
 *
 * Output Description: 
 *      Displays messages showing when the threads attempt to acquire the locks, 
 *      the random value generated, the updated totals, and whether the locks were released.
 *      When deadlock occurs, the threads will be stuck without any further output.
 *
 * Compilation Command: gcc q.c
 * Execution Sequence: ./a.out
 *
 *
 * Sample Input:
 * Sample Output:
 /-----------------------------------
Thread 1: Trying to acuire lock 1...
Thread 1: Acquired lock 1
Thread 1: Trying to acuire lock 2...
Thread 1: Acquired lock 2
Thread 1: Acquired both locks
Thread 2: Trying to acuire lock 2...
Thread 1: Generated random value: 14
Thread 1: After operation, Total 1 = 1014, Total 2 = 986
Thread 1: Released lock 1
Thread 1: Released lock 2
Thread 1: Trying to acuire lock 1...
Thread 1: Acquired lock 1
Thread 1: Trying to acuire lock 2...
Thread 2: Acquired lock 2
Thread 2: Trying to acuire lock 1...
^C
 -----------------------------------/
 * 
 * Remarks:
 *      1. Two threads were enough to demonstate Deadlock.
 *      2. To prevent this, acquire locks in the same order in all processes.
 * 
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define UPPER_BOUND 50
// define flags for addition/ substraction operations
#define MODE_ADD 0x01
#define MODE_SUB 0x02 

// Global Variables
int total_1 = 1000;
int total_2 = 1000;

// Threads and Mutexes
pthread_t tid[2];                           // Thread identifiers
pthread_mutex_t lock[2];                    // Mutexes for protecting Global vars

void error_exit(const char *error_msg);     // prints error message and exits program

/*
 * Function to add or substract a given value from global variables 'total_1' 'total)_2'
 * @param1: the value to be added/ substracted
 * @param2: mode of operation
*/
void operate_on_total1(int value, int mode);
void operate_on_total2(int value, int mode);

void *thread1_func(void *arg);
void *thread2_func(void *arg);

int main(int argc, char **argv) {
    srand(time(NULL));

    if ((pthread_mutex_init(&(lock[0]), NULL) != 0) || (pthread_mutex_init(&(lock[1]), NULL) != 0)) { 
        error_exit("Error: mutex init has failed\n");  
    }
 
    if ((pthread_create(&(tid[0]), NULL, &thread1_func, NULL) != 0) || 
        (pthread_create(&(tid[1]), NULL, &thread2_func, NULL) != 0)) {
        error_exit("Error: Threads can't be created");
    }
  
    pthread_join(tid[0], NULL); 
    pthread_join(tid[1], NULL);

    pthread_mutex_destroy(&(lock[0]));
    pthread_mutex_destroy(&(lock[1]));

    return 0;
}

void error_exit(const char* error_msg) {
    perror(error_msg);
    exit(EXIT_FAILURE);
}

void operate_on_total1(int value, int mode) {
    if (mode & MODE_ADD) {
        total_1 += value;
    }
    if (mode & MODE_SUB) {
        total_1 -= value;
    }
}

void operate_on_total2(int value, int mode) {
    if (mode & MODE_ADD) {
        total_2 += value;
    }
    if (mode & MODE_SUB) {
        total_2 -= value;
    }
}

void *thread1_func(void *arg) {
    while (1) {
        // Lock mutexes in the same order to avoid deadlock
        printf("Thread 1: Trying to acuire lock 1...\n");
        pthread_mutex_lock(&lock[0]);
        printf("Thread 1: Acquired lock 1\n");

        printf("Thread 1: Trying to acuire lock 2...\n");
        pthread_mutex_lock(&lock[1]);
        printf("Thread 1: Acquired lock 2\n");

        printf("Thread 1: Acquired both locks\n");

        // Perform operation
        int value = rand() % UPPER_BOUND;
        printf("Thread 1: Generated random value: %d\n", value);

        operate_on_total1(value, MODE_ADD);
        operate_on_total2(value, MODE_SUB);
        printf("Thread 1: After operation, Total 1 = %d, Total 2 = %d\n", total_1, total_2);

        // Release locks
        printf("Thread 1: Released lock 1\n");
        pthread_mutex_unlock(&lock[0]);
        printf("Thread 1: Released lock 2\n");
        pthread_mutex_unlock(&lock[1]);
        
        // to control pace
        sleep(1);
    }
}

void *thread2_func(void *arg) {
    while (1) {
        // Lock mutexes in the same order to avoid deadlock
        printf("Thread 2: Trying to acuire lock 2...\n");
        pthread_mutex_lock(&lock[1]);
        printf("Thread 2: Acquired lock 2\n");

        printf("Thread 2: Trying to acuire lock 1...\n");
        pthread_mutex_lock(&lock[0]);
        printf("Thread 2: Acquired lock 1\n");

        printf("Thread 2: Acquired both locks\n");

        // Perform operation
        int value = rand() % UPPER_BOUND;
        printf("Thread 2: Generated random value: %d\n", value);

        operate_on_total1(value, MODE_SUB);
        operate_on_total2(value, MODE_ADD);
        printf("Thread 2: After operation, Total 1 = %d, Total 2 = %d\n", total_1, total_2);

        // Release locks
        printf("Thread 2: Released lock 2\n");
        pthread_mutex_unlock(&lock[1]);
        printf("Thread 2: Released lock 1\n");
        pthread_mutex_unlock(&lock[0]);

        // to control pace
        sleep(1);
    }
}
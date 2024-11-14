/***********************************************************************
 *
 * Name: 
 * Roll: 
 * 
 * Date:
 *
 * Assignment: 7 - Thread, Synchronizations & Shared Memory
 *              Create a main process (M) to manage three threads (Th1, Th2, Th3) and a shared memory (Mem, One variant of IPC Primitive), accessable by all 4 processes.
 *              M creates N random pairs (X, Y); x, y < 10, which are all placed inside Mem.
 *              The Tabular_Datas will be done by the threads on each pair in a synchronized manner.
 *              Firstly, Th1 will compute A (X*Y) then, Th2 will compute B (X*Y)/2) after that, Th3 computes C (X+Y), 
 *              Th2 again computes D ((X*Y)/(X+Y)), and finally Th1 computes E ((X+Y)(X-Y)).
 *              After completing one phase of Tabular_Datas they will go for the next pair in the same order.
 *              All these values are kept in the shared memory in a tabular fashion as shown below.
 *              (X, Y) | A | B | C | D | E
 *              
 *
 * Input Description: A number (N) that represents number of random pairs / itterations
 *
 * Output Description: Shared Memory Status / Info / Statistics + 
 *                      Print Pairs(X, Y) | A | B | C | D | E
 *
 *
 * Compilation Command: gcc q.c
 * Execution Sequence: ./a.out <number of pairs>
 *
 * 
 * Shared Memory Info: used 100_000 pairs
 /-----------------------------------
$ ipcs -m

------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
0x45076e9c 196655     user       644        2800104    1
/-----------------------------------
 * 
 *
 * Sample Input: 4
 * Sample Output:
 /-----------------------------------
Main: Generating 3 random pairs - Done
Th1: Waiting for signal to compute A
Th1: Computing A for pair 1 - Done
Th1: Waiting for signal to compute E
Th2: Waiting for signal to compute B
Th2: Computing B for pair 1 - Done
Th2: Waiting for signal to compute D
Th3: Waiting for signal to compute C
Th3: Computing C for pair 1 - Done
Th3: Waiting for signal to compute C
Th2: Computing D for pair 1 - Done
Th2: Waiting for signal to compute B
Th1: Computing E for pair 1 - Done
Th1: Waiting for signal to compute A
Th1: Computing A for pair 2 - Done
Th1: Waiting for signal to compute E
Th2: Computing B for pair 2 - Done
Th2: Waiting for signal to compute D
Th3: Computing C for pair 2 - Done
Th3: Waiting for signal to compute C
Th2: Computing D for pair 2 - Done
Th2: Waiting for signal to compute B
Th1: Computing E for pair 2 - Done
Th1: Waiting for signal to compute A
Th1: Computing A for pair 3 - Done
Th1: Waiting for signal to compute E
Th2: Computing B for pair 3 - Done
Th2: Waiting for signal to compute D
Th3: Computing C for pair 3 - Done
Th2: Computing D for pair 3 - Done
Th1: Computing E for pair 3 - Done


Results Table:
Each column represents the following calculations:
  - A = X * Y
  - B = (X * Y) / 2
  - C = X + Y
  - D = (X * Y) / (X + Y); (0 if, X + Y = 0)
  - E = (X + Y) * (X - Y)

--------------------------------------------------------------------------------------------------------
	Pairs(X, Y)	|	A	|	B	|	C	|	D	|	E	
--------------------------------------------------------------------------------------------------------
	(8.09, 2.81)	|	10.91	|	11.39	|	10.91	|	2.09	|	57.59	
--------------------------------------------------------------------------------------------------------
	(2.10, 1.41)	|	3.51	|	1.48	|	3.51	|	0.84	|	2.43	
--------------------------------------------------------------------------------------------------------
	(5.50, 8.87)	|	14.37	|	24.38	|	14.37	|	3.39	|	-48.42	
--------------------------------------------------------------------------------------------------------

 -----------------------------------/
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <time.h>

#define SHARED_STORAGE "shared_file"
#define MAX_PAIRS 200000
#define RANDOM_QUANTITY_UPPER_BOUND 10
#define TOTAL_TASKS 5

typedef struct {
    float X, Y;
    float A, B, C, D, E;
} Tabular_Data;

typedef struct {
    Tabular_Data *tabular_data;
    int total_pairs;
    int current_task;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Shared_Data;

float get_rand(int upper_bound);
void *compute_Th1(void *arg);
void *compute_Th2(void *arg);
void *compute_Th3(void *arg);
void display_result(Tabular_Data *shared_data, int num_pairs);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage - %s [number of random pairs]", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_pairs = atoi(argv[1]);
    if (num_pairs <= 0 || num_pairs > MAX_PAIRS) {
        fprintf(stderr, "Number of random pairs must be from 1 to %d\n", MAX_PAIRS);
        exit(EXIT_FAILURE);
    }

    // Create shared memory
    int fd = open(SHARED_STORAGE, O_CREAT | O_RDONLY, 0666);
    key_t key = ftok(SHARED_STORAGE, 69);
    size_t shared_data_size = sizeof(Shared_Data) + sizeof(Tabular_Data) * num_pairs;
    int shm_id = shmget(key, shared_data_size, IPC_CREAT | 0644);
    Shared_Data *shared_data = (Shared_Data *)shmat(shm_id, NULL, 0);
    if (key == -1 || shm_id == -1 || shared_data == (void *)-1) {
        perror("Shared Memory");
        exit(EXIT_FAILURE);
    }

    // Initialize shared data and mutexes/conditions
    shared_data->tabular_data = (Tabular_Data *)(shared_data + 1);
    shared_data->total_pairs = num_pairs;
    shared_data->current_task = 1;
    pthread_mutex_init(&shared_data->mutex, NULL);
    pthread_cond_init(&shared_data->cond, NULL);

    // Generate random pairs
    printf("Main: Generating %d random pairs", num_pairs);
    srand(time(NULL));
    for (int i = 0; i < num_pairs; i++) {
        shared_data->tabular_data[i].X = get_rand(RANDOM_QUANTITY_UPPER_BOUND);
        shared_data->tabular_data[i].Y = get_rand(RANDOM_QUANTITY_UPPER_BOUND);
    }
    printf(" - Done\n");

    // create threads
    pthread_t th1, th2, th3;
    pthread_create(&th1, NULL, compute_Th1, shared_data);
    pthread_create(&th2, NULL, compute_Th2, shared_data);
    pthread_create(&th3, NULL, compute_Th3, shared_data);

    // wait for threads
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    // display results
    display_result(shared_data->tabular_data, num_pairs);

    // Cleanup
    pthread_mutex_destroy(&shared_data->mutex);
    pthread_cond_destroy(&shared_data->cond);
    shmdt(SHARED_STORAGE);
    shmctl(shm_id, IPC_RMID, NULL);
    close(fd);

    return 0;
}

float get_rand(int upper_bound) {
    return (rand() / (float)RAND_MAX) * upper_bound;
}

void *compute_Th1(void *arg) {
    Shared_Data *shared_data = (Shared_Data *)arg;
    Tabular_Data *tabular_data = shared_data->tabular_data;

    for (int i = 0; i < shared_data->total_pairs; i++) {
        float x = tabular_data[i].X, y = tabular_data[i].Y;

        pthread_mutex_lock(&shared_data->mutex);
        
        // Wait for ?
        printf("Th1: Waiting for signal to compute A\n");
        while (shared_data->current_task != 1) {
            pthread_cond_wait(&shared_data->cond, &shared_data->mutex);
        }

        // Step 1: Compute A
        printf("Th1: Computing A for pair %d", i+1);
        tabular_data[i].A = x + y;
        printf(" - Done\n");

        // Signal Th2 to compute B and wait for Th1's turn for E
        shared_data->current_task++;
        pthread_cond_broadcast(&shared_data->cond);

        printf("Th1: Waiting for signal to compute E\n");
        while (shared_data->current_task != 5) {
            pthread_cond_wait(&shared_data->cond, &shared_data->mutex);
        }

        // Step 5: Compute E
        printf("Th1: Computing E for pair %d", i+1);
        tabular_data[i].E = (x + y) * (x - y);
        printf(" - Done\n");

        // Move onto the next pair
        shared_data->current_task++;
        shared_data->current_task %= TOTAL_TASKS;
        pthread_cond_broadcast(&shared_data->cond);

        pthread_mutex_unlock(&shared_data->mutex);
    }
    
    return NULL;
}

void *compute_Th2(void *arg) {
    Shared_Data *shared_data = (Shared_Data *)arg;
    Tabular_Data *tabular_data = shared_data->tabular_data;

    for (int i = 0; i < shared_data->total_pairs; i++) {
        float x = tabular_data[i].X, y = tabular_data[i].Y;

        pthread_mutex_lock(&shared_data->mutex);

        printf("Th2: Waiting for signal to compute B\n");
        while (shared_data->current_task != 2) {
            pthread_cond_wait(&shared_data->cond, &shared_data->mutex);
        }

        // Step 2: Compute B
        printf("Th2: Computing B for pair %d", i+1);
        tabular_data[i].B = (x * y) / 2;
        printf(" - Done\n");

        // Signal Th3 to compute C and wait for Th2's turn for D
        shared_data->current_task++;
        pthread_cond_broadcast(&shared_data->cond);

        printf("Th2: Waiting for signal to compute D\n");
        while (shared_data->current_task != 4) {
            pthread_cond_wait(&shared_data->cond, &shared_data->mutex);
        }

        // Step 4: Compute D
        printf("Th2: Computing D for pair %d", i+1);
        if (x + y == 0) {
            printf(" - Division by 0 in calculating D, returning 0 ");
        }
        tabular_data[i].D = (x + y) == 0 ? 0 : (x * y) / (float)(x + y);
        printf(" - Done\n");

        // Signal Th1 to compute E
        shared_data->current_task++;
        pthread_cond_broadcast(&shared_data->cond);

        pthread_mutex_unlock(&shared_data->mutex);
    }

    return NULL;
}

void *compute_Th3(void *arg) {
    Shared_Data *shared_data = (Shared_Data *)arg;
    Tabular_Data *tabular_data = shared_data->tabular_data;

    for (int i = 0; i < shared_data->total_pairs; i++) {
        float x = tabular_data[i].X, y = tabular_data[i].Y;

        pthread_mutex_lock(&shared_data->mutex);

        printf("Th3: Waiting for signal to compute C\n");
        while (shared_data->current_task != 3) {
            pthread_cond_wait(&shared_data->cond, &shared_data->mutex);
        }
            
        // Step 3: Compute C
        printf("Th3: Computing C for pair %d", i+1);
        tabular_data[i].C = x + y;
        printf(" - Done\n");

        // Signal Th2 to compute D
        shared_data->current_task++;
        pthread_cond_broadcast(&shared_data->cond);

        pthread_mutex_unlock(&shared_data->mutex);
    }

    return NULL;
}

void display_result(Tabular_Data *tabular_data, int num_pairs) {
    printf("\n\nResults Table:\n");
    printf("Each column represents the following calculations:\n");
    printf("  - A = X * Y\n");
    printf("  - B = (X * Y) / 2\n");
    printf("  - C = X + Y\n");
    printf("  - D = (X * Y) / (X + Y); (0 if, X + Y = 0)\n");
    printf("  - E = (X + Y) * (X - Y)\n");

    printf("\n\n--------------------------------------------------------------------------------------------------------\n");
    printf("\tPairs(X, Y)\t|\tA\t|\tB\t|\tC\t|\tD\t|\tE\t\n");
    printf("--------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < num_pairs; i++) {
        printf("\t(%.2f, %.2f)\t|\t%.2f\t|\t%.2f\t|\t%.2f\t|\t%.2f\t|\t%.2f\t\n", 
               tabular_data[i].X, tabular_data[i].Y,
               tabular_data[i].A, tabular_data[i].B,
               tabular_data[i].C, tabular_data[i].D,
               tabular_data[i].E);
        printf("--------------------------------------------------------------------------------------------------------\n");
    }
}
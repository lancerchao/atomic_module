#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
 
sem_t beginSema1, endSema;
sem_t beginSema2;
 
int X, Y;
int r1, r2;
 
void *thread1Func(void *param) {  // Initialize random number generator
    for (;;)                      // Loop indefinitely
    {
        sem_wait(&beginSema1);  // Wait for signal from main thread
        while (rand() % 8 != 0) {
        }  // Add a short, random delay
 
        // ----- THE TRANSACTION! -----
        __atomic_store_n(&X, 1, __ATOMIC_SEQ_CST);
        //asm volatile("" ::: "memory");  // Prevent compiler reordering
        r1 = Y;
 
        sem_post(&endSema);  // Notify transaction complete
    }
    return NULL;  // Never returns
};
 
void *thread2Func(void *param) {  // Initialize random number generator
    for (;;)                      // Loop indefinitely
    {
        sem_wait(&beginSema2);  // Wait for signal from main thread
        while (rand() % 8 != 0) {
        }  // Add a short, random delay
 
        // ----- THE TRANSACTION! -----
        __atomic_store_n(&Y, 1, __ATOMIC_SEQ_CST);
 
        //asm volatile("" ::: "memory");  // Prevent compiler reordering
        r2 = X;
 
        sem_post(&endSema);  // Notify transaction complete
    }
    return NULL;  // Never returns
};
 
int main(void) {
    // Initialize the semaphores
    sem_init(&beginSema1, 0, 0);
    sem_init(&beginSema2, 0, 0);
    sem_init(&endSema, 0, 0);
 
    // Spawn the threads
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread1Func, NULL);
    pthread_create(&thread2, NULL, thread2Func, NULL);
 
    // Repeat the experiment ad infinitum
    int detected = 0;
    int iterations;
    for ( iterations = 1; ; iterations++)
    {
        // Reset X and Y
        X = 0;
        Y = 0;
        // Signal both threads
        sem_post(&beginSema1);
        sem_post(&beginSema2);
        // Wait for both threads
        sem_wait(&endSema);
        sem_wait(&endSema);
        // Check if there was a simultaneous reorder
        if (r1 == 0 && r2 == 0)
        {
            detected++;
            printf("%d reorders detected after %d iterations\n", detected, iterations);
        }
        //printf("%d",iterations);
    }
    return 0;  // Never returns

    }

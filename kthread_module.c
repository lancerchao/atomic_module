
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/random.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lance Chao");
MODULE_DESCRIPTION("kthreads");

#define N 4
#define NUM_ITER 1000000
struct semaphore beginSema1, beginSema2, endSema;

int X, Y;
int r1, r2;

static struct task_struct *tids[N];
static bool done[N] = {false};

static int thread1_routine(void *data) 
{
    while (!kthread_should_stop())
    {
        down(&beginSema1);  // Wait for signal from main thread
        while (get_random_int() % 8 != 0) {
        }  // Add a short, random delay
 
        // ----- THE TRANSACTION! -----
        __atomic_store_n(&X, 1, __ATOMIC_SEQ_CST);
        //asm volatile("" ::: "memory");  // Prevent compiler reordering
        r1 = Y;
 
        up(&endSema);  // Notify transaction complete
    }
    return 0; 
}

static int thread2_routine(void *data) 
{
    while (!kthread_should_stop())
    {
        down(&beginSema2);  // Wait for signal from main thread
        while (get_random_int() % 8 != 0) {
        }  // Add a short, random delay
 
        // ----- THE TRANSACTION! -----
        __atomic_store_n(&Y, 1, __ATOMIC_SEQ_CST);
        //asm volatile("" ::: "memory");  // Prevent compiler reordering
        r2 = X;
 
        up(&endSema);  // Notify transaction complete
    }
    return 0;  // Never returns
}
/*
static void startThreads(void) {
    long long i;

    for (i = 0; i < N; i++) {
        tids[i] = kthread_run(thread_routine, (void *)i, "spawn");
    }
}
static bool join(void) {
    int i;

    for (i = 0; i < N; i++) {
        if (done[i] == false) return false;
    }
    return true;
}
*/
int init_module(void) 
{
    int detected;
    int iterations;

    printk(KERN_INFO "Starting kthread_module\n"); 
    // Initialize the semaphores
    sema_init(&beginSema1, 0);
    sema_init(&beginSema2, 0);
    sema_init(&endSema, 0);
 

    // Spawn the threads
    tids[0] = kthread_run(thread1_routine, NULL, "spawn");
    tids[1] = kthread_run(thread2_routine, NULL, "spawn");

 
    // Repeat the experiment ad infinitum
    detected = 0;
    for (iterations = 1; iterations < NUM_ITER; iterations++)
    {
        // Reset X and Y
        X = 0;
        Y = 0;
        // Signal both threads
        up(&beginSema1);
        up(&beginSema2);
        // Wait for both threads
        down(&endSema);
        down(&endSema);
        // Check if there was a simultaneous reorder
        if (r1 == 0 && r2 == 0)
        {
            detected++;
            printk(KERN_INFO "%d reorders detected after %d iterations\n", detected, iterations);
        }
        printk(KERN_INFO "%d iterations\n");
    }
    kthread_stop(tids[0]);
    kthread_stop(tids[1]);
    return 0;  // Never returns

}

void cleanup_module(void)
{ 
    printk(KERN_INFO "Stopping kthread_module\n"); 
}


#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/random.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lance Chao");
MODULE_DESCRIPTION("kthreads");

#define N 2
#define NUM_ITER 1000000
#define MEMORDER __ATOMIC_SEQ_CST
struct semaphore beginSema1, beginSema2, endSema;

#ifdef BUILTIN
int X, Y;
#else
atomic_t X, Y;
#endif
int r1, r2;
bool done;

static struct task_struct *tids[N];

static int thread1_routine(void *data) 
{
    while (!done)
    {
        down(&beginSema1);  // Wait for signal from main thread
        while (get_random_int() % 8 != 0) {
        }  // Add a short, random delay
 
        // ----- THE TRANSACTION! -----

#ifdef BUILTIN
        __atomic_store_n(&X, 1, MEMORDER);
        r1 = __atomic_load_n(&Y, MEMORDER);
#else
        atomic_set(&X, 1);
        asm volatile("" ::: "memory");  // Prevent compiler reordering
        r1 = atomic_read(&Y);
#endif 
        up(&endSema);  // Notify transaction complete
    }

    while(!kthread_should_stop()){}
    return 0; 
}

static int thread2_routine(void *data) 
{
    while (!done)
    {
        down(&beginSema2);  // Wait for signal from main thread
        while (get_random_int() % 8 != 0) {
        }  // Add a short, random delay
 
        // ----- THE TRANSACTION! -----

#ifdef BUILTIN
        __atomic_store_n(&Y, 1, MEMORDER);
        r2 = __atomic_load_n(&X, MEMORDER);
#else
        atomic_set(&Y, 1);
        asm volatile("" ::: "memory");  // Prevent compiler reordering
        r2 = atomic_read(&X);
#endif 

        up(&endSema);  // Notify transaction complete
    }
    while(!kthread_should_stop()){}
    return 0;  // Never returns
}

int init_module(void) 
{
    int detected;
    int iterations;

    printk(KERN_INFO "Starting kthread_module\n"); 
    // Initialize the semaphores
    sema_init(&beginSema1, 0);
    sema_init(&beginSema2, 0);
    sema_init(&endSema, 0);
 
    done = false;

    // Spawn the threads
    tids[0] = kthread_run(thread1_routine, NULL, "spawn");
    tids[1] = kthread_run(thread2_routine, NULL, "spawn");

    // Repeat the experiment ad infinitum
    detected = 0;
    for (iterations = 1; iterations < NUM_ITER; iterations++)
    {
        // Reset X and Y

#ifdef BUILTIN
        X = 0;
        Y = 0;
#else
        atomic_set(&X, 0);
        atomic_set(&Y, 0);
#endif

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

        if (iterations % 10000 == 0)
            printk(KERN_INFO "%d iterations\n", iterations);
    }
    return 0;  // Never returns

}

void cleanup_module(void)
{ 
    printk(KERN_INFO "Stopping kthread_module\n"); 
    done = true;
    up(&beginSema1);
    up(&beginSema2);
    kthread_stop(tids[0]);
    kthread_stop(tids[1]);
}

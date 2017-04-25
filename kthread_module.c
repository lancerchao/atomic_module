
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lance Chao");
MODULE_DESCRIPTION("kthreads");

#define N 4

static struct task_struct *tids[N];
static bool done[N] = {false};

/* Load, wait 6 seconds, unload */
/* kthread_stop signals kthread_should_stop to exit the function */
/* A flag is needed to prevent kthread_stop from executing if the function
   has already completed - otherwise a kernel NULL reference will occur */
static int thread_routine(void *data) {
    int i;
    long long tid;

    tid = (long long)(data);
    for (i = 0; i < 10; i++) {
        printk("Thread %lld ping\n", tid);
        msleep(100);
    }
    done[tid] = true;
    return 0;
}

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
int init_module(void) {
    /* Start two kernel threads */
    printk(KERN_INFO "Starting...");
    startThreads();
    while (!join()) {
    }
    return 0;
}

void cleanup_module(void) { printk(KERN_INFO "Module unload successful\n"); }

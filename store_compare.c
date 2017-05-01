#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/time.h>

struct timeval start, end;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John Kalin");
MODULE_DESCRIPTION("Test performance impact of atomic stores");

#define MMODEL __ATOMIC_SEQ_CST

int i = 0;

static int __init atomic_module_init(void) {
#ifdef BUILTIN
    int store_loc;
    do_gettimeofday(&start);
    for (i=0; i<1000000000; i++) {
        __atomic_store_n(&store_loc, i, MMODEL);
    }
    do_gettimeofday(&end);
#else
    atomic_t store_loc;
    do_gettimeofday(&start);
    for (i=0; i<1000000000; i++) {
        atomic_set(&store_loc, i);
    }
    do_gettimeofday(&end);
#endif
    printk("start %ld.%06ld s\nend %ld.%06ld s\n", start.tv_sec, start.tv_usec, end.tv_sec, end.tv_usec);
    return 0;  // Non-zero return means that the module couldn't be loaded.
}

static void __exit atomic_module_cleanup(void) { printk(KERN_INFO "Cleaning up module.\n"); }

module_init(atomic_module_init);
module_exit(atomic_module_cleanup);

#include <linux/module.h>  
#include <linux/kernel.h> 
#include <linux/init.h>  

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lance Chao");
MODULE_DESCRIPTION("Test Kernel and GCC atomics");

void test_atomic_add_fetch(void)
{

#ifdef BUILTIN
    int counter = 0;
    __atomic_add_fetch(&counter, 1, __ATOMIC_SEQ_CST);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_add(1, &counter);
#endif
    printk(KERN_INFO "Counter value is %d\n", counter);

    	    
}
static int __init atomic_module_init(void)
{
    printk(KERN_INFO "Hello world!\n");
    test_atomic_add_fetch();
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit atomic_module_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(atomic_module_init);
module_exit(atomic_module_cleanup);

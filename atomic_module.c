#include <linux/module.h>  
#include <linux/kernel.h> 
#include <linux/init.h>  

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lance Chao");
MODULE_DESCRIPTION("Test Kernel and GCC atomics");

#ifdef BUILTIN
static int counter_gcc = 0;
#else
static atomic_t counter_kernel = ATOMIC_INIT(0);
#endif

static int __init atomic_module_init(void)
{
    printk(KERN_INFO "Hello world!\n");
#ifdef BUILTIN
    __atomic_add_fetch(&counter_gcc, 1, __ATOMIC_RELAXED);
    printk(KERN_INFO "Counter_gcc value is %d\n", __atomic_load_n(&counter_gcc, __ATOMIC_RELAXED));
#else
    atomic_add(1, &counter_kernel);
    printk(KERN_INFO "Counter_kernel value is %d\n", atomic_read(&counter_kernel)); 
#endif
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit atomic_module_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(atomic_module_init);
module_exit(atomic_module_cleanup);

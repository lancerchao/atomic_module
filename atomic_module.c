#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lance Chao");
MODULE_DESCRIPTION("Test Kernel and GCC atomics");

void test_atomic_load_n(void) 
{
#ifdef BUILTIN
    int load_val = 99;
    int loaded = __atomic_load_n(&load_val, __ATOMIC_SEQ_CST);
    WARN_ON(loaded != load_val);
#else
    atomic_t load_val = ATOMIC_INIT(99);
    int loaded = atomic_read(&load_val);
    WARN_ON(load_val.counter != loaded);
#endif
}

void test_atomic_store_n(void) 
{
#ifdef BUILTIN
    int store_val = 99;
    int store_loc;
    __atomic_store_n(&store_loc, store_val, __ATOMIC_SEQ_CST);
    WARN_ON(store_loc != store_val);
#else
    atomic_t store_loc;
    int store_val = 99;
    atomic_set(&store_loc, store_val);
    WARN_ON(store_loc.counter != store_val);
#endif
}

void test_atomic_exchange_n(void)
{
#ifdef BUILTIN
    int xchg_loc = 49;
    int xchg_val = 99;
    int old = __atomic_exchange_n(&xchg_loc, xchg_val, __ATOMIC_SEQ_CST);
    WARN_ON(old != 49);
    WARN_ON(xchg_loc != xchg_val);
#else
    atomic_t xchg_loc = ATOMIC_INIT(49);
    int xchg_val = 99;
    int old = atomic_xchg(&xchg_loc, xchg_val);
    WARN_ON(old != 49);
    WARN_ON(xchg_loc.counter != xchg_val);
#endif

}
void test_atomic_compare_exchange_n(void)
{
#ifdef BUILTIN
    int xchg_loc = 49;
    int expected = 49;
    int desired = 99;
    bool success = __atomic_compare_exchange_n(&xchg_loc, &expected, desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    WARN_ON(success != true);
    WARN_ON(xchg_loc != desired);
#else
    atomic_t xchg_loc = ATOMIC_INIT(49);
    int old = 49;
    int new = 99;
    atomic_cmpxchg(&xchg_loc, old, new);
    WARN_ON(xchg_loc.counter != new);
#endif
}
void test_atomic_add_fetch(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_add_fetch(&counter, 1, __ATOMIC_SEQ_CST);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_add(1, &counter);
#endif
    printk(KERN_INFO "Counter value is %d\n", counter);
}

void test_atomic_sub_fetch(void) {}

void test_atomic_fetch_and(void) {}
void test_atomic_fetch_xor(void) {}
void test_atomic_fetch_or(void) {}
void test_atomic_fetch_nand(void) {}
void test_atomic_test_and_set(void) {}
void test_atomic_clear(void) {}
void test_atomic_thread_fence(void) {}
void test_atomic_signal_fence(void) {}
void test_atomic_always_lock_free(void) {}
void test_atomic_is_lock_free(void) {}

static int __init atomic_module_init(void) {
    printk(KERN_INFO "Hello world!\n");
    test_atomic_add_fetch();
    test_atomic_load_n();
    return 0;  // Non-zero return means that the module couldn't be loaded.
}

static void __exit atomic_module_cleanup(void) { printk(KERN_INFO "Cleaning up module.\n"); }

module_init(atomic_module_init);
module_exit(atomic_module_cleanup);

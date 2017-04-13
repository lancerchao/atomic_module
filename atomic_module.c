#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lance Chao");
MODULE_DESCRIPTION("Test Kernel and GCC atomics");

#define assert(cond) \
    if (!(cond)) printk("%s:%d: Assertion (%s) failed.\n", __FILE__, __LINE__, #cond)
/*----------------------------------------------------------------*/
/*----operation_n ------------------------------------------------*/
/*----------------------------------------------------------------*/
void test_atomic_load_n(void) {
#ifdef BUILTIN
    int load_val = 99;
    int loaded = __atomic_load_n(&load_val, __ATOMIC_SEQ_CST);
    assert(loaded == load_val);
#else
    atomic_t load_val = ATOMIC_INIT(99);
    int loaded = atomic_read(&load_val);
    assert(load_val.counter == loaded);
#endif
}

void test_atomic_store_n(void) {
#ifdef BUILTIN
    int store_val = 99;
    int store_loc;
    __atomic_store_n(&store_loc, store_val, __ATOMIC_SEQ_CST);
    assert(store_loc == store_val);
#else
    atomic_t store_loc;
    int store_val = 99;
    atomic_set(&store_loc, store_val);
    assert(store_loc.counter == store_val);
#endif
}

void test_atomic_exchange_n(void) {
#ifdef BUILTIN
    int xchg_loc = 49;
    int xchg_val = 99;
    int old = __atomic_exchange_n(&xchg_loc, xchg_val, __ATOMIC_SEQ_CST);
    assert(old == 49);
    assert(xchg_loc == xchg_val);
#else
    atomic_t xchg_loc = ATOMIC_INIT(49);
    int xchg_val = 99;
    int old = atomic_xchg(&xchg_loc, xchg_val);
    assert(old == 49);
    assert(xchg_loc.counter == xchg_val);
#endif
}
void test_atomic_compare_exchange_n(void) {
#ifdef BUILTIN
    int xchg_loc = 49;
    int expected = 49;
    int desired = 99;
    bool success = __atomic_compare_exchange_n(&xchg_loc, &expected, desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    assert(success == true);
    assert(xchg_loc == desired);
#else
    atomic_t xchg_loc = ATOMIC_INIT(49);
    int old = 49;
    int new = 99;
    atomic_cmpxchg(&xchg_loc, old, new);
    assert(xchg_loc.counter == new);
#endif
}
/*----------------------------------------------------------------*/
/*----operation + fetch ------------------------------------------*/
/*----------------------------------------------------------------*/
void test_atomic_add_fetch(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_add_fetch(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_add(1, &counter);
    assert(counter.counter == 1);
#endif
}
void test_atomic_sub_fetch(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_sub_fetch(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == -1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_sub(1, &counter);
    assert(counter.counter == -1);
#endif
}
void test_atomic_and_fetch(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_and_fetch(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == 0);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_and(1, &counter);
    assert(counter.counter == 0);
#endif
}
void test_atomic_xor_fetch(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_xor_fetch(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_xor(1, &counter);
    assert(counter.counter == 1);
#endif
}
void test_atomic_or_fetch(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_or_fetch(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_or(1, &counter);
    assert(counter.counter == 1);
#endif
}
void test_atomic_nand_fetch(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_nand_fetch(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == -1);
#endif
}
/*----------------------------------------------------------------*/
/*----fetch + operation ------------------------------------------*/
/*----------------------------------------------------------------*/
void test_atomic_fetch_add(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_fetch_add(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_add(1, &counter);
    assert(counter.counter == 1);
#endif
}
void test_atomic_fetch_sub(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_fetch_sub(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == -1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_sub(1, &counter);
    assert(counter.counter == -1);
#endif
}
void test_atomic_fetch_and(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_fetch_and(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == 0);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_and(1, &counter);
    assert(counter.counter == 0);
#endif
}
void test_atomic_fetch_xor(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_fetch_xor(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_xor(1, &counter);
    assert(counter.counter == 1);
#endif
}
void test_atomic_fetch_or(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_fetch_or(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    atomic_or(1, &counter);
    assert(counter.counter == 1);
#endif
}
void test_atomic_fetch_nand(void) {
#ifdef BUILTIN
    int counter = 0;
    __atomic_fetch_nand(&counter, 1, __ATOMIC_SEQ_CST);
    assert(counter == -1);
#endif
}
/*----------------------------------------------------------------*/
/*----aux operations ---------------------------------------------*/
/*----------------------------------------------------------------*/
void test_atomic_test_and_set(void) {
#ifdef BUILTIN
    // Only works for booleans
    bool val1 = false, *ptr1 = &val1, rval1;
    bool val2 = true, *ptr2 = &val2, rval2;
    rval1 = __atomic_test_and_set(ptr1, __ATOMIC_SEQ_CST);
    rval2 = __atomic_test_and_set(ptr2, __ATOMIC_SEQ_CST);
    assert(rval1 == false && val1 == true);
    assert(rval2 == true && val2 == true);
#endif
}
void test_atomic_clear(void) {
#ifdef BUILTIN
    bool val1 = true, *ptr1 = &val1;
    bool val2 = true, *ptr2 = &val2;
    __atomic_clear(ptr1, __ATOMIC_SEQ_CST);
    __atomic_clear(ptr2, __ATOMIC_SEQ_CST);
    assert(val1 == false);
    assert(val2 == false);
#endif
}
void __attribute__((optimize("O0"))) test_atomic_thread_fence(void) {
#ifdef BUILTIN
    int store;
    int load;
    store = 42;
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    load = store;
    assert(load == store);
#endif
}
void __attribute__((optimize("O0"))) test_atomic_signal_fence(void) {
#ifdef BUILTIN
    int store;
    int load;
    store = 42;
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    load = store;
    assert(load == store);
#endif
}
void test_atomic_always_lock_free(void) {
#ifdef BUILTIN
    size_t size1 = sizeof(int);
    struct s {
        int x, y, z;
    };
    size_t size2 = sizeof(struct s);
    bool rval1, rval2;
    rval1 = __atomic_always_lock_free(size1, 0);
    rval2 = __atomic_always_lock_free(size2, 0);
    assert(rval1 == true);
    assert(rval2 == false);
#endif
}
void test_atomic_is_lock_free(void) {
#ifdef BUILTIN
    size_t size1 = sizeof(int);
    struct s {
        int x, y, z;
    };
    size_t size2 = sizeof(struct s);
    bool rval1, rval2;
    rval1 = __atomic_always_lock_free(size1, 0);
    rval2 = __atomic_always_lock_free(size2, 0);
    assert(rval1 == true);
    assert(rval2 == false);
#endif
}

static int __init atomic_module_init(void) {
    printk(KERN_INFO "Hello world!\n");
    test_atomic_add_fetch();
    printk("test_atomic_add_fetch: done\n");
    test_atomic_sub_fetch();
    printk("test_atomic_sub_fetch: done\n");
    test_atomic_and_fetch();
    printk("test_atomic_and_fetch: done\n");
    test_atomic_xor_fetch();
    printk("test_atomic_xor_fetch: done\n");
    test_atomic_or_fetch();
    printk("test_atomic_or_fetch: done\n");
    test_atomic_nand_fetch();
    printk("test_atomic_nand_fetch: done\n");
    test_atomic_fetch_add();
    printk("test_atomic_fetch_add: done\n");
    test_atomic_fetch_sub();
    printk("test_atomic_fetch_sub: done\n");
    test_atomic_fetch_and();
    printk("test_atomic_fetch_and: done\n");
    test_atomic_fetch_xor();
    printk("test_atomic_fetch_xor: done\n");
    test_atomic_fetch_or();
    printk("test_atomic_fetch_or: done\n");
    test_atomic_fetch_nand();
    printk("test_atomic_fetch_nand: done\n");
    test_atomic_test_and_set();
    printk("test_atomic_test_and_set: done\n");
    test_atomic_clear();
    printk("test_atomic_clear: done\n");
    test_atomic_thread_fence();
    printk("test_atomic_thread_fence: done\n");
    test_atomic_signal_fence();
    printk("test_atomic_signal_fence: done\n");
    test_atomic_always_lock_free();
    printk("test_atomic_always_lock_free: done\n");
    test_atomic_is_lock_free();
    printk("test_atomic_is_lock_free: done\n");
    // test_atomic_test_and_set();
    return 0;  // Non-zero return means that the module couldn't be loaded.
}

static void __exit atomic_module_cleanup(void) { printk(KERN_INFO "Cleaning up module.\n"); }

module_init(atomic_module_init);
module_exit(atomic_module_cleanup);

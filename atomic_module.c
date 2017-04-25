#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lance Chao");
MODULE_DESCRIPTION("Test Kernel and GCC atomics");

#define MMODEL __ATOMIC_SEQ_CST
#define NOOP asm("nop")

#define assert(cond) \
    if (!(cond)) printk("%s:%d: Assertion (%s) failed.\n", __FILE__, __LINE__, #cond)
/*----------------------------------------------------------------*/
/*----operation_n ------------------------------------------------*/
/*----------------------------------------------------------------*/
void __attribute__((optimize("O1"))) test_atomic_load_n(void) {
#ifdef BUILTIN
    volatile int load_val = 99, loaded;
    NOOP;
    loaded = __atomic_load_n(&load_val, MMODEL);
    NOOP;
    assert(loaded == load_val);
#else
    atomic_t load_val = ATOMIC_INIT(99);
    volatile int loaded = atomic_read(&load_val);
    assert(load_val.counter == loaded);
#endif
}

void __attribute__((optimize("O1"))) test_atomic_store_n(void) {
#ifdef BUILTIN
    volatile int store_val = 99;
    volatile int store_loc;
    NOOP;
    __atomic_store_n(&store_loc, store_val, MMODEL);
    NOOP;
    assert(store_loc == store_val);
#else
    atomic_t store_loc;
    volatile int store_val = 99;
    atomic_set(&store_loc, store_val);
    assert(store_loc.counter == store_val);
#endif
}

void __attribute__((optimize("O1"))) test_atomic_exchange_n(void) {
#ifdef BUILTIN
    volatile int xchg_loc = 49;
    volatile int xchg_val = 99;
    volatile int old;
    NOOP;
    old = __atomic_exchange_n(&xchg_loc, xchg_val, MMODEL);
    NOOP;
    assert(old == 49);
    assert(xchg_loc == xchg_val);
#else
    atomic_t xchg_loc = ATOMIC_INIT(49);
    volatile int xchg_val = 99;
    volatile int old = atomic_xchg(&xchg_loc, xchg_val);
    assert(old == 49);
    assert(xchg_loc.counter == xchg_val);
#endif
}
void __attribute__((optimize("O1"))) test_atomic_compare_exchange_n(void) {
#ifdef BUILTIN
    volatile int xchg_loc = 49;
    int expected = 49;
    int desired = 99;
    bool success;
    NOOP;
    success = __atomic_compare_exchange_n(&xchg_loc, &expected, desired, false, MMODEL, MMODEL);
    NOOP;
    assert(success == true);
    assert(xchg_loc == desired);
#else
    atomic_t xchg_loc = ATOMIC_INIT(49);
    volatile int old = 49;
    volatile int new = 99;
    atomic_cmpxchg(&xchg_loc, old, new);
    assert(xchg_loc.counter == new);
#endif
}
/*----------------------------------------------------------------*/
/*----operation + fetch ------------------------------------------*/
/*----------------------------------------------------------------*/
void __attribute__((optimize("O1"))) test_atomic_add_fetch(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_add_fetch(&counter, 1, MMODEL);
    NOOP;
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    NOOP;
    atomic_add(1, &counter);
    NOOP;
    assert(counter.counter == 1);
#endif
}
void __attribute__((optimize("O1"))) test_atomic_sub_fetch(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_sub_fetch(&counter, 1, MMODEL);
    NOOP;
    assert(counter == -1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    NOOP;
    atomic_sub(1, &counter);
    NOOP;
    assert(counter.counter == -1);
#endif
}
void __attribute__((optimize("O1"))) test_atomic_and_fetch(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_and_fetch(&counter, 1, MMODEL);
    NOOP;
    assert(counter == 0);
#else
    atomic_t counter = ATOMIC_INIT(0);
    NOOP;
    atomic_and(1, &counter);
    NOOP;
    assert(counter.counter == 0);
#endif
}
void __attribute__((optimize("O1"))) test_atomic_xor_fetch(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_xor_fetch(&counter, 1, MMODEL);
    NOOP;
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    NOOP;
    atomic_xor(1, &counter);
    NOOP;
    assert(counter.counter == 1);
#endif
}
void __attribute__((optimize("O1"))) test_atomic_or_fetch(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_or_fetch(&counter, 1, MMODEL);
    NOOP;
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    NOOP;
    atomic_or(1, &counter);
    NOOP;
    assert(counter.counter == 1);
#endif
}
void __attribute__((optimize("O1"))) test_atomic_nand_fetch(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_nand_fetch(&counter, 1, MMODEL);
    NOOP;
    assert(counter == -1);
#endif
}
/*----------------------------------------------------------------*/
/*----fetch + operation ------------------------------------------*/
/*----------------------------------------------------------------*/
void __attribute__((optimize("O1"))) test_atomic_fetch_add(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_fetch_add(&counter, 1, MMODEL);
    NOOP;
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    NOOP;
    atomic_add(1, &counter);
    NOOP;
    assert(counter.counter == 1);
#endif
}
void __attribute__((optimize("O1")))  test_atomic_fetch_sub(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_fetch_sub(&counter, 1, MMODEL);
    NOOP;
    assert(counter == -1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    NOOP;
    atomic_sub(1, &counter);
    NOOP;
    assert(counter.counter == -1);
#endif
}
void __attribute__((optimize("O1")))  test_atomic_fetch_and(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_fetch_and(&counter, 1, MMODEL);
    NOOP;
    assert(counter == 0);
#else
    atomic_t counter = ATOMIC_INIT(0);
    NOOP;
    atomic_and(1, &counter);
    NOOP;
    assert(counter.counter == 0);
#endif
}
void __attribute__((optimize("O1")))  test_atomic_fetch_xor(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_fetch_xor(&counter, 1, MMODEL);
    NOOP;
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    NOOP;
    atomic_xor(1, &counter);
    NOOP;
    assert(counter.counter == 1);
#endif
}
void __attribute__((optimize("O1")))  test_atomic_fetch_or(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_fetch_or(&counter, 1, MMODEL);
    NOOP;
    assert(counter == 1);
#else
    atomic_t counter = ATOMIC_INIT(0);
    NOOP;
    atomic_or(1, &counter);
    NOOP;
    assert(counter.counter == 1);
#endif
}
void __attribute__((optimize("O1")))  test_atomic_fetch_nand(void) {
#ifdef BUILTIN
    volatile int counter = 0;
    NOOP;
    __atomic_fetch_nand(&counter, 1, MMODEL);
    NOOP;
    assert(counter == -1);
#endif
}
/*----------------------------------------------------------------*/
/*----aux operations ---------------------------------------------*/
/*----------------------------------------------------------------*/
void __attribute__((optimize("O1"))) test_atomic_test_and_set(void) {
#ifdef BUILTIN
    // Only works for booleans
    bool val1 = false, *ptr1 = &val1, rval1;
    bool val2 = true, *ptr2 = &val2, rval2;
    NOOP;
    rval1 = __atomic_test_and_set(ptr1, MMODEL);
    NOOP;
    NOOP;
    rval2 = __atomic_test_and_set(ptr2, MMODEL);
    NOOP;
    assert(rval1 == false && val1 == true);
    assert(rval2 == true && val2 == true);
#endif
}
void __attribute__((optimize("O1"))) test_atomic_clear(void) {
#ifdef BUILTIN
    bool val1 = true, *ptr1 = &val1;
    bool val2 = false, *ptr2 = &val2;
    NOOP;
    __atomic_clear(ptr1, MMODEL);
    NOOP;
    NOOP;
    __atomic_clear(ptr2, MMODEL);
    NOOP;
    assert(val1 == false);
    assert(val2 == false);
#endif
}
void __attribute__((optimize("O1"))) test_atomic_thread_fence(void) {
#ifdef BUILTIN
    volatile int store;
    volatile int load;
    store = 42;
    NOOP;
    __atomic_thread_fence(MMODEL);
    NOOP;
    load = store;
    assert(load == store);
#endif
}
void __attribute__((optimize("O1"))) test_atomic_signal_fence(void) {
#ifdef BUILTIN
    volatile int store;
    volatile int load;
    store = 42;
    NOOP;
    __atomic_signal_fence(MMODEL);
    NOOP;
    load = store;
    assert(load == store);
#endif
}
void test_atomic_always_lock_free(void) {
#ifdef BUILTIN
    struct s {
        volatile int x, y, z;
    };
    bool rval1, rval2;
    NOOP;
    rval1 = __atomic_always_lock_free(sizeof(int), 0);
    NOOP;
    NOOP;
    rval2 = __atomic_always_lock_free(sizeof(struct s), 0);
    NOOP;
    assert(rval1 == true);
    assert(rval2 == false);
#endif
}
void test_atomic_is_lock_free(void) {
#ifdef BUILTIN
    struct s {
        volatile int x, y, z;
    };
    bool rval1, rval2;
    NOOP;
    rval1 = __atomic_always_lock_free(sizeof(int), 0);
    NOOP;
    NOOP;
    rval2 = __atomic_always_lock_free(sizeof(struct s), 0);
    NOOP;
    assert(rval1 == true);
    assert(rval2 == false);
#endif
}

/* assembly code to read the TSC */
static inline uint64_t RDTSC(void)
{
  unsigned int hi, lo;
  __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}

static int __init atomic_module_init(void) {

	uint64_t start_counter;
    uint64_t end_counter;
    printk(KERN_INFO "Hello world!\n");
    start_counter = RDTSC();
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
    end_counter = RDTSC();
    printk("Start is %llu, end is %llu, diff is %llu\n", start_counter, end_counter, end_counter - start_counter);
    // test_atomic_test_and_set();
    return 0;  // Non-zero return means that the module couldn't be loaded.
}

static void __exit atomic_module_cleanup(void) { printk(KERN_INFO "Cleaning up module.\n"); }

module_init(atomic_module_init);
module_exit(atomic_module_cleanup);

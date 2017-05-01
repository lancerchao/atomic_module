obj-m += atomic_module.o kthread_module.o store_compare.o
CFLAGS_atomic_module.o := -O2 -g -fno-inline -DBUILTINS
CFLAGS_kthread_module.o := -O2 -g -fno-inline -DBUILTINS
CFLAGS_store_compare.o := -O2 -g -fno-inline -DBUILTINS

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


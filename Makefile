obj-m += atomic_module.o
CFLAGS_atomic_module.o := -O2 -fno-inline

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


#!/bin/bash
sudo insmod atomic_module.ko; sudo rmmod atomic_module; dmesg
objdump -d atomic_module.ko >dump.txt
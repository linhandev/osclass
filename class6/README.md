umame -r to check linux kernel version <br>
uname -a check the current kernel running <br>
mhwd-kernel -li 看当前所有的和正在跑的kernel <br>
最低的能跑的linux3看起来也没有asm，他这个代码看起来事linux2写的<br>

ah chinese input chrashed from here<br>
so i followed a tutorial on this link http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/<br>

so there are basically 2 kinds of kernel models, one is built in, compiled with the system. Obviously not straight-forward and kernel would be really huge supporting 10000 types of printers. So the loadable kernel model LKM is dynamic, can be loaded and unloaded at run time .LKM have no support for interruputs while kernel code has.<br>

An obvious difference from kernel code and user code is that it runs in kernel space, which has a difference memory address space from the user space. Code meant for user program is not usable in kernel model. Like dont try to printf, but they do have a printk. <br>

System prep include installing kernel headers which typically is in /usr/src but happens to be somewhere else on my manjaro arch. Anyway just turn to ur package manager and try to find the header package for ur kernel version. <br>

uname -a to check current running kernel version,
an apt example
```
molloyd@DebianJessieVM:~$ sudo apt-get update
molloyd@DebianJessieVM:~$ apt-cache search linux-headers-$(uname -r)
linux-headers-3.16.0-4-amd64 - Header files for Linux 3.16.0-4-amd64
molloyd@DebianJessieVM:~$ sudo apt-get install linux-headers-3.16.0-4-amd64
molloyd@DebianJessieVM:~$ cd /usr/src/linux-headers-3.16.0-4-amd64/
molloyd@DebianJessieVM:/usr/src/linux-headers-3.16.0-4-amd64$ ls
arch  include  Makefile  Module.symvers  scripts
```
<br>and note that this is not kernel source!!!<br>

If things go south, kernel can be corrupted. Most cases a reboot will restore everything, but prep for something worse.<br>

Avoid using global variabels cuz they are shared with all kernels and may get things messy.<br>

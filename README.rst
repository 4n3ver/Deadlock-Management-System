==========================
Deadlock Management System
==========================
:Info: A mutex syscall that prevents deadlock from occurring
:Authors: William Morlan, Christopher Wang, Yoel Ivan

To start, all commands are in quotes the commands do not need the quotes:

1) run "make lib" or "make all"

2) run "export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH"

3) run "./bestoffer"

4) The program should stall after printing some output.  If anything else happens, seek help.

Installing System Call into the Kernel
======================================

- Edit the *syscall_64.tbl*

    ``$ cd /arch/x86/syscalls``
    
- Add following entry to the end of 64-bit syscalls list, adjust the syscalls number accordingly::

    318 common  hello           sys_hello
    319 common  loltex_init     sys_loltex_init
    320 common  loltex_lock     sys_loltex_lock
    321 common  loltex_unlock   sys_loltex_unlock
    
- Edit the *syscalls.h*

    ``$ cd /include/linux/``
    
- Add following entry just before the *#endif*::

    asmlinkage long sys_hello(long test);
    asmlinkage long sys_loltex_init(int index);
    asmlinkage long sys_loltex_lock(long process_id, int mutex_id);
    asmlinkage long sys_loltex_unlock(int mutex_id);
    
- Create new folder */hello*

    ``$ mkdir hello``
    
- Place *hello.c* to */hello* folder

- Create *Makefile* on */hello* folder

    ``$ echo 'obj-y := hello.o' >> Makefile``
    
- Edit *Makefile* of the root directory, and append at the end of the line 'hello/' (line 771 on ours)

    ``core-y        += kernel/ mm/ fs/ ipc/ security/ crypto/ block/ hello/``

- Run following command, a window will pop-up, select save
  
    ``$ make menuconfig``
    
- Compile the kernel ``X`` is the number of processor available on your system

    ``$ make -jX``





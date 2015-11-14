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

    319 common  loltex_init     sys_loltex_init
    320 common  loltex_lock     sys_loltex_lock
    321 common  loltex_unlock   sys_loltex_unlock
    
- Edit the *syscalls.h*

    ``$ cd /include/linux/``
    
- Add following entry just before the *#endif*::

    asmlinkage long sys_loltex_init(int index);
    asmlinkage long sys_loltex_lock(long process_id, int mutex_id);
    asmlinkage long sys_loltex_unlock(int mutex_id);
    
- Create new folder */loltex*

    ``$ mkdir loltex``
    
- Place *loltex.c* to */loltex* folder

- Create *Makefile* on */loltex* folder

    ``$ echo 'obj-y := loltex.o' >> Makefile``
    
- Edit *Makefile* of the root directory, and append at the end of the line 'loltex/' (line 771 on ours)

    ``core-y        += kernel/ mm/ fs/ ipc/ security/ crypto/ block/ loltex/``

- Run following command, a window will pop-up, select save
  
    ``$ make menuconfig``
    
- Compile the kernel ``X`` is the number of processor available on your system

    ``$ make -jX``

- Compile the kernel ``X`` is the number of processor available on your system and ``arbitrary-name`` is whatever you want

    ``$ make -j X KDEB_PKGVERSION=1.arbitrary-name deb-pkg``

- Install the kernel (which should be in your home directory)

    ``$ dpkg -i linux*.deb``

- Reboot into the kernel and run these commands to check the kernel and if the sys call was included

    ``$ uname -r``

    ``$ cat /proc/kallsyms | grep loltex_init``

    
Compile/Run Test Programs
=========================

- use *gcc* to compile the *\*.c* files::
    
    $ gcc class_thread.o -pthread -o class_thread_test test_class_thread.c
    $ gcc -pthread -o pthread_test test_pthread.c
    
- run the test programs::

    $ ./class_thread_test
    $ ./pthread_test



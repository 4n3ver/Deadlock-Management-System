#include <linux/kernel.h>
#include <linux/mutex.h>

#define NUM_MUTEXES 2

static struct mutex mutexes[NUM_MUTEXES];
static long mutex_holders[NUM_MUTEXES] = {0};

asmlinkage long sys_hello(long test)
{
        printk("Hello world\n");
        return test;
}

asmlinkage long sys_loltex_init(int index)
{
    printk("sys_loltex_init %d\n", index);
    mutex_init(&mutexes[index]);
    return 0;
}

asmlinkage long sys_loltex_lock(long process_id, int mutex_id) {
	printk("sys_loltex_lock %ld %d\n", process_id, mutex_id);
	printk("mutex_holders %ld %ld\n", mutex_holders[0], mutex_holders[1]);
	int i;
	if (mutex_holders[mutex_id] == 0) {
		printk("%ld wait for %d\n", process_id, mutex_id);
		mutex_lock(&mutexes[mutex_id]);
		printk("%ld got %d\n", process_id, mutex_id);
		mutex_holders[mutex_id] = process_id;
	} else {
		if (mutex_holders[mutex_id] > process_id) {
			printk("releasing all and trying to get back\n");
			bool held_by_me[NUM_MUTEXES];
			for (i=0; i < NUM_MUTEXES; i++) {
				if (process_id == mutex_holders[i]) {
					held_by_me[i] = true;
					printk("%ld unlock %d deadlock\n", process_id, i);
					mutex_unlock(&mutexes[i]);	
				} else {
					held_by_me[i] = false;
				}
			}

			printk("%ld wait for %d\n", process_id, mutex_id);
			mutex_lock(&mutexes[mutex_id]);
			printk("%ld got %d\n", process_id, mutex_id);

			mutex_holders[mutex_id] = process_id;
			for (i=0; i < NUM_MUTEXES; i++) {
				if (held_by_me[NUM_MUTEXES]) {
					printk("%ld wait for %d\n", process_id, i);
					mutex_lock(&mutexes[i]);
					printk("%ld got %d\n", process_id, i);
					mutex_holders[i] = process_id;
				}
			}
		} else {
			printk("continue waiting\n");
			printk("%ld wait for %d\n", process_id, mutex_id);
			mutex_lock(&mutexes[mutex_id]);
			printk("%ld got %d\n", process_id, mutex_id);
		}
	}
	return 0;
}

asmlinkage long sys_loltex_unlock(int mutex_id) {
	printk("sys_loltex_unlock %d\n", mutex_id);
	printk("mutex_holders %ld %ld\n", mutex_holders[0], mutex_holders[1]);
	mutex_holders[mutex_id] = 0;
	mutex_unlock(&mutexes[mutex_id]);	
	return 0;
}

#include "class_thread.h"
#include <pthread.h>

int mutex_id = 0;
#define LOLTEX_INIT 319
#define LOLTEX_LOCK 320
#define LOLTEX_UNLOCK 321

int allocate_mutex(class_mutex_t *cmutex)
{
  return 0;
}

int allocate_cond(class_condit_ptr ccondit)
{
  if(!ccondit->condition)
  {
    ccondit->condition = malloc(sizeof(pthread_cond_t));
    
    if(!ccondit->condition)
    {
      fprintf(stderr, "Error: malloc failed to allocate space for condition var!\n");
      return -1;
    }
  }
}


int class_mutex_init(class_mutex_ptr cmutex)
{

  /*if(pthread_mutex_init(&(cmutex->mutex), NULL))
  {
    fprintf(stderr, "Error: pthread mutex initialization failed!\n");
    return -1;
  }*/
  cmutex->id = mutex_id++;
  long int amma = syscall(LOLTEX_INIT, cmutex->id);
  printf("mutex init ok with mutex_id of %d\n", cmutex->id);
  return 0;
}

int class_mutex_destroy(class_mutex_ptr cmutex)
{
  if(pthread_mutex_destroy(&cmutex->mutex))
  {
    fprintf(stderr, "Error: pthread mutex destruction failed!\n");
    return -1;
  }

  return 0;
}

int class_cond_init(class_condit_ptr ccondit)
{

  if(pthread_cond_init(ccondit->condition, NULL))
  {
    fprintf(stderr, "Error: pthread condition initialization failed!\n");
    return -1;
  }

  return 0;
}

int class_cond_destroy(class_condit_ptr ccondit)
{
  if(pthread_cond_destroy(ccondit->condition))
  {
    fprintf(stderr, "Error: pthread condition destruction failed!\n");
    return -1;
  }

  return 0;
}


int class_mutex_lock(class_mutex_ptr cmutex)
{
  pid_t pid;
  //pid = gettid();
  //printf("%ld\n", (long)pid);
  pthread_t pthread;
  pthread = pthread_self();
  printf("%ld\n", (long)pthread);
  // if(pthread_mutex_lock(&cmutex->mutex))
  // {
  //   fprintf(stderr, "Error: pthread mutex lock failed!\n");
  //   return -1;
  // }
  long int amma = syscall(LOLTEX_LOCK, (long)pthread, cmutex->id);

  return 0;
}


int class_mutex_unlock(class_mutex_ptr cmutex)
{
  // if(pthread_mutex_unlock(&cmutex->mutex))
  // {
  //   fprintf(stderr, "Error: pthread mutex unlock failed!\n");
  //   return -1;
  // }
  long int amma = syscall(LOLTEX_UNLOCK, cmutex->id);
  
  return 0;
}


int class_thread_create(class_thread_t * cthread, void *(*start)(void *), void * arg, int * num_of_mutexes)
{
  pthread_t temp_pthread;
  if(pthread_create(&temp_pthread, NULL, start, arg))
  {
    fprintf(stderr, "Error: Failed to create pthread!\n");
    return -1;
  }

  //Hacking a bit to get everything working correctly
  memcpy(cthread, &temp_pthread, sizeof(pthread_t));

  return 0;
}
  
int class_thread_join(class_thread_t cthread, void ** retval)
{
  pthread_t temp_pthread;
  memcpy(&temp_pthread, &cthread, sizeof(pthread_t));

  if(pthread_join(temp_pthread, retval))
  {
    fprintf(stderr, "Error: failed to join the pthread!\n");
    return -1;
  }

  return 0;
}

int class_thread_cond_wait(class_condit_ptr ccondit, class_mutex_ptr cmutex)
{

  if(pthread_cond_wait(ccondit->condition, &cmutex->mutex))
  {
    fprintf(stderr, "Error: pthread cond wait failed!\n");
    return -1;
  }

  return 0;
}

int class_thread_cond_signal(class_condit_ptr ccondit)
{
  
  if(pthread_cond_signal(ccondit->condition))
  {
    fprintf(stderr, "Error: pthread cond signal failed!\n");
    return -1;
  }

  return 0;
}


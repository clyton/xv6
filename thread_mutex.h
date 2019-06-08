#include "types.h"

struct thread_mutex {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
};


void            thread_mutex_lock(struct thread_mutex*);
int             thread_holding(struct thread_mutex*);
void            thread_mutex_init(struct thread_mutex*);
void            thread_mutex_unlock(struct thread_mutex*);

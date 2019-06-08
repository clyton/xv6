#include "types.h"
//  Mutual exclusion lock.
struct thread_spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
};


void            thread_spin_lock(struct thread_spinlock*);
int             thread_holding(struct thread_spinlock*);
void            thread_spin_init(struct thread_spinlock*);
void            thread_spin_unlock(struct thread_spinlock*);

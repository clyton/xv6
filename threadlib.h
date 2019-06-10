#ifndef __THREADLIB_H_
#define __THREADLIB_H_

# include "types.h"

#define MAX_T 5
#define DEFINE_PER_THREAD(type, name) type name[MAX_T]
#define PER_THREAD(name) name[gettid()]

struct tls {
  uint tid;
};
//  THREAD SPINLOCKS
struct thread_spinlock {
  uint locked;       // Is the lock held?
  char *name;        // Name of lock.
};


void            thread_spin_lock(struct thread_spinlock*);
int             thread_spin_holding(struct thread_spinlock*);
void            thread_spin_init(struct thread_spinlock*);
void            thread_spin_unlock(struct thread_spinlock*);

// THREAD MUTEXES
struct thread_mutex {
  uint locked;       // Is the lock held?
  char *name;        // Name of lock.
};


void            thread_mutex_lock(struct thread_mutex*);
int             thread_mutex_holding(struct thread_mutex*);
void            thread_mutex_init(struct thread_mutex*);
void            thread_mutex_unlock(struct thread_mutex*);

// THREAD CONDITIONAL VARIABLES
struct thread_cond {
  char name[16];
};


void   thread_cond_wait(struct thread_cond *cv, struct thread_mutex *m);
void   thread_cond_init(struct thread_cond *cv, const char* chan);
void   thread_cond_signal(struct thread_cond *cv);

// PER THREAD LOCAL VARIABLES
uint   gettid();

#endif // __THREADLIB_H_

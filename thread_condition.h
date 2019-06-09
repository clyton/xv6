#include "thread_mutex.h"

struct thread_cond {
  char name[16];
};


void   thread_cond_wait(struct thread_cond *cv, struct thread_mutex *m);
void   thread_cond_init(struct thread_cond *cv, const char* chan);
void   thread_cond_signal(struct thread_cond *cv);

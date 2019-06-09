#include "types.h"
#include "user.h"
#include "thread_condition.h"

void   thread_cond_wait(struct thread_cond *cv, struct thread_mutex *m) {

  printf(1, "cond wait\n");
  thread_wait(cv->name, &m->locked);
}
void   thread_cond_init(struct thread_cond *cv, const char *name) {
  printf(1,"cond init\n");
  strcpy(cv->name, name);
}
void   thread_cond_signal(struct thread_cond *cv) {
  printf(1, "cond signal\n");
  thread_signal(cv->name); // system call to wake up just like wakeup
}

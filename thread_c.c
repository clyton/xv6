#include "thread_condition.h"
#include "user.h"

struct queue {
   struct thread_cond cv;
   struct thread_mutex m;

   void *ptr;
};

struct queue q;

struct sendwork {
  struct queue *q;
  void *p;
};
// Initialize


// Thread 1 (sender)
void
send(struct queue *q, void *p)
{
   thread_mutex_lock(&q->m);
   while(q->ptr != 0);
   q->ptr = p;
   printf(1, "thread send : %s\n", (char*)q->ptr);
   thread_cond_signal(&q->cv);
   thread_mutex_unlock(&q->m);
}

// Thread 2 (receiver)

void*
recv(struct queue *q)
{
  void *p;

  thread_mutex_lock(&q->m);

  while((p = q->ptr) == 0)
    thread_cond_wait(&q->cv, &q->m);
  printf(1, "thread recevied: %s\n", (char*) q->ptr);
  q->ptr = 0;

  thread_mutex_unlock(&q->m);
  return p;
}

void
do_send_work(void *arg) {

  struct sendwork *swarg = (struct sendwork *) arg;
  send(swarg->q, swarg->p);
  thread_exit();

}

void
do_recv_work(void *arg) {

  struct queue *q1 = (struct queue*) arg;
  recv(q1);

  thread_exit();
}

int main()
{
  /* thread_cond_init(&q.cv); */
  /* thread_mutex_init(&q.m); */
  /* printf(1,"hello"); */
  char* name = "hello";
  int sendt, recvt, r1, r2;
  void *p = (void*) name;
  struct sendwork swarg = {&q, p};

  void *send_stack =  malloc(4096);
  void *recv_stack = malloc(4096);
  sendt = thread_create(do_send_work, (void *)&swarg, send_stack);
  recvt = thread_create(do_recv_work, (void *)&q, recv_stack);

  r1 = thread_join();
  r2 = thread_join();

  printf(1, "Threads finished: (%d):%d, (%d):%d\n", sendt, r1, recvt, r2);
  exit();
  return 0;
}

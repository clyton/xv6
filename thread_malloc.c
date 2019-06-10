#include "types.h"
#include "stat.h"
#include "user.h"
#include "threadlib.h"

struct balance {
    char name[32];
    int amount;
};

volatile int total_balance = 0;
struct thread_spinlock bal_lock;

volatile unsigned int delay (unsigned int d) {
   unsigned int i;
   for (i = 0; i < d; i++) {
       __asm volatile( "nop" ::: );
   }

   return i;
}

void do_work(void *arg){
    int i;
    int old;

    struct balance *b = (struct balance*) arg;
    printf(1, "Starting do_work: s:%s\n", b->name);

    for (i = 0; i < b->amount; i++) {
        thread_spin_lock(&bal_lock);
      /* acquire(&bal_lock); */
         old = total_balance;
         delay(100000);
         total_balance = old + 1;
      /* release(&bal_lock); */
         thread_spin_unlock(&bal_lock);
    }

    printf(1, "Done s:%s\n", b->name);

    thread_exit();
    return;
}

int main(int argc, char *argv[]) {

  struct balance b1 = {"b1", 3200};
  /* struct balance b2 = {"b2", 2800}; */
  thread_spin_init(&bal_lock);

  void *s1;

  int i = 0;

  for( i = 0; i < 5; i++) {
    s1 = thread_malloc(4096);
    thread_create(do_work, (void *)&b1, s1);
  }

  for( i = 0; i < 5; i++) {
    thread_join();
  }

  printf(1, "shared balance:%d\n", total_balance);

  exit();
}

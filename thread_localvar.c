
#include "types.h"
#include "stat.h"
#include "user.h"
#include "threadlib.h"
#define N 5

struct balance {
    char name[32];
    int amount;
};

struct balance bal_arr[N];

volatile unsigned int delay (unsigned int d) {
   unsigned int i;
   for (i = 0; i < d; i++) {
       __asm volatile( "nop" ::: );
   }

   return i;
}

void do_work(void *arg){
 int threadid = gettid();
 bal_arr[threadid].amount += threadid;
 thread_exit();
}

int main(int argc, char *argv[]) {


  void *s1;

  int i;
  for (i = 0; i < N; i++){
    bal_arr[i].amount = 10;
    bal_arr[i].name[0] = 'A' + i;
  }

  for (i = 0; i < N; i++){
    s1 = sbrk(4096);
    thread_create(do_work, 0, s1);
  }

  for (i = 0; i < N; i++){
    thread_join();
  }

  for (i = 0; i < N; i++){
    printf(1,"Threads finished\n");
    printf(1,"Thread ID = %d, Bal Amount = %d, Balance account name = %s\n", i, bal_arr[i].amount, bal_arr[i].name);
  }

  exit();
}

#include "thread_mutex.h"
#include "types.h"
#include "user.h"
#include "x86.h"

void
thread_mutex_init(struct thread_mutex *lk)
{
  lk->locked = 0;
}

// Acquire the lock.
// Loops (mutexs) until the lock is acquired.
// Thread_Holding a lock for a long time may cause
// other CPUs to waste time mutexning to acquire it.
void
thread_mutex_lock(struct thread_mutex *lk)
{
  /* thread_pushcli(); // disable interrupts to avoid deadlock. */
  if(thread_mutex_holding(lk))
    printf(2, "acquire\n");

  // The xchg is atomic.
  while(xchg(&lk->locked, 1) != 0) {
   sleep(1);
  }

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen after the lock is acquired.
  __sync_synchronize();

}

// Release the lock.
void
thread_mutex_unlock(struct thread_mutex *lk)
{
  if(!thread_mutex_holding(lk))
    printf(2, "release\n");


  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
  __sync_synchronize();

  // Release the lock, equivalent to lk->locked = 0.
  // This code can't use a C assignment, since it might
  // not be atomic. A real OS would use C atomics here.
  asm volatile("movl $0, %0" : "+m" (lk->locked) : );
}

// Record the current call stack in pcs[] by following the %ebp chain.
// Check whether this cpu is thread_holding the lock.
int
thread_mutex_holding(struct thread_mutex *lock)
{
  int r;
  r = lock->locked;
  return r;
}

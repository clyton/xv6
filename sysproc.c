#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "thread_mutex.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if((addr=growproc(n)) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_memtop(void)
{

  struct run {
    struct run *next;
  };

  char* firstfree = kalloc();
  struct run *temp = (struct run *) firstfree;
  int count = 0;
  while (temp) {
    temp = temp->next;
    count++;
  }
  kfree(firstfree);
  return count*PGSIZE;

}

int
sys_getmeminfo(void) {
  int pid;
  char* buf;
  int len;

  if (argint(0, &pid) < 0)
    return -1;
  if (argint(2, &len) < 0)
    return -1;
  if (argptr(1, &buf, len) < 0)
    return -1;

  int bytesUsed = proc_getmeminfo(pid, buf, len);
  return bytesUsed;


}

int sys_thread_create() {

  char *fcn;
  char *args;
  char *stack;
  if (argptr(0, &fcn, sizeof(int)) < 0)
    return -1;

  if (argptr(1, &args, sizeof(int)) < 0)
    return -1;


  if (argptr(2, &stack, PGSIZE) < 0)
    return -1;

  int childpid = user_thread_create((void *)fcn, (void *)args, (void *)stack);
  return childpid;

}

int sys_thread_exit() {
  exit();
  return 0;
}

int sys_thread_join() {

  return user_thread_join();

}

int sys_thread_wait() {

  char *chan;
  char *clk;
  int *lk;

  if (argstr(0, &chan) < 0)
    return -1;
  if (argptr(1, &clk, sizeof(int)) < 0)
    return -1;

  lk = (int*) clk;

  return user_thread_wait(chan, (uint *)lk);

}

int sys_thread_signal() {

  char *condname;
  if (argstr(0, &condname) < 0)
    return -1;

  return user_thread_signal(condname);
}

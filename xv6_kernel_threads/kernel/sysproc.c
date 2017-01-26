#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

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
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  if(proc->stack != NULL)
    return proc->parent->sz;

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
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_clone(void) {

  void * ptr;
  void * arg;
  void * stack;
  int pid;

  argptr(0, (char **) &ptr, sizeof(void *));
  argptr(1, (char **) &arg, sizeof(void *));
  argptr(2, (char **) &stack, sizeof(void *));

  if (stack == NULL || ((uint) stack) % PGSIZE != 0){
    cprintf("stack = %p\n stack % PGSIZE = %p\n", stack, ((uint) stack) % PGSIZE);
    cprintf("BAD STACK VAL\n");
    return -1;
  }

  cprintf("PASS 1\n");

  if ((uint)proc->sz - (uint)stack == PGSIZE/2)
    return -1;

  cprintf("PASS2\n");

  if (ptr == NULL)
    return -1;

  cprintf("PASS 3\n");

  pid = clone(ptr, arg, stack);
  cprintf("sys_clone returns pid = %d\n", pid);
  return pid;

}

int sys_join(void) {

  void * stack = NULL;

  argptr(0, (void *) &stack, sizeof(void *));

  if(stack == NULL)
    return -1;

  return join(stack);
}

#include <signal.h>
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

ucontext_t ucontext1, ucontext2;

int current = 0;

void produce() {
  sleep(0.5);  // 为了可以直观观看
  current++;
  setcontext(&ucontext2);
}

void consume() {
  printf("current value: %d\n", current);
  setcontext(&ucontext1);
}

int main(int argc, char **argv) {
  char iterator_stack1[64];
  char iterator_stack2[64];

  getcontext(&ucontext1);
  ucontext1.uc_link = NULL;
  ucontext1.uc_stack.ss_sp = iterator_stack1;
  ucontext1.uc_stack.ss_size = sizeof(iterator_stack1);
  makecontext(&ucontext1, (void (*)(void))produce, 0);

  getcontext(&ucontext2);
  ucontext2.uc_link = NULL;
  ucontext2.uc_stack.ss_sp = iterator_stack2;
  ucontext2.uc_stack.ss_size = sizeof(iterator_stack2);
  makecontext(&ucontext2, (void (*)(void))consume, 0);

  setcontext(&ucontext1);

  return 0;
}
#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];
char *const child_args[] = {
  "/usr/bin/bash",
  NULL
};

int child_main(void *args) {
  printf("在子进程中!\n");
  sethostname("NewNamespace", 12);
  execv(child_args[0], child_args);
  return 1;
}

int main(int argc, char *argv[]) {
  printf("程序开始: \n");
  int child_pid = clone(child_main, child_stack + STACK_SIZE, 
                            CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD, NULL);
  waitpid(child_pid, NULL, 0);
  printf("已退出\n");
  return 0;
} 
#define _GNU_SOURCE

#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <sys/capability.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];
char *const child_args[] = {"/usr/bin/fish", NULL};

void set_uid_map(pid_t pid, int inside_id, int outside_id, int length) {
  char path[256];
  sprintf(path, "/proc/%d/uid_map", getpid());
  FILE *uid_map = fopen(path, "w");
  fprintf(uid_map, "%d %d %d", inside_id, outside_id, length);
  fclose(uid_map);
}

void set_gid_map(pid_t pid, int inside_id, int outside_id, int length) {
  char path[256];
  sprintf(path, "/proc/%d/gid_map", getpid());
  FILE *gid_map = fopen(path, "w");
  fprintf(gid_map, "%d %d %d", inside_id, outside_id, length);
  fclose(gid_map);
}

int child_main(void *args) {
  printf("在子进程中!\n");
  set_uid_map(getpid(), 0, 1000, 1);
  set_gid_map(getpid(), 0, 1000, 1);
  cap_t caps;
  printf("eUID = %ld, eGID = %ld\n", (long)geteuid(), (long)getegid());
  caps = cap_get_proc();
  printf("capabilities: %s\n", cap_to_text(caps, NULL));
  execv(child_args[0], child_args);
  return 1;
}

int main(int argc, char *argv[]) {
  printf("程序开始: \n");
  int child_pid = clone(child_main, child_stack + STACK_SIZE,
                        CLONE_NEWUSER | SIGCHLD, NULL);
  waitpid(child_pid, NULL, 0);
  printf("已退出\n");
  return 0;
}
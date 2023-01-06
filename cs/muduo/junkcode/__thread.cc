#include <iostream>
#include <pthread.h>
#include <unistd.h>

__thread int i = 0;

void *f1(void *arg) {
  i++;
  printf("f1 i: address %p val %d\n", &i, i);
  return nullptr;
}

void *f2(void *arg) {
  i += 2;
  printf("f2 i: address %p val %d\n", &i, i);
  return nullptr;
}

int main() {
  pthread_t pid1, pid2;
  i += 3;
  pthread_create(&pid1, NULL, f1, NULL);
  pthread_create(&pid2, NULL, f2, NULL);
  pthread_join(pid1, NULL);
  pthread_join(pid2, NULL);

  printf("main i: address %p val %d\n", &i, i);

  return 0;
}
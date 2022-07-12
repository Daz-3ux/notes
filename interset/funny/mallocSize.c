#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <malloc.h>

int main(int argc, char **argv)
{
  int *a = (int *)malloc(0);//Linux定义
  int *b = (int *)malloc(1);//内存对齐
  int *c = (int *)malloc(9);//内存对齐
  int *d = (int *)malloc(25);//内存对齐
  int *e = (int *)malloc(4096);
  size_t a_size = malloc_usable_size(a);
  size_t b_size = malloc_usable_size(b);
  size_t c_size = malloc_usable_size(c);
  size_t d_size = malloc_usable_size(d);
  size_t e_size = malloc_usable_size(e);

  printf("%ld %ld %ld %ld %ld\n", a_size, b_size, c_size, d_size, e_size);
}
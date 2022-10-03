#include <stdio.h>

void my_swap(int ***a, int ***b) {
  int temp = ***a;
  ***a = ***b;
  ***b = temp;
}

int main(int argc, char **argv)
{
  int a = 1;
  int b = 2;
  printf("%d %d\n", a, b);
  int *c = &a;
  int *d = &b;
  int **e = &c;
  int **f = &d;
  my_swap(&e, &f);
  printf("%d %d\n", a, b);
}
#include <stdio.h>
#include "usedByC.h"

int main(int argc, char *argv[]) {
  GoInt x = 2;
  GoInt y = 3;
  printf("About to call a Go function!\n");
  PrintMessage();

  GoInt p = Multiply(x, y);
  printf("Product: %d\n", (int)p);
  printf("It worked!\n");

  return 0;
}
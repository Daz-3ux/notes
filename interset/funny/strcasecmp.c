#include <stdio.h>
#include <strings.h>

int main() {
  char *a = "aBcDeF";
  char *b = "AbCdEf";
  if (!strcasecmp(a, b)) { // 相同则返回0
    printf("%s=%s\n", a, b);
  }
}

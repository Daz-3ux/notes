#include <iostream>
#include <array>

int main(int argc, char **argv)
{
  int **a2d = new int*[50];
  for(int i = 0; i < 50; i++) {
    a2d[i] = new int[50];
  }

  int ***a3d = new int**[50];
  for(int i = 0; i < 50; i++) {
    a3d[i] = new int*[50];
    for(int j = 0; j < 50; j++) {
      //a3d[i][j] = new int[50];
      int **ptr = a3d[i];
      ptr[j] = new int[50];
    }
  }
}
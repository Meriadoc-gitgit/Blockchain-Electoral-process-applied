#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "src.h"

void print_long_vector(long *result, int size) {
  printf("Vector: [");
  for(int i=0; i<size;i++) {
    printf("%lx \t",result[i]);
  }
  printf("]\n");
  return;
}

int main(void) {
  
  srand(time(NULL));
  
  CellKey* c = read_public_keys("keys.txt");

  print_list_keys(c);
  delete_cell_keys(c);
  printf("\n\n");
  print_list_keys(c);

  printf("\n\n");

  delete_list_keys(c);
  print_list_keys(NULL);
  return 0;
}
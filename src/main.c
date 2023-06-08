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
  int n=10;
  while (n--) {
    printf("%d\n",rand()%10);
  }


  /*
  FILE* f = fopen("keys.txt","w");
  int i = 0; long n = 10;
  while (i<n) {
    fprintf(f,"test\n");
    i++;
  }
  */
  printf("\n\ngenerate test \n\n");
  generate_random_data(10,5);
  /*
  Key* pKey = (Key*)malloc(sizeof(Key));
  Key* sKey = (Key*)malloc(sizeof(Key));

  init_pair_keys(pKey,sKey,3,200);
  fprintf(f,"(%lx,%lx)\n",pKey->val,sKey->val);
  printf("(%lx,%lx)\n",pKey->val,sKey->val);
  */

  return 0;
}
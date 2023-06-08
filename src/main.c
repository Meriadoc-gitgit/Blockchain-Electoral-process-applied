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
    printf("PRIME: %ld\n",random_prime_number(3,200,5000));
    printf("RAND_LONG: %ld\n",rand_long(3,200));
  }


  /*
  FILE* f = fopen("keys.txt","w");
  int i = 0; long n = 10;
  while (i<n) {
    fprintf(f,"test\n");
    i++;
  }
  */
  //printf("\n\ngenerate test \n\n");
  //generate_random_data(5,3); 
  /*
  Key* pKey = (Key*)malloc(sizeof(Key));
  Key* sKey = (Key*)malloc(sizeof(Key));

  init_pair_keys(pKey,sKey,3,200);
  fprintf(f,"(%lx,%lx)\n",pKey->val,sKey->val);
  printf("(%lx,%lx)\n",pKey->val,sKey->val);
  */

  return 0;
}
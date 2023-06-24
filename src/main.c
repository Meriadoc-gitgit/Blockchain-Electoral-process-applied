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
  
  long p = 3, q = 111;
  long n, s, u;
  generate_key_values(p,q,&n,&s,&u);
  printf("%lu %lu %lu\n",n,s,u);

  Key* pKey = (Key*)malloc(sizeof(Key));
  Key* sKey = (Key*)malloc(sizeof(Key));

  Key* pKey2 = (Key*)malloc(sizeof(Key));
  Key* sKey2 = (Key*)malloc(sizeof(Key));

  init_pair_keys(pKey,sKey,3,11);

  init_pair_keys(pKey2,sKey2,5,25);

  printf("pKey: %s\nsKey: %s\n",key_to_str(pKey),key_to_str(sKey));

  printf("%s\n",key_to_str(str_to_key(key_to_str(pKey))));


  Signature* sgn = sign(key_to_str(pKey),sKey);

  printf("%s\n",signature_to_str(sgn));

  Signature* sgn2 = str_to_signature(signature_to_str(sgn));

  printf("%s\n",signature_to_str(sgn2));


  printf("%s\n",decrypt(sgn2->content,strlen(key_to_str(pKey)),sKey->val,sKey->n));

  printf("verification: %d\n",verify(init_protected(pKey,key_to_str(pKey2),sign(key_to_str(pKey2),sKey)))); //ok
  return 0;
}
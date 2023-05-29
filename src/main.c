#include <stdio.h>
#include <stdlib.h>

#include "src.h"

int main(void) {
  long u = 0, v = 1;
  long random = random_prime_number(100,10000,7);
  printf("Prime generator: %ld\nPrime check: %d\n",random,is_prime_naive(random));
  printf("TEST=========");
  printf("test of extended_gcd: %ld\nu: %ld\nv: %ld",extended_gcd(random_prime_number(1,100,7),3,&u,&v),u,v);
  return  0;
}
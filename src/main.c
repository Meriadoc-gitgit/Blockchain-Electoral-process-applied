#include <stdio.h>
#include <stdlib.h>

#include "src.h"

int main(void) {
  long random = random_prime_number(100,10000,7);
  printf("Prime generator: %ld\nPrime check: %d\n",random,is_prime_naive(random));
  return  0;
}
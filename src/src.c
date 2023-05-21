#include <stdio.h>
#include <stdlib.h>

#include "src.h"

/*PART 1*/

int is_prime_naive(long p) {
  if (!(p%2)) return 0;
  for (int i=3;i<p;i++) {
    if (!(p%i)) return 0;
  }
  return 1;
}
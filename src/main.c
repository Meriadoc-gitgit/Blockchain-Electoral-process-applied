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
  /*
  long u = 0, v = 1;
  long random = random_prime_number(100,10000,7);
  printf("Prime generator: %ld\nPrime check: %d\n",random,is_prime_naive(random));

  printf("TEST=========\n");
  printf("test of extended_gcd: %ld\nu: %ld\nv: %ld",extended_gcd(random_prime_number(1,100,7),3,&u,&v),u,v);

  long p = 3, q = 11;
  long n, s, uK;
  generate_key_values(p,q,&n,&s,&uK);
  printf("Key generator\np: %ld\ns: %ld\nu: %ld\n",p,s,uK);
  */

  srand(time(NULL));

  //Generation de cle: 
  long p = random_prime_number(3,7,5000);
  long q = random_prime_number(3,7,5000);
  while (p==q) 
    q = random_prime_number(3,7,5000);
  long n, s, u;
  generate_key_values(p,q,&n,&s,&u);

  printf("n: %ld\n",n);

  //Pour avoir des cles positives: 
  if (u<0) {
    long t = (p-1)*(q-1);
    u+=t; //on aura toujours s*u mod t = 1
  }

  //Affichage des cles en hexadecimal
  printf("cle publique = (%lx, %lx)\n", s, n);
  printf("cle privee = (%lx, %lx)\n", u, n);
  
  //Chiffrement:
  char mess[10] = "Hello";
  int len = strlen(mess);
  long* crypted = encrypt(mess,s,n);

  printf("Initial message: %s\n",mess);
  printf("Encoded representation: \n");
  print_long_vector(crypted,len);

  //Dechiffrement
  char* decoded = decrypt(crypted,len,u,n);
  printf("Decoded: %s\n",decoded);

  return 0;
}
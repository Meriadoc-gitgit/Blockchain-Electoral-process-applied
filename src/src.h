#ifndef SRC_H
#define SRC_H

/*PART 1*/
/*RESOLUTION DU PROBLEME DE PRIMALITE*/
int is_prime_naive(long p);

/*EXPONENTATION MODULAIRE RAPIDE*/
long modpow_naive(long a, long m, long n);
int modpow(long a, long m, long n);

/*TEST DE MILLER-RABIN*/
int witness(long a, long b, long d, long p);
long rand_long(long low, long up);
int is_prime_miller(long p, int k);
#endif
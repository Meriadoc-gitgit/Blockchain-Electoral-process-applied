#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "src.h"

/*PART 1*/
/*RESOLUTION DU PROBLEME DE PRIMALITE*/
int is_prime_naive(long p) {
  if (!(p%2)) return 0; //verifier si p est impair
  for (int i=3;i<p;i++) {
    if (!(p%i)) return 0; //s'il existe un entier entre e et p-1 divisant p, p n'est pas premier
  }
  return 1; //si toutes conditions satisfaites, p est donc premier
}

/*EXPONENTATION MODULAIRE RAPIDE*/
long modpow_naive(long a, long m, long n) {
  long res = 1;
  while(m>0) {
    res*=a; //multiplier la valeur courante par a
    res = res%n; //applique mod n sur le resultat avant de passer a l'iteration suivante
    m--; //decrementer m en fin de boucle
  }
  return res;
}
int modpow(long a, long m, long n) {
  if (m==0) return 1; //si m=0, cas de base
  else if (m%2==0) return (modpow(a,m/2,n) * modpow(a,m/2,n)) % n; //m est pair
  else return (a * modpow(a,floor(m/2),n) * modpow(a,floor(m/2),n)) % n; //m est impair
}
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

/*TEST DE MILLER-RABIN*/
int witness(long a, long b, long d, long p) {
  long x = modpow(a,d,p);
  if (x==1) return 0;
  for(long i=0;i<b;i++) {
    if (x==p-1) return 0;
    x = modpow(x,2,p);
  }
  return 1;
 }
long rand_long(long low, long up) {
  return rang() % (up-low+1) + low;
}
int is_prime_miller(long p, int k) {
  if (p==2) return 1;
  if (!(p&1) || p<=1) return 0; //verifie que p est impair et different de 1

  //determine b et d
  long b=0, d=p-1;
  while(!(d&1)) { //tant que d pas impair
    d/=2;
    b++;
  }
  //generer k valeurs pour a, et teste si c'est un temoin : 
  long a;
  for (int i=0;i<k;i++) {
    a = rand_long(2,p-1);
    if (witness(a,b,d,p)) return 0;
  }
  return 1;
}
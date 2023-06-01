#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

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
  srand(time(NULL));
  return rand() % (up-low+1) + low;
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
  //generer k valeurs de a, et teste si c'est un temoin : 
  long a;
  for (int i=0;i<k;i++) {
    a = rand_long(2,p-1);
    if (witness(a,b,d,p)) return 0;
  }
  return 1;
}

/*GENERATION DE NOMBRES PREMIERS*/
long random_prime_number(int low_size, int up_size, int k) {
  long random = rand_long(low_size,up_size);
  while (!is_prime_miller(random,k)) 
    random = rand_long(low_size,up_size);
  return random;
}


/*IMPLEMENTATION DU PROTOCOLE RSA*/
/*generatoin d'une paire (pub,sec)*/
long extended_gcd(long s, long t, long *u,long *v) {
  if (!s) {
    *u=0;
    *v=1;
    return t;
  }
  long uPrim, vPrim;
  long gcd = extended_gcd(t%s,s,&uPrim,&vPrim);
  *u = vPrim - (t/s)*uPrim;
  *v = uPrim;
  return gcd;
}
void generate_key_values(long p, long q, long *n, long *s, long *u) {
  *n = p*q;
  long t = (p-1) * (q-1);
  long v = 1;
  long sPrim = random_prime_number(0,t,1000);
  long uPrim = 0;
  while (extended_gcd(sPrim,t,&uPrim,&v)!=1) 
    sPrim = random_prime_number(0,t,1000);
  *s = sPrim;
  *u = uPrim;
  return;
}

/*chiffrement et déchiffrement de messages*/
long* encrypt(char* chaine, long s, long n) {
  long* encr = (long*)malloc(sizeof(long)*strlen(chaine));
  for(int i=0;chaine[i]!='\0';i++)
    encr[i] = modpow((long)chaine[i],s,n);
  return encr;
}
char* decrypt(long* crypted, long size, long u, long n) {
  char* decr = (char*)malloc((size+1)*sizeof(char));
  for(int i=0;i<size;i++) {
    decr[i] = modpow(crypted[i],u,n);
    printf("%ld %d %c\t",crypted[i],modpow(crypted[i],u,n),(char)(modpow(crypted[i],u,n) + n*5));
  }
  decr[size] = '\0';
  printf("\n");3
  return decr;
}
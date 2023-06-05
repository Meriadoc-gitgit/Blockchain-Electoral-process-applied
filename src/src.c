#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

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
  while(m--) {
    res*=a; //multiplier la valeur courante par a
    res%=n; //applique mod n sur le resultat avant de passer a l'iteration suivante
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
  while (extended_gcd(sPrim,t,&uPrim,&v)!=1 || sPrim==uPrim) {
    sPrim = random_prime_number(0,t,1000);
  }
  //printf("s: %ld u: %ld\n",sPrim,uPrim);
  *s = sPrim;
  *u = uPrim;
  return;
}

/*chiffrement et déchiffrement de messages*/
long* encrypt(char* chaine, long s, long n) {
  long* encr = (long*)malloc(sizeof(long)*strlen(chaine));
  for(int i=0;chaine[i]!='\0';i++) {
    encr[i] = modpow((long)chaine[i],s,n);
  }
  return encr;
}
char* decrypt(long* crypted, long size, long u, long n) {
  char* decr = (char*)malloc((size+1)*sizeof(char));
  for(int i=0;i<size;i++) {
    decr[i] = (char)modpow(crypted[i],u,n);
  }
  decr[size] = '\0';
  return decr;
}



/*MANIPULATION DE STRUCTURES SECURISEES*/
/*manipulation des cles*/
void init_key(Key* key, long val, long n) {
  key->val = val;
  key->n = n;
  return;
}
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size) {
  /*Generer 2 premiers p et q*/
  
  long p = random_prime_number(low_size,up_size,5000);
  long q = random_prime_number(low_size,45,5000);
  while (p==q || q<43) 
    q = random_prime_number(low_size,45,5000);
  

  /*generer les 2 cles publiques et secretes*/
  long n, s, u;
  generate_key_values(p,q,&n,&s,&u);

  //pour avoir des cles positives
  if (u<0) {
    long t = (p-1)*(q-1);
    u+=t; //on aura toujours s*u mod t = 1
  }
  //initialiser les cles publiques et secretes
  init_key(pKey,s,n);
  init_key(sKey,u,n);
  return;
}
char* key_to_str(Key* key) {
  char* res = (char*)malloc(INT_MAX);
  sprintf(res,"(%lx,%lx)",key->val,key->n);
  return res;
}
Key* str_to_key(char* str) {
  long val, n;
  sscanf(str,"(%lx,%lx)",&val,&n);
  Key* key = (Key*)malloc(sizeof(Key));
  init_key(key,val,n);
  return key;
}


/*signature*/
Signature* init_signature(long* content, int size) {
  Signature* sgn = (Signature*)malloc(sizeof(Signature));
  sgn->content = content;
  sgn->size = size;
  return sgn;
}
Signature* sign(char* mess, Key* sKey) {
  return init_signature(encrypt(mess,sKey->val,sKey->n),(int)strlen(mess));
}
char* signature_to_str(Signature* sgn) {
  char* result = (char*)malloc(INT_MAX);
  result[0] = '#';
  int pos = 1;
  char buffer[256];
  for(int i=0;i<sgn->size;i++) {
    sprintf(buffer,"%lx",sgn->content[i]);
    for(int j=0;j<strlen(buffer);j++) {
      result[pos] = buffer[j];
      pos++;
    }
    result[pos] = '#';
    pos++;
  }
  result[pos] = '\0';
  result = realloc(result,(pos+1)*sizeof(char));
  return result;
}
Signature* str_to_signature(char* str) {
  int len = strlen(str);
  long* content = (long*)malloc(sizeof(long)*len);
  int num = 0;
  char buffer[256];
  int pos = 0;
  for(int i=0;i<len;i++) {
    if (str[i]!='#') {
      buffer[pos] = str[i];
      pos++;
    } else {
      if (pos) {
        buffer[pos] = '\0';
        sscanf(buffer,"%lx",&(content[num]));
        num++;
        pos = 0;
      }
    }
  }
  content = realloc(content,num*sizeof(long));
  return init_signature(content,num);
}


/*declarations signes*/
Protected* init_protected(Key* pKey, char* mess, Signature* sgn) {
  Protected* pr = (Protected*)malloc(sizeof(Protected));
  pr->pKey = pKey;
  pr->mess = strdup(mess);
  pr->sgn = sgn;
  return pr;
}
int verify(Protected* pr) {
  return strcmp(pr->mess,decrypt(pr->sgn->content,pr->sgn->size,pr->pKey->val,pr->pKey->n)) == 0;
}
char* protected_to_str(Protected* pr) {
  char* chaine = (char*)malloc(INT_MAX);
  sprintf(chaine,"%s\t%s\t%s",key_to_str(pr->pKey),pr->mess,signature_to_str(pr->sgn));
  return chaine;
}
Protected* str_to_protected(char* chaine) {
  char key[256], mess[256], sign[256];
  sscanf(chaine,"%s\t%s\t%s",key,mess,sign);
  return init_protected(str_to_key(key),mess,str_to_signature(sign));
}

/*creation de donnees pour simuler le processus de vote*/
struct stat st = {0}; //structure pour la verification de l'existence d'un fichier dans l'environnement
int exists(Key** tab, int taille, long val) {
  while (taille--) {
    if (tab[taille-1]->val==val) return 1;
  }
  return 0;
}
void generate_random_data(int nv, int nc) {
  if (stat("keys.txt",&st)==-1) {
    system("echo > keys.txt");
  }

  FILE* f = fopen("keys.txt","w");
  long s, u; //(public, private) key
  long n;

  //randomize 2 prime numbers p and q
  long p = random_prime_number(3,7,5000);
  long q = random_prime_number(3,200,5000);
  printf("%ld %ld\n",p,q);

  Key* public_key_src[nv];
  Key* pKey = NULL;
  int i = 0;
  while (i<nv) {
    generate_key_values(p,q,&n,&s,&u);
    if (!exists(public_key_src,nv,s)) {
      init_key(pKey,s,n);
      public_key_src[i] = pKey;
      fprintf(f,"(%lx,%lx)\n",s,u);
      i++;
    }
  }
  fclose(f);


  if (stat("candidates.txt",&st)==-1) {
    system("echo > candidates.txt");
  }
  f = fopen("candidates.txt","w");
  i = 0;
  Key* candidate_key_src[nc];
  while (i<nc) {
    int rdm = rand() % nv;
    if (!exists(candidate_key_src,nc,public_key_src[rdm]->val)) {
      candidate_key_src[i] = public_key_src[rdm];
      fprintf(f,"%s\n",key_to_str(public_key_src[rdm]));
      i++;
    }
  }
  fclose(f);


  if (stat("declarations.txt",&st)==-1) {
    system("echo > declarations.txt");
  }
  i = 0;
  f = fopen("declarations.txt","w");
  while (i<nv) {
    int rdm = rand() % nc;
    Signature* sgn = sign(key_to_str(candidate_key_src[rdm]),public_key_src[i]);
    fprintf(f,"%s\n",signature_to_str(sgn));
    i++;
  }
  fclose(f);
  return;
}
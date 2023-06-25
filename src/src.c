#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <limits.h>
//#include <openssl/sha.h>

#include "src.h"


/*PART 1*/
/*RESOLUTION DU PROBLEME DE PRIMALITE*/
int is_prime_naive(long p) {
  if (!(p%2)) return 0;
  for (int i=3;i<p;i++) {
    if (!(p%i)) return 0;
  }
  return 1;
}

/*exponentiation modulaire rapide*/
long modpow_naive(long a, long m, long n) {
  long res = a%n;
  while (m--) {
    res*=a; res%=n;
  }
  return res;
}
int modpow(long a, long m, long n) {
  if (!m) return 1;
  else if (!(m%2)) return (modpow(a,m/2,n) * modpow(a,m/2,n)) % n;
  else return (a * modpow(a,floor(m/2),n) * modpow(a,floor(m/2),n)) % n;
}

/*test de miller-rabin*/
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

/*generation des nombres premiers*/
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
  long sPrim = random_prime_number(0,t,500);
  long uPrim = 0;
  while (extended_gcd(sPrim,t,&uPrim,&v)!=1 || sPrim==uPrim) {
    sPrim = random_prime_number(0,t,500);
  }
  *s = sPrim;
  *u = uPrim;
  return;
}

/*chiffrement et déchiffrement de messages*/
long* encrypt(char* chaine, long s, long n) {
  long* encrypted = (long*)malloc(strlen(chaine)*sizeof(long));
  for (int i=0;i<strlen(chaine);i++) {
    encrypted[i] = modpow((long)chaine[i],s,n);
  }
  return encrypted;
}
char* decrypt(long* crypted, long size, long u, long n) {
  char* decrypted = (char*)malloc((size+1)*sizeof(char));
  for(int i=0;i<size;i++) {
    decrypted[i] = (char)modpow(crypted[i],u,n);
  }
  decrypted[size] = '\0';
  return decrypted;
}




//============================================

/*PART 2*/
/*DECLARATIONS SECURISEES*/
/*Structure for key manipulation*/

/*manipulation des cles*/
void init_key(Key* key, long val, long n) {
  key->val = val;
  key->n = n;
  return;
}
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size) {
  /*Generer 2 premiers p et q*/
  long p = random_prime_number(low_size,up_size,5000);
  //long q = random_prime_number(low_size,up_size_q,5000);
  long q = ceil(129/p);
  if (p==q || p*q<128) q++;
  //printf("p: %lu q: %lu\n",p,q);
  /*generer les 2 cles publiques et secretes*/
  long n,s,u;
  generate_key_values(p,q,&n,&s,&u);

  if (u<0) { 
    long t = (p-1) * (q-1);
    u+=t;
  }

  init_key(pKey,s,n);
  init_key(sKey,u,n);
  
  return;
}
char* key_to_str(Key* key) {
  char* res = (char*)malloc(28);
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
  sprintf(chaine,"%s %s %s",key_to_str(pr->pKey),pr->mess,signature_to_str(pr->sgn));
  return chaine;
}
Protected* str_to_protected(char* chaine) {
  char key[256], mess[256], sign[256];
  sscanf(chaine,"%s %s %s",key,mess,sign);
  return init_protected(str_to_key(key),mess,str_to_signature(sign));
}

/*creation de donnees pour simuler le processus de vote*/
void generate_random_data(int nv, int nc) {

  //nv couples de cles (publique, secrete) des nv citoyens
  char buff_pub[2000*sizeof(long)];
  char buff_private[2000*sizeof(long)];

  Key* src_public[nv];
  Key* src_private[nv];

  Key* pKey = (Key*)malloc(sizeof(Key));
  Key* sKey = (Key*)malloc(sizeof(Key));

  for(int i=0;i<nv;i++) {
    init_pair_keys(pKey,sKey,3,11);
    src_public[i] = (Key*)malloc(sizeof(Key));
    src_private[i] = (Key*)malloc(sizeof(Key));

    src_public[i]->val = pKey->val;
    src_public[i]->n = pKey->n;

    src_private[i]->val = sKey->val;
    src_private[i]->n = sKey->n;

    sprintf(buff_pub,"%s%s\n",buff_pub,key_to_str(pKey));
    sprintf(buff_private,"%s%s\n",buff_private,key_to_str(sKey));
  }
  FILE* f = fopen("keys.txt","w");
  fprintf(f,"%s",buff_pub);
  fclose(f);

  //list candidates de nc couples de cles publiques choisies aleatoirement
  Key* candidates[nc];
  char buff_c[2000*sizeof(long)];
  int i=0;
  while (i<nc) {
    long random = rand_long(0,nv);
    candidates[i] = (Key*)malloc(sizeof(Key));
    candidates[i]->val = src_public[random]->val;
    candidates[i]->n = src_public[random]->n;
    sprintf(buff_c,"%s%s\n",buff_c,key_to_str(src_public[random]));
    i++;
  }
  f = fopen("candidates.txt","w");
  fprintf(f,"%s",buff_c);
  fclose(f);

  char buff_d[2000*sizeof(long)];
  for(i=0;i<nv;i++) {
    long random = rand_long(0,nc);
    Protected* pr = init_protected(
      src_public[i],
      key_to_str(candidates[random]),
      sign(
        key_to_str(candidates[random]),
        src_private[i]
      )
    );
    sprintf(buff_d,"%s%s\n",buff_d,protected_to_str(pr));
  }
  f = fopen("declarations.txt","w");
  fprintf(f,"%s",buff_d);
  fclose(f);

  return;
}
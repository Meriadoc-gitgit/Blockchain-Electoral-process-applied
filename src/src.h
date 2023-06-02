#ifndef SRC_H
#define SRC_H

/*Structure used to enhance decryption*/
typedef struct _encr {
  long mod;
  int res;
} Encrypted;

/*PART 1*/
/*RESOLUTION DU PROBLEME DE PRIMALITE*/
int is_prime_naive(long p);

/*exponentiation modulaire rapide*/
long modpow_naive(long a, long m, long n);
int modpow(long a, long m, long n);

/*test de miller-rabin*/
int witness(long a, long b, long d, long p);
long rand_long(long low, long up);
int is_prime_miller(long p, int k);

/*generation des nombres premiers*/
long random_prime_number(int low_size, int up_size, int k);


/*IMPLEMENTATION DU PROTOCOLE RSA*/
/*generatoin d'une paire (pub,sec)*/
long extended_gcd(long s, long t, long *u,long *v);
void generate_key_values(long p, long q, long *n, long *s, long *u);

/*chiffrement et déchiffrement de messages*/
Encrypted* encrypt(char* chaine, long s, long n);
char* decrypt(Encrypted* crypted, long size, long u, long n);


//============================================

/*PART 2*/
/*DECLARATIONS SECURISEES*/
/*Structure for key manipulation*/
typedef struct _key {
  long val;
  long n;
} Key;

/*manipulation des cles*/
void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size);
char* key_to_str(Key* key);
Key* str_to_key(char* str);

/*signature*/
typedef struct _sign {
	Encrypted* content;
	int size;
} Signature;

Signature* init_signature(Encrypted* content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str(Signature* sgn);
Signature* str_to_signature(char* str);
#endif
#ifndef SRC_H
#define SRC_H

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
long* encrypt(char* chaine, long s, long n);
char* decrypt(long* crypted, long size, long u, long n);


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
	long* content;
	int size;
} Signature;

Signature* init_signature(long* content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str(Signature* sgn);
Signature* str_to_signature(char* str);


/*declarations signes*/
typedef struct protect_ {
	Key* pKey;
	char* mess;
	Signature* sgn;
} Protected;

Protected* init_protected(Key* pKey, char* mess, Signature* sgn);
int verify(Protected* pr);
char* protected_to_str(Protected* pr);
Protected* str_to_protected(char* chaine);

/*creation de donnees pour simuler le processus de vote*/
void generate_random_data(int nv, int nc);



/*BASE DE DECLARATION CENTRALISEE*/
/*Lecture et stockage des données dans des listes de chainées*/

//Liste chainee de cle
typedef struct cellKey {
	Key* data;
	struct cellKey* next;
} CellKey;

CellKey* create_cell_key(Key* key);
void insert_cell_key(CellKey** c, Key* key);
CellKey* read_public_keys(char* file);
void print_list_keys(CellKey* LCK);
void delete_cell_keys(CellKey* c);
void delete_list_keys(CellKey* c);

//Liste chainee de declarations signees
typedef struct cellProtected {
	Protected* data;
	struct cellProtected* next;
} CellProtected;

CellProtected* create_cell_protected(Protected* pr);
void insert_cell_protected(CellProtected** cp, Protected* pr);
CellProtected* read_protected(char* file);
void print_list_protected(CellProtected* LCP);
void delete_cell_protected(CellProtected* c);
void delete_list_protected(CellProtected* c);





/*DETERMINATION DU GAGNANT DE L'ELECTION*/
void violation_filter(CellProtected* cp);

typedef struct hashcell {
	Key* key;
	int val;
} HashCell;
typedef struct hashtable {
	HashCell** tab;
	int size;
} HashTable;

HashCell* create_hashcell(Key* key);
int hash_function(Key* key, int size);
int find_position(HashTable* t, Key* key);
HashTable* create_hashtable(CellKey* keys, int size);
void delete_hashtable(HashTable* t);
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizev);

#endif
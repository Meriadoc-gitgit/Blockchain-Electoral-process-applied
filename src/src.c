#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <limits.h>

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
  if (m==0) 
    return 1; //si m=0, cas de base
  else if (m%2==0) 
    return (modpow(a,m/2,n) * modpow(a,m/2,n)) % n; //m est pair
  else 
    return (a * modpow(a,floor(m/2),n) * modpow(a,floor(m/2),n)) % n; //m est impair
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
  //srand(time(NULL));
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
  //srand(time(NULL));
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
  //srand(time(NULL));
  
  char buff[2000*sizeof(long)];

  //nv couples de cles (publique, secrete) des nv citoyens
  Key** l_key = (Key**)malloc(sizeof(Key*)*nv);
  Key* pKey = (Key*)malloc(sizeof(Key));
  Key* sKey = (Key*)malloc(sizeof(Key));
  for (int i=0;i<nv;i++) {
    init_pair_keys(pKey,sKey,3,200);
    l_key[i] = (Key*)malloc(sizeof(Key));
    l_key[i]->val = pKey->val;
    l_key[i]->n = pKey->n;
    sprintf(buff,"%s%s\n",buff,key_to_str(pKey));
    //printf("l_key: %lx\n",l_key[i]->val);
  }
  FILE* f = fopen("keys.txt","w");
  fprintf(f,"%s",buff);
  fclose(f);

  //list candidates de nc couples de cles publiques choisies aleatoirement
  Key** l_candidat = (Key**)malloc(sizeof(Key*)*nc);
;
  int i=0;
  char buff_c[2000*sizeof(long)];
  while (i<nc) {
    long rdm = rand_long(0,nv);
    //printf("rdm: %ld\n",rdm);
    l_candidat[i] = (Key*)malloc(sizeof(Key));
    l_candidat[i]->val = l_key[rdm]->val;
    l_candidat[i]->n = l_key[rdm]->n;
    sprintf(buff_c,"%s%s\n",buff_c,key_to_str(l_candidat[i]));
    i++;
  }

  f = fopen("candidates.txt","w");
  fprintf(f,"%s",buff_c);
  fclose(f);


  f = fopen("declarations.txt","w");
  char buff_d[2000*sizeof(long)];
  for (i=0;i<nv;i++) {
    long rdm = rand_long(0,nc);
    //printf("rdm2: %ld\n",rdm);
    Protected* p = init_protected(l_key[i],key_to_str(l_candidat[rdm]),sign(key_to_str(l_candidat[rdm]),l_key[i]));

    sprintf(buff_d,"%s%s\n",buff_d,

    protected_to_str(p)

    );

    printf("protected: %s\n",protected_to_str(p));
  }
  fprintf(f,"%s",buff_d);
  fclose(f);
  
  return;
}






/*BASE DE DECLARATION CENTRALISEE*/
/*Lecture et stockage des données dans des listes de chainées*/

//Liste chainee de cles
CellKey* create_cell_key(Key* key) {
  CellKey* c = (CellKey*)malloc(sizeof(CellKey));
  c->data = key;
  c->next = NULL;
  return c;
}
void insert_cell_key(CellKey** c, Key* key) {
  if (!(*c)) *c = create_cell_key(key);
  else {
    CellKey* tmp = create_cell_key(key);
    tmp->next = *c;
    *c = tmp;
  }
  return;
}
CellKey* read_public_keys(char* file) {
  if (strcmp(file,"keys.txt")!=0 && strcmp(file,"candidates.txt")!=0) { 
    printf("Invalid file for candidates keys\n");
    return NULL;
  }
  FILE* f = fopen(file,"r");
  if (!f) {
    printf("Unable to open %s\n",file);
    return NULL;
  }
  char buffer[256];
  //long val, n;
  CellKey* c = NULL;
  while (fgets(buffer,256,f)) {
    //sscanf(buffer,"(%lx,%lx)\n",&val,&n);
    Key* key = str_to_key(buffer);
    //init_key(key,val,n);
    insert_cell_key(&c,key);
  }
  fclose(f);
  return c;
}
void print_list_keys(CellKey* LCK) {
  if (LCK==NULL || (LCK->data->n==0 && LCK->data->val==0)) {
    printf("Empty cell\n");
    return;
  }
  while (LCK) {
    printf("%s\n",key_to_str(LCK->data));
    LCK = LCK->next;
  }
  return;
}
void delete_cell_keys(CellKey* c) {
  if (!c->next) {
    free(c->data);
    free(c);
    return;
  }
  CellKey* tmp = c->next;
  free(c->data);
  free(c);
  c->data = tmp->data;
  c->next = tmp->next;
  return;
}
void delete_list_keys(CellKey* c) {
  CellKey* tmp;
  while (c) {
    if (c->next) tmp = c->next;
    else {
      delete_cell_keys(c);
      break;
    }
    delete_cell_keys(c);
    c = tmp;
  }
  return;
}

//Liste chainee de declarations signees
CellProtected* create_cell_protected(Protected* pr) {
  CellProtected* cp = (CellProtected*)malloc(sizeof(CellProtected));
  cp->data = pr;
  cp->next = NULL;
  return cp;
}
void insert_cell_protected(CellProtected** cp, Protected* pr) {
  if (!(*cp)) *cp = create_cell_protected(pr);
  else {
    CellProtected* tmp = create_cell_protected(pr);
    tmp->next = *cp;
    *cp = tmp;
  }
  return;
}
CellProtected* read_protected(char* file) {
  if (strcmp(file,"declarations.txt")!=0) {
    printf("Invalid file for signed declarations");
    return NULL;
  }
  FILE* f = fopen(file,"r");
  if (!f) {
    printf("Unable to open %s\n",file);
    return NULL;
  }
  char buffer[2000];
  //char key[500], mess[500],sgn[500];
  CellProtected* cp = NULL;
  while (fgets(buffer,256,f)) {
    //printf("buffer: %s\n",buffer);
    Protected* p = str_to_protected(buffer);
    insert_cell_protected(&cp,p);
  }
  fclose(f);
  return cp;
}
void print_list_protected(CellProtected* LCP) {
  if (!LCP) { 
    printf("No signed declaration found\n");
    return;
  }
  while(LCP) {
    printf("%s\n",protected_to_str(LCP->data));
    LCP = LCP->next;
  }
  return;
}
void delete_cell_protected(CellProtected* c) {
  if (!c->next) {
    free(c->data);
    free(c);
    return;
  }
  CellProtected* tmp = c->next;
  free(c->data);
  free(c);
  c->data = tmp->data;
  c->next = tmp->next;
  return;
}
void delete_list_protected(CellProtected* c) {
  CellProtected* tmp;
  printf("\n\n");
  while (c) {
    if (c->next) tmp = c->next;
    else {
      delete_cell_protected(c);
      break;
    }
    delete_cell_protected(c);
    c = tmp;
    print_list_protected(c);
    printf("\n\n");
  }
  return;
}




/*DETERMINATION DU GAGNANT DE L'ELECTION*/
void violation_filter(CellProtected* cp) {
  while (cp) {
    if (!verify(cp->data)) delete_cell_protected(cp);
    cp = cp->next;
  }
  return;
}
HashCell* create_hashcell(Key* key) {
  HashCell* hc = (HashCell*)malloc(sizeof(HashCell));
  hc->key = key;
  hc->val = 0;
  return hc;
}
int hash_function(Key* key, int size) {
  return key->val % size;
}
int find_position(HashTable* t, Key* key) {
  int pos = hash_function(key,t->size);
  for (int i=pos;i<t->size;i++) {
    if (t->tab[i]->key->val==key->val && t->tab[i]->key->n==key->val) 
      return i;
    else if (t->tab[i]->key->val==0 && t->tab[i]->key->n==0) {
      printf("This candidate suppose to be in the position no.%d\n",i);
      return i;
    }
  }
  printf("Table full, no data found\n");
  return -1;
}
HashTable* create_hashtable(CellKey* keys, int size) {
  
}
void delete_hashtable(HashTable* t);
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizev);
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
  
  srand(time(NULL));
  int n=10;
  while (n--) {
    printf("PRIME: %ld\n",random_prime_number(3,200,5000));
    printf("RAND_LONG: %ld\n",rand_long(3,200));
  }


  /*
  FILE* f = fopen("keys.txt","w");
  int i = 0; long n = 10;
  while (i<n) {
    fprintf(f,"test\n");
    i++;
  }
  */
  //printf("\n\ngenerate test \n\n");
  //generate_random_data(5,3); 
  /*
  Key* pKey = (Key*)malloc(sizeof(Key));
  Key* sKey = (Key*)malloc(sizeof(Key));

  init_pair_keys(pKey,sKey,3,200);
  fprintf(f,"(%lx,%lx)\n",pKey->val,sKey->val);
  printf("(%lx,%lx)\n",pKey->val,sKey->val);



  
  Key* key = (Key*)malloc(sizeof(Key));
  init_key(key,10,5);
  Key* key2 = (Key*)malloc(sizeof(Key));
  init_key(key2,15,5);
  CellKey* c = create_cell_key(key);
  printf("%ld\n",c->data->val);

  insert_cell_key(&c,key2);
  printf("%ld\n",c->next->data->val);
  printf("%ld\n",c->data->val);

  c = NULL; 
  insert_cell_key(&c,key2);
  printf("%ld\n",c->data->val);
  
  CellKey* c = read_public_keys("keys.txt");
  printf("\ntest de read_public_keys\n");
  print_list_keys(c);
  printf("\ntest de delete_cell_keys\n");
  //delete_cell_keys(c);
  print_list_keys(c);
  printf("\ntest de delete_list_keys\n");
  delete_list_keys(c);
  print_list_keys(c);
  */

  ///*
  CellProtected* cp = read_protected("declarations.txt");
  printf("\ntest de read_public_protected\n");
  print_list_protected(cp);
  //printf("\ntest de delete_cell_protected\n");
  //delete_cell_protected(cp);
  //print_list_protected(cp);
  printf("\ntest de delete_list_protected\n");
  delete_list_protected(cp);
  //printf("%s\n",protected_to_str(cp->data));
  //print_list_protected(cp);

  
  Protected* p = str_to_protected("#1c5#260#671#442#762#765#a46#63a#260#47d#208#");

  //printf("%s\t%s\t%s\n",key_to_str(p->pKey),p->mess,signature_to_str(p->sgn));
  
  //*/
  return 0;
}
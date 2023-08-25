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
  
  //CellKey* c = read_public_keys("keys.txt");
  CellKey* c2 = read_public_keys("candidates.txt");
  while (c2) {
    printf("%s\n",key_to_str(c2->data));
    c2 = c2->next;
  }

  /*
  print_list_keys(c);
  delete_cell_keys(c);
  printf("\n\n");
  print_list_keys(c);

  printf("\n\n");

  delete_list_keys(c);
  print_list_keys(NULL);
*/


  CellProtected* cp = read_protected("declarations.txt");

  /*while (cp) {
    printf("verify: %d\n",verify(cp->data));
    cp = cp->next;
  }*/
  //printf("\nCellProtected cp : \n");
  //print_list_protected(cp);
  printf("\n\n");
  //delete_cell_protected(cp);
  print_list_protected(cp);
  //delete_list_protected(cp);
  //print_list_protected(cp);

  HashTable* ht = create_hashtable(c2,5);

  for (int i=0;i<5;i++) {
    printf("%s\n",key_to_str(ht->tab[i]->key));
  }
}
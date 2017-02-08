#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#include "MIPS_registre.h"

/**----- REGISTRES -----**/

void init_banc_reg(struct reg banc[34]){

  char tab_noms[35][6] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra","$hi","$lo","$pc"};

  for (uint32_t i = 0; i < 35; i++){
    strcpy(banc[i].nom,tab_noms[i]);
    banc[i].val = 0;
  }
}

void display_one_register(struct reg banc[34],char name[]){
  for (uint32_t i = 0; i < 35; i++){
    if (strcmp(name,banc[i].nom) == 0){
    	printf("%s : %08X\n",banc[i].nom,banc[i].val);
	return;
    }
  }
  printf("%s n'est pas un registre\n",name);
}

void display_all_register(struct reg banc[34]){
  for (uint32_t i = 0; i < 35; i++){
    if ((i-3)%4 == 0 && i>=3){
      printf("%s : %08X\n",banc[i].nom,banc[i].val);
    }
    else{
      if(i != 0){
    	printf("%s : %08X ",banc[i].nom,banc[i].val);
      }
      else{
    	printf("%s:%08X ",banc[i].nom,banc[i].val);
      }
    }
  }
  printf("\n");
}

void set_register(struct reg banc[34], char name[], long val){
  for (uint32_t i = 1; i < 35; i++){
    if (strcmp(name,banc[i].nom) == 0){
      banc[i].val = val;
      return;
    }
  }
  if (strcmp(name,"$zero") != 0){
    printf("%s n'est pas un registre\n",name);
  }
}

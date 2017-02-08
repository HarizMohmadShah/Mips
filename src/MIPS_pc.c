#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "MIPS_pc.h"

// banc[34] car on a les 32 registres "classiques" 
// et 3 autres registres hi, lo et pc
void add(struct reg banc[34], struct cell *cell)
{
  int64_t val_somme =(int64_t)banc[cell->rs].val + (int64_t)banc[cell->rt].val;
  
  
  if (val_somme < -2147483648 || val_somme  > 2147483647)
    {
      printf("%li too big for 32bit register %s\n",val_somme, banc[cell->rd].nom);
      return;  
    }
  set_register(banc, banc[cell->rd].nom, banc[cell->rs].val + banc[cell->rt].val);
} 


void addi(struct reg banc[34], struct cell *cell)
{ 
  int64_t val_somme = (int64_t) banc[cell->rs].val + (int64_t) cell->imm;
 
  if (val_somme < -2147483648 || val_somme  > 2147483647)
    {
      printf("%li too big for 32bit register %s\n",val_somme, banc[cell->rt].nom);
      return;
    }
  set_register(banc, banc[cell->rt].nom, banc[cell->rs].val + cell->imm);
}

 void addiu(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rt].nom, banc[cell->rs].val + cell->imm);
}

void addu(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rd].nom, banc[cell->rs].val + banc[cell->rt].val);
}

void and(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rd].nom, banc[cell->rs].val & banc[cell->rt].val);
}

void beq(struct reg banc[34], struct cell *cell)
{
  int32_t valeurRS, valeurRT;
  valeurRS = banc[cell->rs].val;
  valeurRT = banc[cell->rt].val;
  
  if (valeurRS == valeurRT)
    {
      set_register(banc, banc[34].nom, banc[34].val + 4 + ( cell->imm << 2) );
      cell->need_to_jump = true;
    }else{
    cell->need_to_jump = false;
  }
}


void or(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rd].nom, banc[cell->rs].val | banc[cell->rt].val);
}

void ori(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rt].nom, banc[cell->rs].val | cell->imm);
}


void sll(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rd].nom, (banc[cell->rt].val << cell->sa) & (0xFFFFFFFF << cell->sa)); 
}

void slt(struct reg banc[34], struct cell *cell)
{
  int32_t valeur_rs, valeur_rt;
  valeur_rs = banc[cell->rs].val;
  valeur_rt = banc[cell->rt].val;
  
  if (valeur_rs < valeur_rt)
    {
      set_register(banc, banc[cell->rd].nom, 1); 
    }
  else
    {
       set_register(banc, banc[cell->rd].nom, 0);
    }
}

void srl(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rd].nom, (banc[cell->rt].val >> cell->sa) & (0xFFFFFFFF >> cell->sa));
}

void sub(struct reg banc[34], struct cell *cell)
{  
  int32_t val_diff = banc[cell->rs].val - banc[cell->rt].val;
  if (val_diff < -2147483648 || val_diff  > 2147483647)
    {
      printf("%i too big for 32bit register %s\n",val_diff, banc[cell->rd].nom);
      return;
    }
  set_register(banc, banc[cell->rd].nom, banc[cell->rs].val - banc[cell->rt].val);
}

void xor(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rd].nom, banc[cell->rs].val^banc[cell->rt].val);
}

void jal(struct reg banc[34], struct cell *cell, bool *execute_un_bloc)
{
  *execute_un_bloc = true;
  set_register(banc, banc[31].nom, cell->addr + 4);
  uint32_t temp, val_stocked;
  // le temp permettra d'isoler les 4 bits de poids forts de $pc
  temp = 0xF0000000 & banc[34].val;
  val_stocked = temp | (cell->instr_index << 2);
  set_register(banc, banc[34].nom, val_stocked);
}

void sw(struct reg banc[34], struct cell *cell, struct mem *pmem, uint8_t *framebuffer_pt_hg)
{
  uint32_t adresse;
  uint32_t base = cell->rs;
  uint32_t offset = cell->imm;
  adresse = banc[base].val + offset;
  //la fonction suivante pointe dans la mémoire (pmem) à la case mémoire 
  //ayant pour adresse "adresse" et modifie la valeur contenue dans la 
  //case mémoire par "banc[cell->rt].val"
  struct cell *found = research_cell_mem(*pmem,adresse);
  if (found == NULL){
    inserer_queue_mem(pmem,adresse,banc[cell->rt].val,framebuffer_pt_hg);
  }else{
    found->val = banc[cell->rt].val;
  }
}

void sb(struct reg banc[34], struct cell *cell, struct mem *pmem)
{
  uint32_t adresse, temp;
  uint32_t base = cell->rs;
  uint32_t offset = cell->imm;
  adresse = banc[base].val + offset;
  temp = 0xFF; 
  set_cell_mem(pmem, adresse, (temp & banc[cell->rt].val) << 24);
}

void lui(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rt].nom, (cell->imm) << 16);
}

void jr(struct reg banc[34], struct cell *cell, bool *execute_un_bloc)
{
  *execute_un_bloc = false;
  set_register(banc, banc[34].nom, banc[cell->rs].val);
}

void bne(struct reg banc[34], struct cell *cell)
{
  int32_t valeurRS, valeurRT;
  valeurRS = banc[cell->rs].val;
  valeurRT = banc[cell->rt].val;
  
  if (valeurRS != valeurRT)
    {
      set_register(banc, banc[34].nom, banc[34].val + 4 + ((cell->imm) <<2));
      cell->need_to_jump = true;
    }else{
    cell->need_to_jump = false;
  }
}

void blez(struct reg banc[34], struct cell *cell)
{
  if (banc[cell->rs].val <= 0)
    {
      set_register(banc, banc[34].nom, banc[34].val + 4  + ((cell->imm)<<2));
      cell->need_to_jump = true;
    }else{
    cell->need_to_jump = false;
  }
}

void bgtz(struct reg banc[34], struct cell *cell)
{
   if (banc[cell->rs].val > 0)
    {
      set_register(banc, banc[34].nom, banc[34].val + 4 + (( cell->imm)<<2));
      cell->need_to_jump = true;
    }else{
    cell->need_to_jump = false;
  }
}

void div_mips(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[33].nom, banc[cell->rs].val / banc[cell->rt].val);
  set_register(banc, banc[32].nom, banc[cell->rs].val % banc[cell->rt].val);
}

void j(struct reg banc[34], struct cell *cell)
{
  uint32_t temp, val_stocked;
  // le temp permettra d'isoler les 4 bits de poids forts de $pc
  temp = 0xF0000000 & banc[34].val;
  val_stocked = temp | (cell->instr_index << 2);
  set_register(banc, banc[34].nom, val_stocked);
}

void lb(struct reg banc[34], struct cell *cell, struct mem *pmem)
{
  uint32_t adresse;
  uint32_t base = cell->rs;
  uint32_t offset = cell->imm;   
  uint32_t temp;
  adresse = banc[base].val + offset;
  struct cell *cour = research_cell_mem(*pmem, adresse);
  if (cour != NULL){
    temp = cour -> val >> 24;
    if( (temp & 0x80) == 0x80 ){
      temp = temp | 0xFFFFFF00;
    }else{
      temp = temp & 0x000000FF;
    }
    set_register(banc, banc[cell->rt].nom, temp);
  }else{
    printf("Erreur: la case mémoire recherchée n'a pas été trouvée\n");
  }
}

void lbu(struct reg banc[34], struct cell *cell, struct mem *pmem)
{
  uint32_t adresse, temp;
  uint32_t base = cell->rs;
  uint32_t offset = cell->imm;   
  adresse = banc[base].val + offset;
  struct cell *cour = research_cell_mem(*pmem, adresse);
  if (cour != NULL){
    temp = cour -> val >> 24;
    temp = temp & 0x000000FF;
    set_register(banc, banc[cell->rt].nom, temp);
  }else{
    printf("Erreur: la case mémoire recherchée n'a pas été trouvée\n");
  }
}

void lw(struct reg banc[34], struct cell *cell, struct mem *pmem)
{
  uint32_t adresse;
  uint32_t base = cell->rs;
  uint32_t offset = cell->imm;   
  adresse = banc[base].val + offset;
  struct cell *cour = research_cell_mem(*pmem, adresse);
  if (cour != NULL){
    set_register(banc, banc[cell->rt].nom, cour->val);
  }else{
    printf("Erreur: la case mémoire recherchée n'a pas été trouvée\n");
  }
}

void mfhi(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rd].nom, banc[32].val);
}

void mflo(struct reg banc[34], struct cell *cell)
{
  set_register(banc, banc[cell->rd].nom, banc[33].val);
}

void mult(struct reg banc[34], struct cell *cell)
{
  uint64_t hi, lo, produit;
  produit = (uint64_t)banc[cell->rs].val * banc[cell-> rt].val;
  lo = produit;
  set_register(banc, banc[33].nom, lo);
  hi = produit >> 32;
  set_register(banc, banc[32].nom, hi);
}

void syscall(struct reg banc[34], struct cell *cell, struct mem *pmem)
{
  uint32_t code, a1, a0;
  uint32_t temp = 0xFF000000;
  uint32_t decalage = 0;
  int32_t i = 24;
  uint8_t byte = 0;
  char tampon[127]; 
  code = banc[2].val;
  struct cell *found = NULL;
  a0 = banc[4].val;
  a1 = banc[5].val;

  switch(code){
  case 1:
    printf("%i\n", a0);
    break;
  case 4:
    found = research_cell_mem(*pmem,a0);
    if(found == NULL){
      printf("l'adresse correspondant ne contient pas de string à afficher\n");
    }else{
      do{ 
	byte = (found -> val & temp) >> i;
	printf("%c", byte);
	if( byte != '\n' && i == 0){
	  temp = 0xFF000000;
	  i = 24;
	  found = found -> suiv;
	  if(found == NULL){
	    printf("\n");
	    break;
	  }
	}else{
	  temp = temp >> 8;
	  i = i - 8;
	}
      }while(i >= 0);
    }
    break;
  case 5:
    do{
      printf("Entrez un entier: \n");
      fgets(tampon,sizeof(tampon),stdin);
      strtok(tampon,"\n");
      if (is_valid_int(tampon)==false){
	printf("Vous n'avez pas rentré un entier, veuillez recommencer: \n");
      }
  }while(is_valid_int(tampon) == false);
    set_register(banc, banc[2].nom, atoi(tampon));
    break;
  case 8:
    found = research_cell_mem(*pmem,a0);
    if(found == NULL){
      printf("L'adresse correspondant ne contient pas de tableau de string\n");
    }else{
      found -> val = 0x00000000;
      decalage = 24;
      for(i = 0; i < a1; i++){
	byte = getchar();
	while(byte == '\n'){
	  byte = getchar();
	}
	temp = byte << decalage;
	found -> val = (found -> val) | temp;
	if(decalage == 0){
	  decalage = 24;
	  found = found -> suiv;
	  if(found != NULL){
	    found -> val = 0x00000000;
	  }
	}else{
	  decalage = decalage - 8;
	}
      }
    }
    break;
  case 10:
    exit(0);
    break;
  default:
    NULL;
  }
}

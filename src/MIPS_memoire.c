#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#include "MIPS_memoire.h"

/**----- MEMOIRE -----**/

void init_mem(struct mem *pmem){
  pmem->tete = NULL;
  pmem->queue = NULL;
}

void inserer_queue_mem(struct mem *pmem, uint32_t addr, uint32_t val, uint8_t *framebuffer_pt_hg){
  uint32_t framebuffer_addr_down = 0xFFFF0600;
  uint32_t framebuffer_addr_up = 0xFFFFFFFF;
  if ((addr >= framebuffer_addr_down && addr <= framebuffer_addr_up) && val <= 255){
    if (framebuffer_pt_hg != NULL ){
      *(framebuffer_pt_hg + addr - 0xFFFF0600) = val;
    }
    return;
  }
  if (pmem-> tete == NULL){
    pmem->tete = malloc(sizeof(struct cell));
    assert(pmem->tete != NULL); /* programmation défensive */
    pmem->tete->addr = addr;
    pmem->tete->val = val;
    pmem->tete->prec = NULL;
    pmem->tete->suiv = NULL;
    pmem->queue = pmem->tete;
  }
  else{
    assert(pmem->queue != NULL);
    pmem->queue->suiv = malloc(sizeof(struct cell));
    assert(pmem->queue->suiv != NULL);
    pmem->queue->suiv->addr = addr;
    pmem->queue->suiv->val = val;
    pmem->queue->suiv->suiv = NULL;
    pmem->queue->suiv->prec = pmem->queue;
    pmem->queue = pmem->queue->suiv;
  }
}

struct cell *research_cell_mem(struct mem pmem, uint32_t addr){
  struct cell *cour = pmem.tete;
  while (cour != NULL){
    if (cour->addr == addr){
      return cour;
    }
    cour = cour->suiv;
  }
  return NULL;
}

void display_onecell_mem(struct mem pmem, uint32_t addr){
  struct cell *found = research_cell_mem(pmem,addr); 
  if (found == NULL){
    return;
  }
  printf("0x%08X: %08X\n",addr,found->val);
}

void display_severalcell_mem(struct mem pmem, uint32_t addr1, uint32_t addr2, bool onebyone, bool disp_operation){
  struct cell *first = research_cell_mem(pmem,addr1);
  struct cell *last = first;
  if (first == NULL || addr1 > addr2){
    return;
  }
  else if (addr2 >= pmem.queue->addr){
    last = pmem.queue;
  }
  else if (addr1 != addr2){
    while (last->suiv != NULL){
      if (last->suiv->addr <= addr2){
    	last = last->suiv;
      }
      else{
	break;
      }
    }
  }
  struct cell *cour = first;
  uint32_t cpt = 0;
  while (cour != NULL && cour->addr <= last->addr){
    if (onebyone == 0){
      if (cpt == 0){
	printf("0x%08X: %08X ",cour->addr,cour->val);
	cour = cour->suiv;
	cpt = cpt+1;
      }
      else if(cpt%4 == 0 && cpt > 0){
	printf("\n0x%08X: %08X ",cour->addr,cour->val);
	cour = cour->suiv;
	cpt = cpt+1;
      }
      else{
	printf("%08X ",cour->val);
	cour = cour->suiv;
	cpt = cpt+1;
      }
    }
    else{
      printf("0x%08X: %08X ",cour->addr,cour->val);
      if (disp_operation == true){
	display_operation(cour);
      }else{
	printf("\n");
      }
      cour = cour->suiv;
    }
  }
  if (onebyone == 0){
    printf("\n");
  }
}

void liberer_mem(struct mem *pmem){
  if (pmem->tete == NULL){
    return;
  }
  struct cell *temp = pmem->tete->suiv;
  while (temp != NULL){
    free(pmem->tete);
    pmem->tete = temp;
    temp = temp->suiv;
  }
  free(pmem->tete);
}

void set_cell_mem(struct mem *pmem, uint32_t addr, uint32_t value){
  struct cell *found = research_cell_mem(*pmem,addr);
  if (found == NULL){
    printf("Impossible d'écrire 0x%08X à l'adresse 0x%08X\n",value,addr);
  }
  else{
    found->val = value;
  }
}

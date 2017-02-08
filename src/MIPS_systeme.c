#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#include "MIPS_systeme.h"

/**----- SIMULATEUR -----**/

void init_sim(struct sim *simu, bool framebuffer_enable){
  simu->fin = false;
  simu->execution_terminee = false;
  simu->execution_warning_displayed = false;
  simu->file_loaded = false;
  if (framebuffer_enable == true){
    simu->framebuffer_pt_hg = framebuffer_init_display();
  }else{
    simu->framebuffer_pt_hg = NULL;
  }
  simu->execute_un_bloc = false;
  simu->tete_lecture = NULL;
  simu->nbr_execution = 0;
  for (uint32_t i = 0; i < 4; i++){
    strncpy(simu->commande[i],"",64);
  }
  for (uint32_t i = 0; i < 3; i++){
    simu->commande_num[i] = 0;
  }
  simu->bp_liste = NULL;
  simu->SIMU_state = SIMU_wait;
  simu->MIPS_state = MIPS_init;
  init_banc_reg(simu->banc_reg);
  init_mem(&(simu->memoire));
}

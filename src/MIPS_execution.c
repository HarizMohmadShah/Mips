#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "MIPS_execution.h"

void executer(struct cell **tete_lecture, struct sim *cmd, bool step, bool stepi, bool run){
  /* Il faut qu´un fichier soit chargé pour pouvoir exécuter une opération */
  if (cmd->file_loaded == false){
    printf("Erreur: Aucun fichier n'a été chargé\n");
    cmd->SIMU_state = SIMU_wait;
  }
  /* Si l'exécution est terminée on peut quitter la fonction */
  if ((*tete_lecture) == NULL || cmd->execution_terminee == true){
    return;
  }
  /* EXECUTION DE L'OPERATION */
  if ((*tete_lecture)->pf == NULL ){
    printf("Fonction pas encore implémentée\n");
    return;
  }
  if (run == false){
    display_operation((*tete_lecture));
  }
  switch((*tete_lecture)->etat_correspondant){
  case MIPS_jal:
    ((*tete_lecture)->pf)(cmd->banc_reg,(*tete_lecture),&cmd->execute_un_bloc);
    cmd->nbr_execution++;
    break;
  case MIPS_jr:
    ((*tete_lecture)->pf)(cmd->banc_reg,(*tete_lecture),&cmd->execute_un_bloc);
    cmd->nbr_execution++;
    break;
  case MIPS_sw:
    ((*tete_lecture)->pf)(cmd->banc_reg,(*tete_lecture),&cmd->memoire,cmd->framebuffer_pt_hg);
    cmd->nbr_execution++;
    break;
  case MIPS_lw:
    ((*tete_lecture)->pf)(cmd->banc_reg,(*tete_lecture),&cmd->memoire);
    cmd->nbr_execution++;
    break;
  case MIPS_lb:
    ((*tete_lecture)->pf)(cmd->banc_reg,(*tete_lecture),&cmd->memoire);
    cmd->nbr_execution++;
    break;
  case MIPS_lbu:
    ((*tete_lecture)->pf)(cmd->banc_reg,(*tete_lecture),&cmd->memoire);
    cmd->nbr_execution++;
    break;
  default:
    ((*tete_lecture)->pf)(cmd->banc_reg,(*tete_lecture));
    cmd->nbr_execution++;
    break;
  }
}

void avancer(struct cell **tete_lecture, struct sim *cmd, bool step, bool stepi, bool run){
  /* AFFICHAGE WARNING */
  if (cmd->nbr_execution >= 5000 && cmd->execution_warning_displayed == false){
    printf("Warning: l'exécution est longue (5000 instructions exécutées à ce stade, risque de boucle infinie)\n");
    printf("L'éxecution est interrompue, vous pouvez si vous le souhaitez relancer l'exécution (ce message n'apparaitra plus)\n");
    cmd->execution_warning_displayed = true;
    cmd->SIMU_state = SIMU_wait;
    return;
  }
  /* GESTION DE LA FIN DE L'EXECUTION */
  if ((*tete_lecture) == NULL || cmd->execution_terminee == true){
    printf("Exécution terminée\n");
    cmd->SIMU_state = SIMU_wait;
    return;
  }
  /* AVANCE LA TETE DE LECTURE */
  switch((*tete_lecture)->etat_correspondant){
  case MIPS_beq:
    if ((*tete_lecture)->need_to_jump == true){
      //printf("RECHERCHE DE LA CASE MEMOIRE D'ADRESSE 0x%08X\n", (*tete_lecture) -> rd);
      (*tete_lecture) = research_cell_mem(cmd->memoire, (*tete_lecture) -> rd);
    }else{
      goto DEFAULT;
    }
    break;
  case MIPS_bne:
    if ((*tete_lecture)->need_to_jump == true){
      //printf("RECHERCHE DE LA CASE MEMOIRE D'ADRESSE 0x%08X\n", (*tete_lecture) -> rd);
      (*tete_lecture) = research_cell_mem(cmd->memoire, (*tete_lecture) -> rd);
    }else{
      goto DEFAULT;
    }
    break;
  case MIPS_bgtz:
    if ((*tete_lecture)->need_to_jump == true){
      //printf("RECHERCHE DE LA CASE MEMOIRE D'ADRESSE 0x%08X\n", (*tete_lecture) -> rd);
      (*tete_lecture) = research_cell_mem(cmd->memoire, (*tete_lecture) -> rd);
    }else{
      goto DEFAULT;
    }
    break;
  case MIPS_blez:
    if ((*tete_lecture)->need_to_jump == true){
      //printf("RECHERCHE DE LA CASE MEMOIRE D'ADRESSE 0x%08X\n", (*tete_lecture) -> rd);
      (*tete_lecture) = research_cell_mem(cmd->memoire, (*tete_lecture) -> rd);
    }else{
      goto DEFAULT;
    }
    break;
  case MIPS_j:
    (*tete_lecture) = research_cell_mem(cmd->memoire,cmd->banc_reg[34].val);
    break;
  case MIPS_jal:
    (*tete_lecture) = research_cell_mem(cmd->memoire,cmd->banc_reg[34].val);
    break;
  case MIPS_jr:
    //printf("RECHERCHE DE LA CASE MEMOIRE D'ADRESSE 0x%08X\n", cmd->banc_reg[(*tete_lecture)->rs].val);
    (*tete_lecture) = research_cell_mem(cmd->memoire,cmd->banc_reg[(*tete_lecture)->rs].val);
    break;
  DEFAULT:
  default:
    set_register(cmd->banc_reg,"$pc",cmd->banc_reg[34].val+4);
    (*tete_lecture) = (*tete_lecture)->suiv;
    break;
  }
  /* MET A JOUR L'ETAT DU SIMULATEUR (cmd->SIMU_state) */
  if (step == true){
    if (cmd->execute_un_bloc == true){
      cmd->SIMU_state = SIMU_step;
    }else{
      cmd->SIMU_state = SIMU_wait;
    }
  }else if (stepi == true){
    cmd->SIMU_state = SIMU_wait;
  }else if (run == true){
    cmd->SIMU_state = SIMU_run;
  }
  /* GESTION DE LA FIN DE L'EXECUTION */
  if ((*tete_lecture) == NULL){
    printf("Exécution terminée\n");
    cmd->execution_terminee = true;
    cmd->SIMU_state = SIMU_wait;
    return;
    //}else if((*tete_lecture) -> rd > cmd->text_addr+cmd->text_size-4){ 
  }else if((*tete_lecture) -> addr > cmd->text_addr+cmd->text_size-4){ 
    printf("Exécution terminée\n");
    cmd->execution_terminee = true;
    cmd->SIMU_state = SIMU_wait;
    return;
  }
  /* GESTION DES BREAKPOINTS (seulement pour le run) */
  if (est_present_bp(cmd->bp_liste,cmd->banc_reg[34].val) && run == true){
    printf("Breakpoint atteint à l'adresse: 0x%X\n",cmd->banc_reg[34].val);
    delete_bp(&cmd->bp_liste,cmd->banc_reg[34].val);
    cmd->SIMU_state = SIMU_wait;
  }
}

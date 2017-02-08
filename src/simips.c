#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h> 

#include "simips.h"

#define ADD_DEFAUT 0x400000
#define TAILLE_DEFAUT 0x1000000

int main(int argc, char *argv[]){
  uint32_t text_addr_param = 0;
  uint32_t data_addr_param = 0;
  uint32_t text_addr = 0;
  uint32_t data_addr = 0;
  uint32_t bss_addr = 0;
  size_t text_size = 0;
  size_t data_size = 0;
  size_t bss_size = 0;
  uint32_t taille = 0;
  bool framebuffer_enable = false;
  /* variable pour sshot */
  uint32_t shot_counter = 1;

  if(init_adresse(argc, argv, &text_addr_param, &data_addr_param, &taille, &framebuffer_enable, ADD_DEFAUT, TAILLE_DEFAUT) == false){
    return EXIT_FAILURE;
  }
  system("clear");
  printf("SIMULATEUR MIPS\n");
  struct sim cmd;
  init_sim(&cmd,framebuffer_enable);
  cmd.text_addr = text_addr;
  cmd.text_size = text_size;
  while (cmd.fin != true){  
    struct cell *tete_lecture = research_cell_mem(cmd.memoire,(uint32_t) cmd.banc_reg[34].val); /* pointe sur la cellule dont l'adresse vaut pc */
    switch(cmd.SIMU_state){
    case SIMU_wait:
      recuperer_commande(cmd.commande);
      update_SIMU_state(&cmd.SIMU_state,&cmd.fin,cmd.commande,cmd.commande_num);
      break;
    case SIMU_help:
      help(cmd.commande);
     cmd.SIMU_state = SIMU_wait;
     break;
    case SIMU_load:
      if(cmd.file_loaded == false){
	if( load_file(text_addr_param, data_addr_param, taille, &cmd.memoire, cmd.commande[1], &text_addr, &data_addr, &bss_addr, &text_size, &data_size, &bss_size) == false){
	  printf("Erreur: Echec du chargement du fichier, le programme s'arrête\n");
	  return EXIT_FAILURE;
	}
	maj_champ_cellules(&cmd.memoire); /* initialise le champ de toutes les cellules dans la mémoire */
	set_register(cmd.banc_reg,"$pc",text_addr); /* Initialise $pc */
	set_register(cmd.banc_reg,"$sp",cmd.memoire.queue->addr); /* Initialise $sp */
	tete_lecture = research_cell_mem(cmd.memoire,text_addr);
	cmd.file_loaded = true;
      }
      else{
	printf("Erreur: Un fichier a déjà été chargé\n");
      }
	cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_dreg_all:
      display_all_register(cmd.banc_reg);
      cmd.SIMU_state = SIMU_wait;
     break;
    case SIMU_dreg_one:
      display_one_register(cmd.banc_reg,cmd.commande[1]);
      cmd.SIMU_state = SIMU_wait;
     break;
    case SIMU_dasm_pc:
      display_operation(tete_lecture);
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_dasm_n_pc:
      if (cmd.commande_num[0] != 0){
	display_severalcell_mem(cmd.memoire,cmd.banc_reg[34].val,cmd.banc_reg[34].val+4*(cmd.commande_num[0]-1),true,true);
      }
      cmd.commande_num[0] = 0;
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_dasm_all:
      display_deassembler(cmd.memoire, text_addr, data_addr, bss_addr, text_size, data_size, bss_size);
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_sreg:
      set_register(cmd.banc_reg,cmd.commande[1],cmd.commande_num[0]); 
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_dmem_one:
      display_onecell_mem(cmd.memoire,cmd.commande_num[0]);
      cmd.commande_num[0] = 0;
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_dmem_several:
      display_severalcell_mem(cmd.memoire,cmd.commande_num[0],cmd.commande_num[1],false,false);
      cmd.commande_num[0] = 0;
      cmd.commande_num[1] = 0;
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_smem:
      if(!smem(&cmd.memoire,cmd.commande_num[0],cmd.commande_num[1],cmd.commande_num[2])){
	printf("Erreur: Echec de la modification de la mémoire\n");
      }
      cmd.commande_num[0] = 0;
      cmd.commande_num[1] = 0;
      cmd.commande_num[2] = 0;
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_run:
      /* step = false, stepi=false, run =true */
      executer(&tete_lecture,&cmd,false,false,true);
      avancer(&tete_lecture,&cmd,false,false,true);
      break;
    case SIMU_step:
      /* step = true, stepi=false, run =false */
      executer(&tete_lecture,&cmd,true,false,false);
      avancer(&tete_lecture,&cmd,true,true,false);
      break;
    case SIMU_stepi:
      /* step = false, stepi=true, run =false */
      executer(&tete_lecture,&cmd,false,true,false);
      avancer(&tete_lecture,&cmd,false,true,false);
      break;
    case SIMU_addbp:
      insert_bp(&cmd.bp_liste,cmd.commande_num[0]);
      cmd.commande_num[0] = 0;
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_dbp:
      afficher_bp(cmd.bp_liste);
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_rmbp_all:
      free_bp(&cmd.bp_liste);
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_rmbp_one:
      delete_bp(&cmd.bp_liste,cmd.commande_num[0]);
      cmd.commande_num[0] = 0;
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_sshot:
      if(framebuffer_enable){
	sshot(cmd.framebuffer_pt_hg,&shot_counter);
      }
      else{
	printf("Erreur: Le framebuffer n'est pas initialisé\n");
      }
      cmd.SIMU_state = SIMU_wait;
      break;
    case SIMU_notsupported:
      printf("Commande non gérée (help pour voir les commandes supportées)\n");
      cmd.SIMU_state = SIMU_wait;
      break;
    default:
      break;
    }
  }
  liberer_mem(&cmd.memoire);
  free_bp(&cmd.bp_liste);
  if (framebuffer_enable == true){
    framebuffer_close_display();
  }
  return EXIT_SUCCESS;
}

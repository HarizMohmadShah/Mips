#ifndef _types_
#define _types_

#include <stdint.h>
#include <stdbool.h>

/**----- REGISTRES -----**/

/* Structure d'un registre */
struct reg{
  char nom[6];  /* Nom du registre: $t0,.." */
  int32_t val;     /* Valeur contenue dans le registre */
};

/**----- MEMOIRE -----**/

/* Etats du MIPS simulé */
enum state_MIPS {MIPS_init, MIPS_decode,MIPS_add, MIPS_addu, MIPS_and, MIPS_addi, MIPS_addiu, MIPS_beq, MIPS_bne, MIPS_slti, MIPS_sltiu, MIPS_j, MIPS_jal, MIPS_or, MIPS_ori, MIPS_sll, MIPS_slt, MIPS_srl, MIPS_sub, MIPS_xor, MIPS_sw, MIPS_sb, MIPS_lui, MIPS_jr, MIPS_blez, MIPS_bgtz, MIPS_div, MIPS_lb, MIPS_lbu, MIPS_lw, MIPS_mfhi, MIPS_mflo, MIPS_mult, MIPS_syscall};

/* Structure des cases de la mémoire */
struct cell{
  uint32_t addr;     /* adresse */
  int32_t val;       /* contenu */
  enum state_MIPS etat_correspondant;
  uint32_t sa,instr_index; /* variables pour les instructions */
  int32_t imm, rs,rt,rd;
  bool need_to_jump; /* pour beq,bne,blez,bgtz, vaut vrai si on doit effectuer le branchement */
  void (*pf)();      /* pointeur vers la fonction a exécuter */
  struct cell *suiv; /* pointeur vers case suivante */
  struct cell *prec; /* pointeur vers case précedente */
};

/* Structure de la mémoire: liste doublement chainée de cases mémoires */
struct mem{
  struct cell *tete;
  struct cell *queue;
};

/**----- BREAKPOINT -----**/

/* Liste simplement chainée de breakpoints */
struct bp{
  uint32_t addr;
  struct bp *suiv;
};

/**----- DECODE -----**/

/* Format des intstructions */
enum state_Format {R, I, J};

/**----- SIMULATEUR -----**/

/* Etats du simulateur */
enum state_SIMU {SIMU_wait, SIMU_notsupported, SIMU_help, SIMU_load, SIMU_dasm_pc, SIMU_dasm_n_pc, SIMU_dasm_all, SIMU_dreg_all, SIMU_dreg_one, SIMU_sreg, SIMU_stepi, SIMU_dmem_one, SIMU_dmem_several, SIMU_addbp, SIMU_dbp, SIMU_rmbp_all, SIMU_rmbp_one, SIMU_run, SIMU_step, SIMU_sshot, SIMU_smem};

/* Structure du simulateur */
struct sim{
  bool fin;                          /* Vaut true quand l'utilisateur entre la commande exit */
  bool execution_terminee;           /* Vaut true quand on a fini d'exécuter la partie .text */
  bool execution_warning_displayed;  /* Vaut true quand on a affiché le message d'avertissement (quand on a effectué plus de 5000 opérations) sécurité contre les boucles infinies) */
  bool file_loaded;                  /* Vaut true quand on a chargé un fichier */
  uint8_t *framebuffer_pt_hg;        /* Pointeur retourné par la fonction d'initialisation du framebuffer */
  bool execute_un_bloc;              /* Utilisée par jal et jr: vaut true quand on exécute jal puis vaut false quand on exécute jr  */
  struct cell *tete_lecture;         /* Pointe vers la case mémoire d'adresse $pc */
  uint32_t nbr_execution;            /* Nombres d'instructions exécutées par le simulateur depuis le chargement du fichier */
  uint32_t text_addr;                
  uint32_t text_size;
  struct bp *bp_liste;               /* Liste qui contient les breakpoints */
  char commande[4][63];              /* Tableau qui contient les commandes entrées par l'utilisateur */ 
  int32_t commande_num[3];           /* Tableau qui contient les commandes numériques entrées par l'utilisateur */
  enum state_SIMU SIMU_state;        
  enum state_MIPS MIPS_state;
  struct reg banc_reg[35];           /* 32 registres + pc,hi,lo */
  struct mem memoire;                /* mémoire du simulateur */
};

#endif

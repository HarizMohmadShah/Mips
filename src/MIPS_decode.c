#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "MIPS_decode.h"

// récupère les bits des registres après l'opcode
uint32_t recup_registre (uint32_t info, int decalage)
{
  uint32_t registre, temp;
  //on décale de 21 pour rs
  //------------ 16 pour rt
  //------------ 11 pour rd
  //------------ 6 pour sa
  registre = info >> decalage;
  temp = 0b11111;
  registre = temp & registre;
  return registre;
}

void decode_operande_R (struct cell *cellule)
{
  if (cellule == NULL){
    printf("Erreur: le pointeur vers la cellule à décoder est nul\n");
    return;
  }
  cellule -> rs = recup_registre(cellule -> val, 21);
  cellule -> rt = recup_registre(cellule -> val, 16);
  cellule -> rd = recup_registre(cellule -> val, 11);
  cellule -> sa = recup_registre(cellule -> val, 6);
}

void decode_operande_I (struct cell *cellule)
{
  /* Prob: Si le cas addi/addiu if faut faire l'extension de signe */
  if (cellule == NULL){
    printf("Erreur: le pointeur vers la cellule à décoder est nul\n");
    return;
  }
  cellule -> rs = recup_registre(cellule -> val, 21);
  cellule -> rt = recup_registre(cellule -> val, 16);
  cellule -> imm = cellule -> val << 16;
  cellule -> imm = cellule -> imm >> 16;
  if( (cellule -> etat_correspondant == MIPS_addi) || (cellule -> etat_correspondant == MIPS_addiu)){
    if ( (0x8000 & cellule -> imm) == 0x8000){
      cellule -> imm = cellule -> imm | 0xFFFF0000;
    }
  } 
  if (cellule -> etat_correspondant == MIPS_ori){
    cellule -> imm = cellule -> imm & 0x0000FFFF;
  }
  
  if(cellule -> etat_correspondant == MIPS_beq || cellule -> etat_correspondant == MIPS_bne || cellule -> etat_correspondant == MIPS_blez || cellule -> etat_correspondant == MIPS_bgtz){
    cellule -> rd = cellule -> addr + 4 + ((cellule -> imm) << 2);
  }
}

void decode_operande_J (struct cell *cellule)
{
  if (cellule == NULL){
    printf("Erreur: le pointeur vers la cellule à décoder est nul\n");
    return;
  }
  uint32_t temp;
  cellule -> instr_index = cellule -> val << 6;
  cellule -> instr_index = cellule -> instr_index >> 6;
  //L'affichage de l'adresse pour JAL
  temp = 0xF0000000;
  cellule->imm = (temp & (cellule->addr + 8))  | (cellule->instr_index << 2);
}


enum state_Format decode(struct cell *cellule)
{
  if (cellule == NULL){
    printf("Erreur: le pointeur vers la cellule à décoder est nul\n");
    return 0;
  }
  uint32_t temp;
  // champ opcode
  uint32_t opcode;
  temp = 0xFC000000;
  opcode = cellule -> val & temp;
  opcode = opcode >> 26;
  // champ function

  uint32_t funct;
  temp = 0b111111;
  funct = temp & cellule -> val;
  
  cellule -> etat_correspondant = 0; /* Sécurité, si jamais on n'arrive pas décoder la fonction on associe undefined comme fonction */
  cellule -> pf = NULL;
  switch(opcode)
  {
    // on tape directement le dernier octet en hexa, le reste est complété par des 0

    /* Format R*/
  case 0x10:
  case 0x00:
    switch(funct)
      {
      case 0x00:
	cellule -> etat_correspondant = MIPS_sll;
	cellule -> pf = sll;
	break;
      case 0x02:
	cellule -> etat_correspondant = MIPS_srl;
	cellule -> pf = srl;
	break;
      case 0x10:
	cellule -> etat_correspondant = MIPS_mfhi;
	cellule -> pf = mfhi;
	break;
      case 0x12:
	cellule -> etat_correspondant = MIPS_mflo;
	cellule -> pf = mflo;
	break;
      case 0x18:
	cellule -> etat_correspondant = MIPS_mult;
	cellule -> pf = mult;
	break;
      case 0x08:
	cellule -> etat_correspondant = MIPS_jr;
	cellule -> pf = jr;
	break;
      case 0x0C:
	cellule -> etat_correspondant = MIPS_syscall;
	cellule -> pf = syscall;
	break;
      case 0x1A:
	cellule -> etat_correspondant = MIPS_div;
	cellule -> pf = div_mips;
	break;
      case 0x20:
	cellule -> etat_correspondant = MIPS_add;
	cellule -> pf = add;
	break;  
      case 0x21:
	cellule -> etat_correspondant = MIPS_addu;
	cellule -> pf = addu;
	break;
      case 0x22:
	cellule -> etat_correspondant = MIPS_sub;
	cellule -> pf = sub;
	break;
      case 0x24:
	cellule -> etat_correspondant = MIPS_and;
	cellule -> pf = and;
	break;
      case 0x25:
	cellule -> etat_correspondant = MIPS_or;
	cellule -> pf = or;
	break;
      case 0x26:
	cellule -> etat_correspondant = MIPS_xor;
	cellule -> pf = xor;
	break;
      case 0x2A:
	cellule -> etat_correspondant = MIPS_slt;
	cellule -> pf = slt;
	break;
      default:
	NULL;
      }
    decode_operande_R(cellule);
    return R;
    break;

  /* Format I*/
  case 0x08:
    cellule -> etat_correspondant = MIPS_addi;
    cellule -> pf = addi;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x09:
    cellule -> etat_correspondant = MIPS_addiu;
    cellule -> pf = addiu;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x04: //valable pour "beq" et "b"
    cellule -> etat_correspondant = MIPS_beq; 
    cellule -> pf = beq;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x05:
    cellule -> etat_correspondant = MIPS_bne;
    cellule ->pf = bne;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x06:
    cellule -> etat_correspondant = MIPS_blez;
    cellule -> pf = blez;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x07:
    cellule -> etat_correspondant = MIPS_bgtz;
    cellule -> pf = bgtz;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x0A :
    cellule -> etat_correspondant = MIPS_slti;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x0B:
    cellule -> etat_correspondant = MIPS_sltiu;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x0D:
    cellule -> etat_correspondant = MIPS_ori;
    cellule -> pf = ori;
    decode_operande_I(cellule);
    return I; 
    break;
  case 0x0F:
    cellule -> etat_correspondant = MIPS_lui;
    cellule -> pf = lui;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x20:
    cellule -> etat_correspondant = MIPS_lb;
    cellule -> pf = lb;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x23:
    cellule -> etat_correspondant = MIPS_lw;
    cellule -> pf = lw;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x24:
    cellule -> etat_correspondant = MIPS_lbu;
    cellule -> pf = lbu;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x2B:
    cellule -> etat_correspondant = MIPS_sw;
    cellule -> pf = sw;
    decode_operande_I(cellule);
    return I;
    break;
  case 0x28:
    cellule -> etat_correspondant = MIPS_sb;
    cellule -> pf = sb;
    decode_operande_I(cellule);
    return I;
    break;
    
    /* Format J*/
  case 0x02:
    cellule -> etat_correspondant = MIPS_j;
    decode_operande_J(cellule);
    cellule -> pf = j;
    return J;
    break;
  case 0x03:
    cellule -> etat_correspondant = MIPS_jal;
    decode_operande_J(cellule);
    cellule -> pf = jal;
    return J;
    break;

  default:
    NULL;
  }
  return 0;
}

void display_operation(struct cell *cellule){
  if (cellule == NULL){
    return;
  }

char tab_noms[35][6] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra","$hi","$lo","$pc"};

  char correspondance_etat_MIPS_nom[34][10] = {"undefined","decode","add", "addu", "and", "addi", "addiu", "beq", "bne", "slti", "sltiu", "j", "jal", "or", "ori", "sll", "slt", "srl", "sub", "xor", "sw", "sb", "lui", "jr", "blez", "bgtz", "div", "lb", "lbu", "lw", "mfhi", "mflo", "mult", "SYSCALL"};
  enum state_Format format;
  format = decode(cellule);
  switch (format)
    {
      /*CASE R*/
    case R:
      if(cellule -> etat_correspondant == MIPS_jr){
	 printf("%s %s\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant], tab_noms[cellule -> rs]);
      }
      else if(cellule -> etat_correspondant == MIPS_sll ||cellule -> etat_correspondant == MIPS_srl){
	printf("%s %s, %s, %u\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant], tab_noms[cellule -> rd], tab_noms[cellule -> rt], cellule -> sa);
      }
      else if (cellule -> etat_correspondant == MIPS_mflo ||cellule -> etat_correspondant == MIPS_mfhi){
	printf("%s %s\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant], tab_noms[cellule -> rd]);
      }
      else if (cellule -> etat_correspondant == MIPS_mult || cellule -> etat_correspondant == MIPS_div){
	printf("%s %s, %s\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant], tab_noms[cellule -> rs], tab_noms[cellule -> rt]);	
      }
      else if (cellule -> etat_correspondant == MIPS_syscall){
	printf("%s\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant]);
      }
	else{
	//autre cas
      printf("%s %s, %s, %s\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant], tab_noms[cellule -> rd], tab_noms[cellule -> rs], tab_noms[cellule -> rt]);
      }     
      break;

      /* CASE I */
    case I:
      if(cellule -> etat_correspondant == MIPS_beq ||cellule -> etat_correspondant == MIPS_bne){
	/* cas BEQ et BNE */
	printf("%s %s, %s, 0x%08X\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant], tab_noms[cellule ->rs], tab_noms[cellule-> rt], cellule-> rd);
      }
      else if(cellule -> etat_correspondant == MIPS_bgtz || cellule -> etat_correspondant == MIPS_blez){
	/* cas BGTZ et BLEZ */
	printf("%s %s, 0x%08X\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant], tab_noms[cellule ->rs], cellule-> rd);
      }
      else if(cellule -> etat_correspondant == MIPS_sw || cellule -> etat_correspondant == MIPS_sb){
	  /* cas SW et SB */
	  printf("%s %s, 0x%X(%s)\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant], tab_noms[cellule ->rt],cellule -> imm,  tab_noms[cellule-> rs]);
      }  
      else if(cellule -> etat_correspondant == MIPS_lw || cellule -> etat_correspondant == MIPS_lbu || cellule -> etat_correspondant == MIPS_lb ){
	  /* cas LW, LBU et LB */
	  printf("%s %s, 0x%X(%s)\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant], tab_noms[cellule ->rt],cellule -> imm,  tab_noms[cellule-> rs]);
      }
      else{
	/* autre cas */
	printf("%s %s, %s, 0x%08X\n",correspondance_etat_MIPS_nom[cellule->etat_correspondant], tab_noms[cellule ->rt], tab_noms[cellule-> rs], cellule-> imm);
      }
      break;

      /* CASE J*/
    case J:
      printf("%s 0x%08X\n", correspondance_etat_MIPS_nom[cellule->etat_correspondant], cellule->imm);
      break;
    default: 
      NULL;
    }
}

void maj_champ_cellules(struct mem *memory){
  struct cell *cour = memory->tete;
  while (cour != NULL){
    decode(cour);
    cour = cour->suiv;
  }
}

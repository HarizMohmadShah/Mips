#ifndef _MIPS_DECODE_
#define _MIPS_DECODE_

#include <stdint.h>

#include "types.h"
#include "MIPS_pc.h"

// prend l'info de 32 bits et détermine le prochain état correspondant, 
// par exemple S_ADD
extern enum state_Format decode(struct cell *cellule);

extern uint32_t recup_registre (uint32_t info, int decalage);

// si on a détecté un format R avec decode
// on retourne les valeurs de rs, rt et rd contenue dans info
// /!\ ne pas oublier de rajouter sa après
extern void decode_operande_R (struct cell *cellule);

extern void decode_operande_I (struct cell *cellule);

extern void decode_operande_J (struct cell *cellule);

extern void display_operation(struct cell *cellule);

/* Parcours toutes les cases de la mémoire et met à jour tous les paramètres correspondants aux fonctions */
void maj_champ_cellules(struct mem *memory);

#endif 

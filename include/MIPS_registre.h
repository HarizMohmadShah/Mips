#ifndef _MIPS_registre_
#define _MIPS_registre_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "types.h"

/**----- REGISTRES -----**/

/* Initialise chaque registre: associe les noms, affecte 0 à chaque registre */
extern void init_banc_reg(struct reg banc[34]);

/* Affiche le nom du registre et la valeur contenue dans celui-ci, utilisé par DREG*/
extern void display_one_register(struct reg banc[34], char name[]);

/* Affiche tous les registres et les valeurs associées, utilisé par DREG*/
extern void display_all_register(struct reg banc[34]);

/* Associe au contenu du registre "name" la valeur val, utilisé par SREG */
extern void set_register(struct reg banc[34], char name[], long val);

#endif	 

#ifndef _MIPS_execution_
#define _MIPS_execution_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"
#include "MIPS_decode.h"
#include "MIPS_memoire.h"
#include "MIPS_registre.h"
#include "SIMU_commande.h"

/* Execute l'opération contenue dans la tete de lecture */
extern void executer(struct cell **tete_lecture, struct sim *cmd, bool step, bool stepi, bool run);

/* Fait avancer la tete de lecture,
Si l'opération exécutée ne faisait pas intervenir de branchement:
    PC<-PC+4
Sinon:
    La tete de lecture est positionnée à l'endroit voulu par le branchement
 */
extern void avancer(struct cell **tete_lecture, struct sim *cmd, bool step, bool stepi, bool run);

#endif 

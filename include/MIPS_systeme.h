#ifndef _MIPS_systeme_
#define _MIPS_systeme_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "types.h"
#include "MIPS_memoire.h"
#include "MIPS_registre.h"
#include "framebuffer.h"

/**----- SIMULATEUR -----**/

/* Initialise le simulateur toutes les variables du simulateur */
extern void init_sim(struct sim *simu, bool framebuffer_enable);

#endif

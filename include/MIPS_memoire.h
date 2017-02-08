#ifndef _MIPS_memoire_
#define _MIPS_memoire_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "types.h"
#include "MIPS_decode.h"

/**----- MEMOIRE -----**/

/* Initialise la mémoire (tete et queue à NULL) */
extern void init_mem(struct mem *pmem);

/* Crée une nouvelle case mémoire contenant l'adresse addr et la valeur val et l'insère en queue de la liste pointée par pmem */
extern void inserer_queue_mem(struct mem *pmem, uint32_t addr, uint32_t val, uint8_t *framebuffer_pt_hg);

/* Recherche une case mémoire d'adresse addr, si elle est trouvée la fonction renvoie un pointeur vers cette case */
extern struct cell *research_cell_mem(struct mem pmem, uint32_t addr);

/* Affiche la valeur contenue dans la case mémoire d'adresse addr */
extern void display_onecell_mem(struct mem pmem, uint32_t addr);

/* Affiche la valeur des cases mémoires dont l'adresse est contenue entre addr1 et addr2*/
extern void display_severalcell_mem(struct mem pmem, uint32_t addr1, uint32_t addr2, bool onebyone, bool disp_operation);

/* Modifie la valeur contenue à l'adresse addr par value */
extern void set_cell_mem(struct mem *pmem, uint32_t addr, uint32_t value);

/* Libère la mémoire utilisée par la liste pointée par mem */
extern void liberer_mem(struct mem *pmem);

#endif

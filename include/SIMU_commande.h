#ifndef _SIMU_commande_
#define _SIMU_commande_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "types.h"
#include "elf_reader.h"
#include "relocation.h"
#include "framebuffer.h"
#include "MIPS_memoire.h"

enum section{
  TEXT,
  DATA,
  BSS
};

struct memoire{
  uint32_t add;
  uint32_t Code;
};

/* Recupère la commande de l'utilisateur */
extern void recuperer_commande(char commande[3][63]);

/* met à jour SIMU_state en fonction de la commande désirée, si la commande est exit alors fin vaut true en sortie */
extern void update_SIMU_state(enum state_SIMU *state, bool *fin, char commande[3][63], int32_t commande_num[3]);

/* Affichage les données sous le format dasm */
void display_mem(struct mem memoir, uint32_t addr_init, uint32_t addr_fin);

/* Insertion dans la structure des données */
extern void inserer_memoire(struct mem *memoir, uint32_t size, uint32_t addr, uint8_t *bytes, enum section type);

/* Pour la commande load <fichier (ELF> */
extern bool load_file(uint32_t text_addr_param, uint32_t data_addr_param, uint32_t taille, struct mem *memory, const char* file, 
		      uint32_t *text_addr, uint32_t *data_addr, uint32_t *bss_addr, 
		      size_t *text_size, size_t *data_size, size_t *bss_size);

/* Pour la commande dasm */
extern void display_deassembler(struct mem memory, uint32_t text_addr, uint32_t data_addr, uint32_t bss_addr, uint32_t text_size, uint32_t data_size, uint32_t bss_size);

/* Recuperation des parametres entrées */
extern bool init_adresse(int argc, char *argv[], uint32_t *text_addr, uint32_t *data_addr, uint32_t *taille, bool *frame, uint32_t ADD_DEFAUT, uint32_t TAILLE_DEFAUT);

/* Insère un bp en tete de la liste de bp */
extern void insert_bp(struct bp **bp, uint32_t addr);

/* Affiche la liste de bp */
extern void afficher_bp(struct bp *bp);

/* Supprime la premiere occurence de addr dans la liste de bp */
extern void delete_bp(struct bp **bp, uint32_t addr);

/* Retourne vrai ssi le bp d'adresse addr est présent dans la liste bp */
extern int est_present_bp(struct bp *bp, uint32_t addr);

/* Libère la liste de bp */
extern void free_bp(struct bp **bp);

/* Fonction pour capturer l'écran du framebuffer */
extern void sshot(uint8_t *fr, uint32_t *counter);

/* Retourne vrai si str pointe vers un entier (positif ou négatif) */
extern bool is_valid_int(char *str);

/* Retourne vrai si str pointe vers entier sous forme hexadecimale */
extern bool is_valid_hex(char *str);

/* Retourne l'entier hexadécimal codé dans la chaine de caractère str */
extern int32_t string_to_hex(char *str);

/* fonction pour modifier la mémoire */
extern bool smem(struct mem *pmem,uint32_t address, uint32_t val, uint32_t octet);

/* Aide du simulateur */
extern void help(char commande[4][63]);

#endif	

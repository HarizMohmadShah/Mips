#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "SIMU_commande.h"

void recuperer_commande(char commande[4][63]){
  printf("Entrer une commande:");
  char tampon[127];
  fgets(tampon,sizeof(tampon),stdin);
  //strtok(tampon,"\n");
  //fflush(stdout);
  uint32_t i = 0;
  uint32_t k = 0;
  if (strcmp(commande[0],"") == 0 || !(strcmp(commande[0],"") != 0 && strcmp(tampon,"\n") == 0)){
    for (uint32_t i = 0; i < 4; i++){
      strncpy(commande[i],"",64);
    }
    for (uint32_t j = 0; j < 128; j++){
      if (tampon[j] != ' ' && tampon[j] != '\n'){
	commande[i][k] = tampon[j];
	k++;
      }
      else if (tampon[j] == ' '){
	i++;
	k = 0;
      }
      else if (tampon[j] == '\n'){
	break;
      }
    }
  }
}

void update_SIMU_state(enum state_SIMU *state, bool *fin, char commande[4][63], int32_t commande_num[3]){
  bool test_presence[3];
  test_presence[0] = (strcmp(commande[1],"") == 0) && (strcmp(commande[2],"") == 0) && (strcmp(commande[3],"") == 0);
  test_presence[1] = (strcmp(commande[2],"") == 0) && (strcmp(commande[3],"") == 0);
  test_presence[2] = (strcmp(commande[3],"") == 0);
  if (strcmp(commande[0],"help") == 0 && test_presence[1]){
    *state = SIMU_help;
  }
  else if (strcmp(commande[0],"exit") == 0 && test_presence[0]){
    *fin = true;
  } 
  else if (strcmp(commande[0],"sshot") == 0 && test_presence[0]){
    *state = SIMU_sshot;
  }
  else if (strcmp(commande[0],"load") == 0 && test_presence[1]){
    if (strcmp(commande[1],"") == 0){
      printf("Erreur: paramètre <filename> manquant\n");
      *state = SIMU_wait;
    }else{
      *state = SIMU_load;
    }
  }
  else if (strcmp(commande[0],"dasm") == 0 && test_presence[0]){
     *state = SIMU_dasm_pc;
  }
  else if (strcmp(commande[0],"dasm") == 0 && test_presence[1]){
    if (strcmp(commande[1],"all") == 0) {
      *state = SIMU_dasm_all;
    }else if (is_valid_int(commande[1])){
      commande_num[0] = atoi(commande[1]);
      *state = SIMU_dasm_n_pc;
    }else {
      *state = SIMU_notsupported;
    }
  }
  else if (strcmp(commande[0],"dreg") == 0 && test_presence[0]){
     *state = SIMU_dreg_all;
  }
  else if (strcmp(commande[0],"dreg") == 0 && test_presence[1]){
     *state = SIMU_dreg_one;
  }
  else if (strcmp(commande[0],"sreg") == 0 && test_presence[2]){
    if (is_valid_int(commande[2])){
      if (*commande[2] == '-'){
	/* commande[2]+1 pour ne pas tenir compte du - */
	commande_num[0] = -1*atoi(commande[2]+1);
      }else{
	commande_num[0] = atoi(commande[2]);
      }
      *state = SIMU_sreg;
    }else if (is_valid_hex(commande[2])){ 
      commande_num[0] = (int)strtol(commande[2],NULL,0);
      *state = SIMU_sreg;
    }else {
      *state = SIMU_notsupported;
    }
  }
  else if (strcmp(commande[0],"dmem") == 0 && test_presence[1]){
    if (is_valid_hex(commande[1])){ 
      commande_num[0] = (int)strtol(commande[1],NULL,0);
      *state = SIMU_dmem_one;
    }else {
      printf("Erreur: <address> doit etre une valeur hexadécimale\n");
      *state = SIMU_notsupported;
    }
  }
  else if (strcmp(commande[0],"dmem") == 0 && test_presence[2]){
    if (is_valid_hex(commande[1]) && is_valid_hex(commande[2])){ 
      commande_num[0] = (int)strtol(commande[1],NULL,0);
      commande_num[1] = (int)strtol(commande[2],NULL,0);
      *state = SIMU_dmem_several;
    }else {
     printf("Erreur: <address1> et <address2> doivent etre des valeurs hexadécimales\n");
     *state = SIMU_notsupported;
    }
  }
  else if (strcmp(commande[0],"run") == 0 && test_presence[0]){
     *state = SIMU_run;
  }
  else if (strcmp(commande[0],"stepi") == 0 && test_presence[0]){
     *state = SIMU_stepi;
  }
  else if (strcmp(commande[0],"step") == 0 && test_presence[0]){
     *state = SIMU_step;
  }
  else if (strcmp(commande[0],"addbp") == 0 && test_presence[1]){
     if (is_valid_hex(commande[1])){
       commande_num[0] = (int)strtol(commande[1],NULL,0);
       *state = SIMU_addbp;
     }else{
       printf("Erreur: <address> doit etre une valeur hexadécimale\n");
       *state = SIMU_notsupported;
     }
  }
  else if (strcmp(commande[0],"dbp") == 0 && test_presence[0]){
     *state = SIMU_dbp;
  }
  else if (strcmp(commande[0],"rmbp") == 0 && test_presence[0]){
    *state = SIMU_rmbp_all;
  }
  else if (strcmp(commande[0],"rmbp") == 0 && test_presence[1]){
     if (is_valid_hex(commande[1])){
       commande_num[0] = (int)strtol(commande[1],NULL,0);
       *state = SIMU_rmbp_one;
     }else{
       printf("Erreur: <address> doit etre une valeur hexadécimale\n");
       *state = SIMU_notsupported;
     }
  }
  else if (strcmp(commande[0],"smem") == 0 && (test_presence[2] || !test_presence[0])){
    if(!test_presence[1]){
      if(is_valid_hex(commande[1])){
	commande_num[0] = (int)strtol(commande[1],NULL,0);
	if(is_valid_hex(commande[2]) || is_valid_int(commande[2])){
	  if(is_valid_hex(commande[2])){
	    commande_num[1] = (int)strtol(commande[2],NULL,0);
	  }else{
	    commande_num[1] = atoi(commande[2]); 
	  }
	  if(!test_presence[2]){
	    if(is_valid_int(commande[3])){
	      commande_num[2] = atoi(commande[3]);
	      if(commande_num[2] == 1 || commande_num[2] == 2 || commande_num[2] == 4 ){
		*state = SIMU_smem;
	      }
	      else{
		printf("Erreur: Les nombres d'octets valides sont 1,2 ou 4\n");
		*state = SIMU_notsupported;
	      }
	    }else{
	      printf("Erreur: Les nombres d'octets valides sont 1,2 ou 4\n");
	      *state = SIMU_notsupported;
	    }
	  }else{
	    commande_num[2] = 1;
	    *state = SIMU_smem;
	  }
	}else{
	  printf("Erreur: <valeur> doit etre une valeur hexadécimale ou une valeur décimale\n");
	  *state = SIMU_notsupported;
	}
      }else{
	printf("Erreur: <address> doit etre une valeur hexadécimale\n");
	*state = SIMU_notsupported;
      }
    }else{
      printf("Erreur: l'utilisation smem <address> <valeur>\n");
      *state = SIMU_notsupported;
    }
  }
  else{
    *state = SIMU_notsupported;
  }
}

void inserer_memoire(struct mem *memoir, uint32_t size, uint32_t addr, uint8_t *bytes, enum section type){
  uint32_t code = 0;
  uint32_t i;
  if((type == TEXT) || (type == DATA)){
    for(i = 0; i < size; i++){
      if( i % 4 == 0){
	code = bytes[i] << 24;
      }
      if( i % 4 == 1){
	code = code | ( bytes[i] << 16 );
      }
      if( i % 4 == 2){
	code = code | ( bytes[i] << 8 );
      }
      if( i % 4 == 3){
	code = code | bytes[i];
	inserer_queue_mem(memoir, addr, code, NULL);
	addr = addr + 4;
      }
    }
  }else{
    for(i = 0 ; i < size; i+=4){
      code = 0;
      inserer_queue_mem(memoir, addr, code, NULL);
      addr = addr + 4;
    }
  }
}

bool load_file(uint32_t text_addr_param, uint32_t data_addr_param, uint32_t taille, struct mem *memory, const char* file, uint32_t *text_addr, uint32_t *data_addr, uint32_t *bss_addr, size_t *text_size, size_t *data_size, size_t *bss_size){
  struct elf_descr *elf;
  uint8_t *text_bytes = NULL;
  uint8_t *data_bytes = NULL;
  uint8_t *bss_bytes = NULL; //tjrs NULL
  uint8_t text_align = 0;
  uint8_t data_align = 0;
  uint8_t bss_align = 0;
  uint16_t type; 
  Elf32_Rel *rel_text_bytes = NULL;
  size_t rel_text_size = 0;
  Elf32_Rel *rel_data_bytes = NULL;
  Elf32_Sym *symtab = NULL;
  size_t rel_data_size = 0;
  char *strtab = NULL;
  size_t strtab_size = 0;
  size_t symtab_size = 0;
  size_t nb_symboles = 0;
  struct symbole *tab_symboles = NULL;

  printf("Lecture du fichier %s...\n", file);
  elf = read_elf(file);
  if (elf == NULL) {
    printf("Erreur de lecture ELF\n");
    return false;
  }
  printf("Chargement de %s ....", file);
  type = get_elf_type(elf);

  get_text_section(elf, &text_bytes, text_size, text_addr, &text_align);
  get_data_section(elf, &data_bytes, data_size, data_addr, &data_align);
  get_bss_section(elf, bss_size, bss_addr, &bss_align);
  switch(type){
  case ET_REL:
    printf("Le fichier est relogeable\n");
    /* cas si data address est plus petit que text address */
    *text_addr = text_addr_param;
    if(text_addr_param > data_addr_param){
      while(*text_addr <= data_addr_param + *data_size + *bss_size){
	*text_addr = *text_addr + 0x1000;
      }
    }
 
    /* cas si la section text est tres grand */
    *data_addr = data_addr_param;
    if(data_addr_param > text_addr_param){
      while(*data_addr <= text_addr_param + *text_size){
	*data_addr = *data_addr + 0x1000;
      }
    }
   
    *bss_addr = *data_addr + *data_size;
    printf("L'adresse d'implantation de la section .text? (Ox%X)\n", *text_addr);
    printf("L'adresse d'implantation de la section .data? (Ox%X)\n", *data_addr);
    get_rel_text_section(elf, &rel_text_bytes, &rel_text_size);
    get_rel_data_section(elf, &rel_data_bytes, &rel_data_size);
    get_string_table(elf, &strtab, &strtab_size);
    get_symbol_table(elf, &symtab, &symtab_size);
    traduit_table_symboles(symtab, symtab_size, strtab, &tab_symboles, &nb_symboles);
    reloge_symboles(symtab, symtab_size, strtab, *text_addr, *data_addr, *bss_addr, &tab_symboles, &nb_symboles); 
    reloge_section(*text_addr, text_bytes, rel_text_bytes, rel_text_size, tab_symboles); 
    reloge_section(*data_addr, data_bytes, rel_data_bytes, rel_data_size, tab_symboles);
    break;
  case ET_EXEC:
    printf("Le fichier est executable\n"); 
    break;
  default:
    NULL;
  }

  if(*text_addr > taille - 1){
    printf("L'adresse de .text est en dehors de la plage l'adresse du simulateur [0x%08X;0x%08X]. Le fichier ne peut pas être éxecuté\n", 0 ,taille - 1);
    return false;
  }
    
  if(*data_addr > taille - 1){
    printf("L'adresse de .data est en dehors de la plage l'adresse du simulateur [0x%08X;0x%08X]. Le fichier ne peut pas être éxecuté\n", 0 ,taille - 1);
    return false;
  }

  if(*bss_addr > taille - 1){
    printf("L'adresse de .bss est en dehors de la plage l'adresse du simulateur [0x%08X;0x%08X]. Le fichier ne peut pas être éxecuté\n", 0 ,taille - 1);
    return false;
  }

  printf("Section .text => %zu octets implantes a l'adresse 0x%X\n",*text_size, *text_addr);  
  printf("Section .data => %zu octets implantes a l'adresse 0x%X\n",*data_size, *data_addr);  
  printf("Section .bss => %zu octets implantes a l'adresse 0x%X\n",*bss_size, *bss_addr);
  inserer_memoire(memory, *text_size, *text_addr, text_bytes, TEXT);
  inserer_memoire(memory, *data_size, *data_addr, data_bytes, DATA); 
  inserer_memoire(memory, *bss_size, *bss_addr, bss_bytes, BSS);
  close_elf(elf);
  return true;
}

void display_deassembler(struct mem memory, uint32_t text_addr, uint32_t data_addr, uint32_t bss_addr, uint32_t text_size, uint32_t data_size, uint32_t bss_size){
  printf("=======>Section .text\n");
  display_severalcell_mem(memory, text_addr, text_addr + text_size - 4,true,true);
  printf("=======>Section .data\n");
  display_severalcell_mem(memory, data_addr, data_addr + data_size - 4,true,false);
  printf("=======>Section .bss\n");
  display_severalcell_mem(memory, bss_addr, bss_addr + bss_size,true,false);
}

bool init_adresse(int argc, char *argv[], uint32_t *text_addr, uint32_t *data_addr, uint32_t *taille, bool *frame, uint32_t ADD_DEFAUT, uint32_t TAILLE_DEFAUT){
  bool text = false;
  bool data = false;
  bool btaille = false;

  for(uint32_t i = 1; i < argc; i++){

    if(strcmp(argv[i],"-t") == 0){
      i++;
      if(argv[i] == NULL){
	printf("ERREUR: IL N'Y A PAS DE VALEUR POUR -t. LE SIMULATEUR S'ARRETE\n");
	return false;
      }
      else{
	if (is_valid_hex(argv[i])){
	  *text_addr = string_to_hex(argv[i]);
	}else{
	  printf("L'adresse saisie n'est pas une valeur hexadecimal. D'où le simulateur prendre l'adresse 0x%X par defaut\n", ADD_DEFAUT);
	  *text_addr = ADD_DEFAUT;
	}
	text = true;
      }
    }

    if(strcmp(argv[i],"-d") == 0){
      i++;
      if(argv[i] == NULL){
	printf("ERREUR: IL N'Y A PAS DE VALEUR POUR -d. LE SIMULATEUR S'ARRETE\n");
	return false;
      }
      else{
	if (is_valid_hex(argv[i])){
	  *data_addr = string_to_hex(argv[i]);
	}else{
	  if(text){
	    printf("L'adresse saisie n'est pas une valeur hexadecimal. D'où le simulateur prendre l'adresse 0x%X par defaut\n", *text_addr + 0x1000);
	    *data_addr = *text_addr + 0x1000;
	  }
	  else{
	    *data_addr = ADD_DEFAUT + 0x1000;
	  }
	}
	data = true;
      }
    }

    if(strcmp(argv[i],"-s") == 0){
      i++;
      if(argv[i] == NULL){
	printf("ERREUR: IL N'Y A PAS DE VALEUR POUR -s. LE SIMULATEUR S'ARRETE\n");
	return false;
      }else{
	if (is_valid_hex(argv[i])){
	  *taille = string_to_hex(argv[i]);
	}else{
	  printf("La taille  saisie n'est pas une valeur hexadecimal. D'où le simulateur prendre l'adresse 0x%X entre 0x%X et 0x%X par defaut\n", TAILLE_DEFAUT, 0x000000, 0xFFFFFF);
	  *taille = TAILLE_DEFAUT;
	}
	btaille = true;
      }
    }

    if(strcmp(argv[i],"-fb") == 0){
      *frame = true;
    }
      
    if(strcmp(argv[i],"-no-fb") == 0){
      *frame = false;
    }
  }

  if(!btaille){
    *taille = TAILLE_DEFAUT;
  }

  if(!text && !data){
    *text_addr = ADD_DEFAUT;
    *data_addr = *text_addr + 0x1000;
  }

  if(!text && data){
    printf("ERREUR: IL N'Y A PAS DE VALEUR POUR -t. LE SIMULATEUR S'ARRETE\n");
    return false;
  }

  if(text && data){
    if(*data_addr == ADD_DEFAUT + 0x1000){
      *data_addr = *text_addr + 0x1000;
    }
  }

  if(text && !data){
    *data_addr = *text_addr + 0x1000;
  }

  return true;
}

void insert_bp(struct bp **bp, uint32_t addr)
{
  if (est_present_bp(*bp,addr)){
    printf("Breakpoint à l'adresse 0x%X déjà activé\n",addr);
    return;
  }
  struct bp *new = malloc(sizeof(struct bp));
  new -> addr = addr;
  new -> suiv = *bp;
  *bp = new;
}

void afficher_bp(struct bp *bp){
  if (bp == NULL){
    printf("Aucun breakpoint n'est activé\n");
    return;
  }
  printf("Breakpoints activés aux adresses: ");
    while (bp != NULL) {
        printf("0x%X", bp->addr);
	if (bp->suiv != NULL){
	  printf(",");
	}
        bp = bp->suiv;
    }
    printf("\n");
}

extern int est_present_bp(struct bp *bp, uint32_t addr){
  struct bp *temp = bp;
  while (temp != NULL){
    if (temp->addr == addr){
      return 1;
    }
    temp = temp->suiv;
  }
  return 0;
}

void delete_bp(struct bp **bp, uint32_t addr)
{
  /* Cette fonction prend en argument un pointeur sur la liste, car cette
   * dernière change lorsqu'on supprime la première cellule */
  struct bp sent = { -1, *bp };
  struct bp *p = &sent;
  /* En C, les conditions sont évaluées séquentiellement. L'expression à
   * droite d'une condition logique && n'est évaluée que si l'expression à
   * gauche est vraie.*/
  while (p->suiv != NULL && p->suiv->addr != addr) {
    p = p->suiv;
  }

  /* Cas occurence trouvée */
  if (p->suiv != NULL) {
    /* On rechaine les 2 cellules de la liste entourant l'occurrence et on
     * libère la cellule trouvée */
    struct bp *style = p->suiv;
    p->suiv = style->suiv;
    free(style);
  }
  *bp = sent.suiv;
}

void free_bp(struct bp **bp)
{
  struct bp *temp = NULL;
  while (*bp != NULL){
    temp = *bp;
    *bp = (*bp)->suiv;
    free(temp);
  }
}

void sshot(uint8_t *fr, uint32_t *counter){
  FILE *ppm;
  char *nb_counter = NULL;
  uint32_t size_counter = 0;
  char nom[20];

  nom[0] = 'i';
  nom[1] = 'm';
  nom[2] = 'a';
  nom[3] = 'g';
  nom[4] = 'e';
  size_counter = log10(*counter) + 2;
  nb_counter = malloc(size_counter);
  snprintf(nb_counter, size_counter, "%d", *counter);
  for(uint32_t i = 0; i < size_counter; i++){
    nom[i+5] = nb_counter[i];
  }
  free(nb_counter);
  nom[4 + size_counter] = '.';
  nom[5 + size_counter] = 'p';
  nom[6 + size_counter] = 'p';
  nom[7 + size_counter] = 'm';
  nom[8 + size_counter] = '\0';
  printf("%s\n", nom);
  ppm = fopen(nom, "w");
  fprintf(ppm, "%c", 'P');
  fprintf(ppm, "%c", '6');
  fprintf(ppm, "%c", '\n');
  fprintf(ppm, "%c", FRAMEBUFFER_W/100 + 48);
  fprintf(ppm, "%c", FRAMEBUFFER_W/10%10 + 48);
  fprintf(ppm, "%c", FRAMEBUFFER_W%100%10 + 48);
  fprintf(ppm, "%c", ' ');
  fprintf(ppm, "%c", FRAMEBUFFER_H/100 + 48);
  fprintf(ppm, "%c", FRAMEBUFFER_H/10%10 + 48);
  fprintf(ppm, "%c", FRAMEBUFFER_H%100%10 + 48);
  fprintf(ppm, "%c", '\n');
  fprintf(ppm, "%c", '2');
  fprintf(ppm, "%c", '5');
  fprintf(ppm, "%c", '5');
  fprintf(ppm, "%c", '\n');

  for(uint32_t i = 0; i < FRAMEBUFFER_H; i++){
    for(uint32_t j = 0; j < FRAMEBUFFER_W; j++){
      for(uint32_t k = 0; k < 3; k++){
	fprintf(ppm, "%c",fr[i*FRAMEBUFFER_W + j]);
      }
    }
  }

  fclose(ppm);
  *counter = *counter + 1;
}

bool is_valid_int(char *str)
{
  // Handle negative numbers.
  if (*str == '-')
    ++str;
  // Handle empty string or just "-".
  if (!*str){
    return false;
  }
  // Check for non-digit chars in the rest of the string.
  while (*str)
    {
      if (!isdigit(*str)){
	return false;
      }else{
	++str;
      }
    }
  return true;
}

bool is_valid_hex(char *str)
{
  // Handle hex numbers.
  if (*str == '0'){
    ++str;
    if (*str == 'x'){
      ++str;
    }
  }else{
    return false;
  }
  if (*str == '\0')
    return false;
  // Check for non-digit chars in the rest of the string.
  while (*str)
    {
      if (!isdigit(*str) && (*str != 'a') && (*str != 'b') && (*str != 'c') && (*str != 'd') && (*str != 'e') && (*str != 'f') && (*str != 'A') && (*str != 'B') && (*str != 'C') && (*str != 'D') && (*str != 'E') && (*str != 'F')){
	return false;
      }else{
	++str;
      }
    }
  return true;
}

int32_t string_to_hex(char *str){
  int32_t entier = 0;
  if (is_valid_hex(str)){ 
    entier = (int)strtol(str,NULL,0);
  }
  return entier;
}

bool smem(struct mem *pmem,uint32_t address, uint32_t val, uint32_t octet){
  uint32_t temp = val;
  struct cell *found = research_cell_mem(*pmem,address);

  if(found == NULL){
    printf("Erreur: L'adresse saisie n'est pas valide\n");
    return false;
  }

  for(uint32_t i = 0; i < octet ; i++){
    temp = temp >> 8;
  }

  if(temp != 0){
    printf("Erreur: Le nombre octet saisie n'est pas suffisant\n");
    return false;
  }

  switch(octet){
  case 1:
    temp = val << 24;
    found -> val = temp;
    break;
  case 2:
    temp = val << 16;
    found -> val = temp;
    break;
  case 4:
    temp = val;
    found -> val = temp;
    break;
  default:
    NULL;
  }
  return true;
}

void help(char commande[4][63]){
  bool has_printed_smth = false;
  printf("Aide du simulateur:\n");
  if (strcmp(commande[1],"load") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande load-----\n");
    printf("> Syntaxe : load <filename>\n> Paramètres :\nfilename : nom du fichier objet à charger\n> Description : Les données lues dans le fichier filename sont placées en mémoire enrespectant les contraintes sur les adresses de sections vues en 6.1.1.\nLe fichier <filename> doit être au format ELF, exécutable ou relogeable sans symbole externe.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"dasm") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande dasm-----\n");
    printf("> Syntaxe : dasm [n | all]\n> Description : Affiche dans la console le code désassemblé, c’est-à-dire les instructions en langage assembleur correspondant au code binaire.\n> Paramètres :\nSans option, dasm désassemble l’instruction courante à l’adresse $pc.\nAvec un paramètre entier n : désassemble n instructions à partir de $pc.\nAvec all : désassemble la totalité des trois zones .text, .data et .bss.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"dreg") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande dreg-----\n");
    printf("> Syntaxe : dreg [<registerName>]\n> Paramètres :\nregisterName (optionnel) : nom d’un des registres\n> Description : Affiche dans la console, au format hexadécimal, la valeur du registre donnéen paramètre. S’il n’y a aucun paramètre, dreg affiche les valeurs de chacun des registres.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"sreg") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande sreg-----\n");
    printf("> Syntaxe : sreg <registerName> <value>\n> Paramètres :\nregisterName : nom d’un registre.\nvalue : valeur numérique valide, entrée au format hexadécimal (préfixe 0x) ou décimale.\n> Description : Écrit la valeur donnée dans le registre passé en paramètre.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"dmem") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande dmem-----\n");
    printf("> Syntaxes : dmem <address> ou dmem <address1> <address2>\n> Paramètres :\naddress : valeur hexadécimale représentant une adresse sur 32 bits.\n> Description : Cette primitive affiche sur la console le contenu de la mémoire. L’affichage se fera à raison de 16 octets par ligne séparés par des espaces (un octet sera affiché par deux chiffres hexadécimaux). Chaque ligne commencera par l’adresse hexadécimale (32 bits) de l’octet le plus à gauche de la ligne. 8 La commande dmem <address> affiche 3 (par exemple) lignes de 16 octets à partir de l’adresse donnée. dmem <address1>-<address2> affiche tous les octets entre les deux adresses en paramètres.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"smem") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande smem-----\n");
    printf("> Syntaxe : smem <address> <value> [nbOctets]\n> Paramètres :\naddress : valeur représentant une adresse mémoire valide\nvalue : valeur numérique, au format hexadécimale ou décimale.\nnbOctets : paramètre optionnel indiquant le nombre d’octets à écrire en mémoire (1,2ou 4). S’il n’est pas présent, un seul octet est écrit.\n> Description : Écrit dans la mémoire, à l’adresse fournie en paramètre, la valeur fournie en paramètre. Selon la valeur de l’argument nbOctets, 1, 2 ou 4 octets seront écrits en mémoire à partir de address. Les valeurs value et nbOctets devront être compatibles.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"sshot") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande sshot-----\n");
    printf("> Syntaxe : sshot <fichier.ppm>\n> Paramètres :\nfichier.ppm : nom du fichier utilisé pour sauvegarder la copie d’écran. La sauvegarde est réalisée au format ppm.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"run") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande run-----\n");
    printf("> Syntaxe : run [<address>]\n> Paramètres :\naddress : valeur hexadécimale (facultatif)\n> Description : Charge PC avec l’adresse fournie en paramètre et lance le microprocesseur.Si le paramètre est omis, l’exécution commence à la valeur courante de PC.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"step") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande step-----\n");
    printf("> Syntaxe : step\n> Description : Provoque l’exécution de l’instruction dont l’adresse est contenue dans le registre PC puis rend la main à l’utilisateur. Si l’on rencontre un appel à une procédure avec retour (JAL), cette dernière s’exécute complètement jusqu’à l’instruction de retour incluse (JR $ra). La main est alors rendue à l’utilisateur sur l’instruction suivant le saut (donc celle après son delay slot).\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"stepi") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande stepi-----\n");
    printf("> Syntaxe : stepi\n> Description : Cette primitive provoque l’exécution de l’instruction dont l’adresse est contenue dans le registre PC puis rend la main à l’utilisateur. Si l’on rencontre un saut, seule l’instruction dans le delay slot puis le saut sont exécutés.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"addbp") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande addbp-----\n");
    printf("> Syntaxe : addbp <address>\n> Paramètre :\naddress : valeur hexadécimale d’adresse\n> Description : Ajoute un point d’arrêt à l’adresse fournie en paramètre. Lorsque le compteur ordinal PC sera égal à cette valeur, l’exécution sera interrompue et la main rendue à l’utilisateur.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"rmbp") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande rmbp-----\n");
    printf("> Syntaxe : rmbp [<address>]\n> Paramètres :\naddress : valeur hexadécimale d’adresse (facultative).\n> Description : Enlève le point d’arrêt à l’adresse fournie en paramètre, s’il y en a un. Si le paramètre est omis, cette commande efface tous les points d’arrêt.\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"dbp") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande dbp-----\n");
    printf("> Syntaxe : dbp\n> Description : Affiche sur la console de visualisation l’adresse de tous les points d’arrêtpositionnés dans la mémoire.\n");
    has_printed_smth = true;
  }    
  if (strcmp(commande[1],"help") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande help-----\n");
    printf("> Syntaxe : help [<command>]\n> Paramètres :\ncommand : nom de la commande sur laquelle l’aide est demandée.\n> Description : Affiche l’aide d’une commande et sa syntaxe. Si aucun argument n’est fourni,affiche la liste de toutes commandes du simulateur avec une description très brève (moins d’une ligne).\n");
    has_printed_smth = true;
  }
  if (strcmp(commande[1],"exit") == 0 || strcmp(commande[1],"") == 0){
    printf("-----Commande exit-----\n");
    printf("> Syntaxe : exit\n> Description : Quitte le simulateur.\n");
    has_printed_smth = true;
  }
  if (has_printed_smth == false){
    printf("Erreur: \"%s\" n'est pas une commande gérée par le simulateur\n", commande[1]);
  }
}

#ifndef _MIPS_pc_
#define _MIPS_pc_

#include <stdint.h>
#include <stdio.h>

#include "types.h"
#include "MIPS_memoire.h"
#include "MIPS_registre.h"
#include "SIMU_commande.h"

// banc[34] car on a les 32 registres "classiques" 
// et 3 autres registres hi, lo et pc
extern void add(struct reg banc[34], struct cell *cell);

extern void addi(struct reg banc[34], struct cell *cell);

extern void addiu(struct reg banc[34], struct cell *cell);

extern void addu(struct reg banc[34], struct cell *cell);

extern void and(struct reg banc[34], struct cell *cell);

extern void beq(struct reg banc[34], struct cell *cell);

extern void or(struct reg banc[34], struct cell *cell);

extern void ori(struct reg banc[34], struct cell *cell);

extern void sll(struct reg banc[34], struct cell *cell);

extern void slt(struct reg banc[34], struct cell *cell);

extern void srl(struct reg banc[34], struct cell *cell);

extern void sub(struct reg banc[34], struct cell *cell);

extern void xor(struct reg banc[34], struct cell *cell);

extern void jal(struct reg banc[34], struct cell *cell, bool *execute_un_bloc);

extern void sw(struct reg banc[34], struct cell *cell, struct mem *pmem, uint8_t *framebuffer_pt_hg);

extern void sb(struct reg banc[34], struct cell *cell, struct mem *pmem);

extern void lui(struct reg banc[34], struct cell *cell);

extern void jr(struct reg banc[34], struct cell *cell, bool *execute_un_bloc);

extern void bne(struct reg banc[34], struct cell *cell);

extern void blez(struct reg banc[34], struct cell *cell);

extern void bgtz(struct reg banc[34], struct cell *cell);

extern void div_mips(struct reg banc[34], struct cell *cell);

extern void j(struct reg banc[34], struct cell *cell);

extern void lb(struct reg banc[34], struct cell *cell, struct mem *pmem);

extern void lbu(struct reg banc[34], struct cell *cell, struct mem *pmem);

extern void lw(struct reg banc[34], struct cell *cell, struct mem *pmem);

extern void mfhi(struct reg banc[34], struct cell *cell);

extern void mflo(struct reg banc[34], struct cell *cell);

extern void mult(struct reg banc[34], struct cell *cell);

extern void syscall(struct reg banc[34], struct cell *cell, struct mem *pmem);

#endif 

.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
lw $t0, X
addi $t1,$0,0x2
mult $t0,$t1
mfhi $t2
mflo $t3
	
# max_cycle 50
# pout_start
# 0000000A
# pout_end

end:                # The End!

.data
X:	 .word 0xFFFFFFFF
	
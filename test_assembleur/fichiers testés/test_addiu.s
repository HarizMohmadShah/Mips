.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
lw $t0, X
addiu $t1,$t0, -12345
ori $t2,$t1, 0xA0A1
ori $t3,$zero, 0x0BF1
add $t4,$t2,$t3
	
# max_cycle 50
# pout_start
# 00000002	
# 00000C92	
# pout_end

end:                # The End!

.data
X:	.word 0x000F0000
	
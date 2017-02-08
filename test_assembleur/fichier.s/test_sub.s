.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
ori $t1,$0, 0x0001
sub $28,$t1,$t1
addi $t3,$0, 0x1
addi $t4,$0, 0x10
sub $28,$t3,$t4
	
# max_cycle 50
# pout_start
# 00000000
# FFFFFFF1	
# pout_end

end:                # The End!


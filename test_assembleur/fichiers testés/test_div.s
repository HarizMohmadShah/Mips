.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
addi $t0,$zero, 10
addi $t1,$zero, 2
addi $t4, $zero,2 
div $t0,$t4
mfhi $t2
mflo $t3
	
# max_cycle 50
# pout_start
# 00000002
# pout_end

end:                # The End!


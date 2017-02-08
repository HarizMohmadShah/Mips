.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
addi $t0,$0,10
addi $t1,$0,2
div $t0,$t1
mfhi $1
mflo $2
	
# max_cycle 50
# pout_start
# 00000002
# pout_end

end:                # The End!


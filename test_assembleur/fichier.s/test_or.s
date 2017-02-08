.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
ori $1,$0, 0x1001
ori $2,$0, 0xFFF0	
or $28,$1,$2
	
# max_cycle 50
# pout_start
# 0000FFF1
# pout_end

end:                # The End!


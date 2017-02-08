.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
ori $28,$0, 0xFFFF
lui $1, 0x1234
ori $28,$1, 0xFFFF
	
# max_cycle 50
# pout_start
# 0000FFFF
# 1234FFFF
# pout_end

end:                # The End!


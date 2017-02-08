.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
ori $1,$0, 0x0001
nor $28,$1,$1
ori $3,$0, 0xFF0A
ori $4,$0, 0x0F0B
nor $28,$3,$4
	
# max_cycle 50
# pout_start
# FFFFFFFE
# FFFF00F4
# pout_end

end:                # The End!


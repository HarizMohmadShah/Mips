.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
ori $1,$0, 0x0001
add $28,$1,$1
ori $3,$0, 0x00A1
ori $4,$0, 0x0BF1
add $28,$3,$4
	
# max_cycle 50
# pout_start
# 00000002	
# 00000C92	
# pout_end

end:                # The End!

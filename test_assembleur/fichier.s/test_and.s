.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
ori $1,$0, 0x0001
and $28,$1,$1
ori $3,$0, 0xFF0A
ori $4,$0, 0x0F0B
and $28,$3,$4
	
# max_cycle 50
# pout_start
# 00000001
# 00000F0A	
# pout_end

end:                # The End!


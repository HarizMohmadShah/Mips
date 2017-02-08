.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
lb $t0, X
	
# max_cycle 50
# pout_start
# 0000000A
# pout_end

end:                # The End!

.data
X: .byte 0xAB
	
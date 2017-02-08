.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
j label2
label:
	ori $1,$0, 0x0001
label2:
	ori $28,$0, 0x0002
	
# max_cycle 50
# pout_start
# 00000002	
# pout_end

end:                # The End!


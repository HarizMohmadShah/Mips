.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
ori $t1,$0, 0x1001
ori $t2,$0, 0xFFF0	
xor $28,$t1,$t2
	
# max_cycle 50
# pout_start
# 0000EFF1
# pout_end

end:                # The End!


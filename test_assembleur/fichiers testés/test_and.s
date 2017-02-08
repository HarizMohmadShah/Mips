.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
ori $t0,$zero, 0x0001
jr $t0
and $t1,$t0,$t0

	
# max_cycle 50
# pout_start
# 00000001
# 00000F0A	
# pout_end

end:                # The End!


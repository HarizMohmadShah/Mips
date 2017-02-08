.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
ori $t0,$zero, 0x0001
ori $t1,$zero, 0x0002
ori $t2,$zero, 0x0001
bne $t0,$t2, test2	
bne $t0,$t1, test
test2:	
    ori $t2,$zero, 0x000A		
test:
    ori $t2,$zero, 0x0004
	
# max_cycle 50
# pout_start
# 00000001
# pout_end

end:                # The End!


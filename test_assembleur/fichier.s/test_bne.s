.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
ori $1,$0, 0x0001
ori $2,$0, 0x0002
ori $3,$0, 0x0001
bne $1,$3, test2	
bne $1,$2, test
test2:	
    ori $28,$0, 0x000A		
test:
    ori $28,$0, 0x0001
	
# max_cycle 50
# pout_start
# 00000001
# pout_end

end:                # The End!


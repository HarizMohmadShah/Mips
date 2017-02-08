.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
.org 0
progprincipal:	
ori $1,$0, 0x002C	
jr $1

test:
addiu $28,$0,1
	
.org 0x002C
j test
		
# max_cycle 50
# pout_start
# 00000001	
# pout_end

end:                # The End!


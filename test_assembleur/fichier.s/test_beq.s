.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
ori $1,$0, 0x0001
ori $2,$0, 0x0001
beq $1,$2, test
ori $28,$0, 0x0000	
test:
    ori $28,$0, 0x0001

# max_cycle 50
# pout_start
# 00000001
# pout_end

end:                # The End!


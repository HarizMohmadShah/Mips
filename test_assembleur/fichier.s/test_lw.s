.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
addi $1,$0, 0x0003
sw $1, 0xFF($0)
lw $28, 0xFF($0)
	
# max_cycle 50
# pout_start
# 00000003	
# pout_end

end:                # The End!


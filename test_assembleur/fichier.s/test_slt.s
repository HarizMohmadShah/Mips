.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
addi $t1,$0, 0x0003
addi $t2,$0, 0x0009	
slt $28,$t1,$t2
slt $28,$t2,$t1
slt $28,$t1,$0	
addi $t1,$0, -5
slt $28,$t1,$0	
	
# max_cycle 50
# pout_start
# 00000001
# 00000000
# 00000000
# 00000001	
# pout_end

end:                # The End!


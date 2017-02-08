.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
ori $t0,$zero, 0x0001
addi $t1,$t0, 0xFFD8
ori $t2,$zero, 0x00A1
addi $t3,$t2, 0xA000
	
# max_cycle 50
# pout_start
# 00001235
# 0000A0A1	
# pout_end

end:                # The End!

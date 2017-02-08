.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
ori $1,$0, 0x0001
addi $28,$1, 0x1234
ori $3,$0, 0x00A1
addi $28,$3, 0xA000
	
# max_cycle 50
# pout_start
# 00001235
# 0000A0A1	
# pout_end

end:                # The End!

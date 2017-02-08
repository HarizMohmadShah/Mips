.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
addi $1,$0,-1
blez $1, label2	
label:
	ori $28,$0, 0x0005
label2:
	ori $28,$0, 0x0004
	blez $0, label
	

# max_cycle 50
# pout_start
# 00000004
# 00000005	
# pout_end

end:                # The End!


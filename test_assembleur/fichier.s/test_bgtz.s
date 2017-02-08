.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
addi $1,$0,-1
ori $2,$0, 1
bgtz $0, label	
bgtz $1, label2
bgtz $2, label2	
label:
	ori $3,$0, 0x0001
label2:
	ori $28,$0, 0x0008

# max_cycle 50
# pout_start
# 00000008
# pout_end

end:                # The End!


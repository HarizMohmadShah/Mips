.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
addi $t0,$zero, -1
ori $t1,$zero, 1
bgtz $t0, label
bgtz $t2, label2
bgtz $t1, label2	
label:
	ori $t2,$zero, 0x0001
label2:
	ori $t3,$zero, 0x0008

# max_cycle 50
# pout_start
# 00000008
# pout_end

end:                # The End!


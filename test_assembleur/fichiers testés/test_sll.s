.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree	
addi $t0,$0,16
sll $t0,$t0,3
addi $t1,$0,0xA
sll $t1,$t1,1
addi $t2,$0,-5
sll $t2,$t2,1

end:                # The End!


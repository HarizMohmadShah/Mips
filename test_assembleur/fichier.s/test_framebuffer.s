.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
addi $t0,$0, 255
lw $t1,Z
lw $t2,Y	
while:
beq $t1,$t2,end
sw $t0,0($t1)
addi $t1,$t1,1
j while
	
end:
        # The End!
.data
Z: .word 0xFFFF0600
Y: .word 0xFFFFFFFF

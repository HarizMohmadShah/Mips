.set noreorder
.global _start      # Exportation du point d'entree (symbole global)

.text
_start:             # Point d'entree
addi $a0,$0, 255
addi $a3,$0,2
addi $t0,$0,1
lw $a1,Z
lw $a2,Y	
while:
beq $a1,$a2,end
jal ecriture_pt
	
addi $a1,$a1,1
	
j while

ecriture_pt: 		#Ecrit le point dans le framebuffer
sw $a0,0($a1)
j plusclair
retour:	
jr $ra

plusclair:		#Change la couleur
sub $a0,$a0,$t0
blez $a0, reinit	#Si couleur <= 0 alors couleur := 255
j retour

reinit:
addi $a0,$0,255		# couleur := 255
j retour	
	
end:
        # The End!
.data
Z: .word 0xFFFF0600
Y: .word 0xFFFFFFFF
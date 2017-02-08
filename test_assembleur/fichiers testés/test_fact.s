.set noreorder
.global _start
	
.text
_start:
ADDI $v0, $0, 1     # Met la valeur 1 dans le registre $v0
ADDI $t0, $0, 1     # Met la valeur 1 dans le registre $t0
ADDI $t1, $0, 6     # Met la valeur 6 dans le registre $t1	
JAL fact            # Appel a la procedure "fact" (met PC a la bonne adresse)
NOP                 # branch delay slot
beq $t0,$t1,end     # On revient ici apres le "fact"
NOP                 

fact:
addi $t0,$t0,1
mult $t0,$v0
mflo $v0
JR $31               # fin de la procedure "fact", retour a l'appelant
NOP	

end:                # The End!

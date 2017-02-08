.set noreorder
.global _start      # Exportation du point d'entree (symbole global)
.text
_start:             # Point d'entree
addi $t1,$0,255
lw $t0,x0
sw $t1,0($t0)
lw $t0,x1
sw $t1,0($t0)
lw $t0,x2
sw $t1,0($t0)
lw $t0,x3
sw $t1,0($t0)
lw $t0,x4
sw $t1,0($t0)
lw $t0,x5
sw $t1,0($t0)
lw $t0,x6
sw $t1,0($t0)
lw $t0,x7
sw $t1,0($t0)
lw $t0,x8
sw $t1,0($t0)
lw $t0,x9
sw $t1,0($t0)
end:
.data
x0: .word 4294903296
x1: .word 4294903616
x2: .word 4294903936
x3: .word 4294904256
x4: .word 4294904576
x5: .word 4294904896
x6: .word 4294905216
x7: .word 4294905536
x8: .word 4294905856
x9: .word 4294906176


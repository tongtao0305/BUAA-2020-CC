.data
nextLine: .asciiz "\n"
.text
j main
main:
move $fp,$sp
addiu $sp,$sp,-36
li $s0,1
sll $s0,$s0,1
sll $s0,$s0,2
sll $s0,$s0,3
sll $s0,$s0,4
sll $s0,$s0,5
sll $s0,$s0,6
move $a0,$s0
li $v0,1
syscall
la $a0,nextLine
li $v0,4
syscall
li $v0,10
syscall
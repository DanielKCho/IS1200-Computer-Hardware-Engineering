  # labwork.S
  # Written 2015 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

.globl hexasc
.globl time2string
.globl enable_interrupt

.text

hexasc:
andi $t0,$a0, 0xf # Masking 4 bits, ignoring all the rest

addi $t1, $0, 9

ble $a0, $t1, letters
addi $v0, $a0, 55

beq $v0, $v0, end

letters: 
addi $v0, $a0, 48

end:
jr $ra
nop

delay:
PUSH $s0
PUSH $s1
PUSH $s2
PUSH $s3
PUSH $s4

move $s0, $a0


li $s2, 15000		#Adding 4711 in order for us to compare in forloop
addi $s4, $0, 1		

while:

addi $s1, $0, 0		#i, counting variable
ble $s0, $0, whilend 	#if ms =< 0, jump to end
nop
sub $s0, $s0, $s4 	#ms = ms - 1

forloop:

ble $s2, $s1, foo
nop
add $s1, $s1, $s4	#i = i+1
j forloop
nop 

foo:
j while
nop

whilend:
POP $s4
POP $s3
POP $s2
POP $s1
POP $s0

jr $ra
nop

time2string: 
PUSH $ra
PUSH $s0
PUSH $s1

move $s1, $a1	#Min, min, sek, sek in på s1
move $s0, $a0	# memory address of string

#Masking up to 16 bits
andi $s1, $s1, 0xffff


#adding null
addi $t0, $0, 0x00
sb $t0, 5($s0)


#LS seconds
move $a0, $s1
andi $a0, $a0, 0xf


jal hexasc
nop


sb $v0, 4($s0)

#MS seconds
srl $a0, $s1, 4
andi $a0, $a0, 0xf

jal hexasc
nop

sb $v0, 3($s0)

#COLON
addi $t0, $0, 0x3A
sb $t0, 2($s0)

#Shifting to minutes
srl $a0, $s1, 8
andi $a0, $a0, 0xf


jal hexasc
nop

sb $v0, 1($s0)

#MS minutes
srl $a0, $s1, 12


jal hexasc
nop

sb $v0, 0($s0)

POP $s1
POP $s0
POP $ra

jr $ra

enable_interrupt:

ei
jr $ra
nop

  


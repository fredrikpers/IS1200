  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
	.globl delay,time2string,hexasc,enable_interrupts
	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	
	# wait a little
	li	$a0,1000
	jal	delay
	nop
	# call tick
	
	la	$a0,mytime
	jal	tick
	nop
	
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	
	# go back and do it all again
	j	main
	nop
	
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
    tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

enable_interrupts:
    ei
    jr $ra
    nop

hexasc:
	li	$t5, 0x30	#0x30 är 0 på assci
	li 	$t7, 10		#variabel som sätts till 10.
	bge	$a3,$t7,hexa10	#om inmatat värde är <= 10 gå till hexa10
		
hexa09:				#om inmatade värdet är 0-9 så körs denna.	
	add	$t5,$t5,$a3	#adderar inmatade värdet med 0x30
	move	$v0,$t5		#tilldelar function return rätt ascci värde
	jr 	$ra		#return
	

hexa10:				#om inmatadet värdet är 10 eller större körs denna.		
	add	$t5,$t5,7	#adderar 7 för att komma rätt
	add	$t5,$a3,$t5	#adderar ihop inmatat värde med grund ascii
	move	$v0,$t5		#tilldelar function return rätt ascci värde			
	jr	$ra		#return

	

delay:

	li	$t7,20500	# konstanten
delaywhile:			# While börjar
	sub	$a0,$a0,1	# ms = ms-1
	li	$t8,0		# i=0;			
forloop:		
	addi	$t8,$t8,1	# i++	
	blt	$t8,$t7,forloop	# Om t8 är mindre än t7 gå till forloop
	bgtz	$a0,delaywhile 	# Om a1 är större än 0 gå till...
		

	jr $ra
	nop	


time2string:

    PUSH $s0
    PUSH $s1

    move    $s1,$a1
    move    $s0,$a0

	srl	$t3,$a1,12	#Får fram tal "nummer 4"
	and	$t3,$t3,0x000f			
	srl	$t2,$a1,8	#Får fram tal "nummer 3"
	and	$t2,$t2,0x000f
	srl	$t1,$a1,4	#Får fram tal "nummer 2"
	and	$t1,$t1,0x000f
	
	
	move	$a3,$t3		#Skickar till hexasc funktion
	PUSH	$ra		#PUSH
	jal	hexasc		#Kallar på funktion
	nop
	POP	$ra		#POP
	sb	$v0, 0($a0)	#Lagrar i $v0
	
	move	$a3,$t2		#Skickar till hexasc funktion
	PUSH	$ra		#PUSH
	jal	hexasc		#Kallar på funktion
	nop
	POP	$ra		#POP
	sb	$v0, 1($a0)	#Lagrar i $v0
	
	addi	$t4, $0, 0x3A	#Lägger till : efter timmar
	sb	$t4, 2($a0)
	
	move	$a3,$t1		#Skickar till hexasc funktion
	PUSH	$ra		#PUSH
	jal	hexasc		#Kallar på funktion
	nop
	POP	$ra		#POP
	sb	$v0, 3($a0)	#Lagrar i $v0
	
	add	$t9, $0, $a1
	and	$t9,$t9,0x000f
	move	$a3,$t9		#Skickar till hexasc funktion
	PUSH	$ra		#PUSH
	jal	hexasc		#Kallar på funktion
	nop
	POP	$ra		#POP
	sb	$v0, 4($a0)	#Lagrar i $v0
	
	addi	$t4, $0, 0x00	#Lägger till "tomrum" i slutet
	sb	$t4, 5($a0)
	
	jr	$ra		#Skickar tillbaka allt
	nop


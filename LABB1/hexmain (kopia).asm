  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,12	# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
		
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

hexasc:
	li	$t5, 0x30	#0x30 är 0 på assci
	bge	$a0,10,hexa10	#om inmatat värde är <= 10 gå till hexa10
		
hexa09:				#om inmatade värdet är 0-9 så körs denna.	
	add	$t5,$t5,$a0	#adderar inmatade värdet med 0x30
	move	$v0,$t5		#tilldelar function return rätt ascci värde
	jr 	$ra		#return
	

hexa10:				#om inmatadet värdet är 10 eller större körs denna.		
	add	$t5,$t5,7	#adderar 7 för att komma rätt
	add	$t5,$a0,$t5	#adderar ihop inmatat värde med grund ascii
	move	$v0,$t5		#tilldelar function return rätt ascci värde			
	jr	$ra		#return
	
	
		

  # You can write your own code for hexasc here
  #


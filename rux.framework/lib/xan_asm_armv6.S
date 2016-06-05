	.global     RuntimeInvoke
	.type RuntimeInvoke, %function
	.code 32
RuntimeInvoke:
	stmdb sp!,{r4-r11,lr}
	sub sp,sp,#4
	mov r5,#0		
	mov r6,r1
	mov r7,r2		
	mov r4,#4
	mul r8,r3,r4	
	mov r11,#0
	cmp r8,#20
	blt 11f
	sub r11,r8,#16
	sub sp,sp,r11
	11:
	mov r4,r0
	1:
	cmp r5,r8
	beq 7f
	cmp r5,#0
	beq 3f
	cmp r5,#4
	beq 4f
	cmp r5,#8
	beq 5f
	cmp r5,#12
	beq 6f
	mov r5,r8
	sub r5,#4
	2:
	ldr r10,[r7,r5]				
	push { r10 }
	sub r5,#4
	cmp r5,#12
	beq 7f
	b 2b
	3:		
	ldr r0,[r7,r5]
	add r5,#4		
	b 1b
	4:		
	ldr r1,[r7,r5]	
	add r5,#4
	b 1b
	5:
	ldr r2,[r7,r5]		
	add r5,#4
	b 1b
	6:
	ldr r3,[r7,r5]
	add r5,#4
	b 1b
	7:	
	mov r5,r8		
	blx r4				
	cmp r5,#20
	blt 9f
	8:		
	pop { r10 }
	sub r5,#4
	9:		
	cmp r5,#16
	bgt 8b	
	cmp r6,#1
	beq 10f
	mov r1,#0
	10:
	add sp,sp,r11
	add sp,sp,#4
	ldmia sp!,{r4-r11,pc}
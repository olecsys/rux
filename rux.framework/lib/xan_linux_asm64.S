global RuntimeInvoke

RuntimeInvoke:	
	push r11
	push rbx
	push r12
	push r13
	mov r11, rdi	;void* pAddress 16
	mov rbx, rdx	;uint64* pParams 24
	mov r12, rcx	;DWORD ulParamsCount 28
	mov r13, 0    ;uint64 is_double 20
	push rdi
	push rdx
	push rcx
	push rsi
	push r14
	push r10
	push r15
	mov rax, 0
	mov r15, 0
pushing0:
	mov r14,rax
	add r14,r15
	add r14,r13
	cmp r14,r12	
	je func0
	cmp dword[rbx],1
	je push_float_arg
	cmp dword[rbx],2
	je push_float_arg
	cmp rax, 0
	je push_1_arg
	cmp rax, 1
	je push_2_arg
	cmp rax, 2
	je push_3_arg
	cmp rax, 3
	je push_4_arg
	cmp rax, 4
	je push_5_arg
	cmp rax, 5
	je push_6_arg
pushing1:
	mov r14, qword[rbx+8]	
	sub rsp,8
	mov qword[rsp],r14
	add r13,byte 1
	add rbx,16
	jmp pushing0
push_1_arg:
	mov rdi, qword[rbx+8]	
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_2_arg:
	mov rsi, qword[rbx+8]	
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_3_arg:
	mov rdx, qword[rbx+8]	
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_4_arg:
	mov rcx, qword[rbx+8]	
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_5_arg:
	mov r8, qword[rbx+8]	
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_6_arg:
	mov r9, qword[rbx+8]	
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_1_float_arg:
	movq xmm0, qword[rbx+8]
	add r15,byte 1
	add rbx,16
	jmp pushing0
push_2_float_arg:
	movq xmm1, qword[rbx+8]
	add r15,byte 1
	add rbx,16
	jmp pushing0
push_3_float_arg:
	movq xmm2, qword[rbx+8]
	add r15,byte 1
	add rbx,16
	jmp pushing0
push_4_float_arg:
	movq xmm3, qword[rbx+8]
	add r15,byte 1
	add rbx,16
	jmp pushing0
push_5_float_arg:
	movq xmm4, qword[rbx+8]
	add r15,byte 1
	add rbx,16
	jmp pushing0
push_6_float_arg:
	movq xmm5, qword[rbx+8]
	add r15,byte 1
	add rbx,16
	jmp pushing0
push_7_float_arg:
	movq xmm6, qword[rbx+8]
	add r15,byte 1
	add rbx,16
	jmp pushing0
push_8_float_arg:
	movq xmm7, qword[rbx+8]
	add r15,byte 1
	add rbx,16
	jmp pushing0
push_float_arg:
	cmp r15, 0
	je push_1_float_arg
	cmp r15, 1
	je push_2_float_arg
	cmp r15, 2
	je push_3_float_arg
	cmp r15, 3
	je push_4_float_arg
	cmp r15, 4
	je push_5_float_arg
	cmp r15, 5
	je push_6_float_arg
	cmp r15, 6
	je push_7_float_arg
	cmp r15, 7
	je push_8_float_arg
	jmp pushing1
func0:
	test r13,1
	jz even
	add r13,1
	sub rsp,8
even:
	mov r14,r13
	cmp r14,0
	je call_func
	sub r14,byte 1
	mov r15,0
func1:
	cmp r14,r15
	je call_func
	cmp r14,r15
	jl call_func
	mov r12,qword[rsp+r15*8]
	mov r10,qword[rsp+r14*8]
	mov qword[rsp+r15*8],r10
	mov qword[rsp+r14*8],r12
	sub r14,byte 1
	add r15,byte 1
	jmp func1
call_func:
	mov rax, 0
	call r11	
	mov r12,0
	jmp funcend0
funcend1:
	add rsp,8
	add r12,byte 1
funcend0:	
	cmp r12, r13
	jl funcend1	
	pop r15
	pop r10
	pop r14
	pop rsi
	pop rcx
	pop rdx
	pop rdi
	pop r13
	pop r12
	pop rbx
	pop r11
	ret

global ObjectInvoke ;TODO

ObjectInvoke:	
	push rsi	
	pop rsi	
	ret

global AsmLeftShift

AsmLeftShift:	
	push rcx
	push rsi
	push rdi
	mov rcx, rsi;	
	mov rsi, rdi;
	mov rdi, rsi;
	sub rcx, 8;
	shr rcx, 3;
	xor rax, rax;
	cld;
loop0:
	lodsq;
	rcl rax, 1;
	stosq;
	loop loop0;
	adc rcx, 0;
	pop rdi
	pop rsi
	pop rcx
	ret

global AsmRightShift

AsmRightShift:	
	push rcx
	push rsi
	push rdi
	mov rcx, rsi;	
	mov rsi, rdi;
	add rsi, rcx;
	sub rsi, 8;
	mov rdi, rsi;
	shr rcx, 3;
	xor rax, rax;
	std;
loop1:
	lodsq;
	rcr rax, 1;
	stosq;
	loop loop1;
	adc rcx, 0;
	cld;
	pop rdi
	pop rsi
	pop rcx
	ret

global AsmMinus

AsmMinus:	
	push rcx
	push rdi
	push rsi
	mov rcx, rdx;	
	clc;
__cycle0: 
	lodsq;
	sbb     [rdi], rax;
	lea     rdi, [rdi+8];
	loop    __cycle0;	
	pop rsi
	pop rdi
	pop rcx
	ret

global AsmPlus

AsmPlus:	
	push rcx
	push rdi
	push rsi
	mov rcx, rdx;
	clc;
__cycle1:
	lodsq;
	adc     rax, [rdi];
	stosq;
	loop    __cycle1;
	pop rsi
	pop rdi
	pop rcx
	ret

global AsmCompare

AsmCompare:	
	push rcx
	push rdi
	push rsi
	push rbx
	mov rbx, rcx
	mov rcx, rdi
	mov rdi , rsi;
	mov rsi , rcx;
	mov rcx, rdx;	
__cycle2:
	mov     rax, [rsi+rcx*8-8];
	cmp     rax, [rdi+rcx*8-8];
	loopz  __cycle2;		
	jb __less;
	ja __more;		
	jmp __exit;
__less:
	sub qword[rbx], 1;
	jmp __exit;
__more:
	add qword[rbx], 1;
	jmp __exit;
__exit:	
	pop rbx
	pop rsi
	pop rdi
	pop rcx
	ret

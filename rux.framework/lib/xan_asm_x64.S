section .data
section .text
global RuntimeInvoke

RuntimeInvoke:	
	mov qword[rsp+8] , rbx  
	mov qword[rsp+16] , rsi  
	mov qword[rsp+24] , r14  
	mov qword[rsp+32] , r13		
	mov qword[rsp+40] , rdi		
	mov qword[rsp+48] , rdx			
	sub rsp, 56	
	mov r14 , r9	
	cmp r14 , 4
	jl continue0
	sub r14 , 4	
continue0:
	test r14,1
	jz even
	add r14 , 1
even:
	imul r14 , 8
	sub rsp ,r14  	
	mov r13 , rcx
	mov rbx, r8				;rux::uint64* pParams 24
	mov rsi, r9				;DWORD ulParamsCount 28
	mov rax, 0
pushing0:
	cmp rax, rsi
	je func0
	cmp rax, 0
	je push_1_arg
	cmp rax, 1
	je push_2_arg
	cmp rax, 2
	je push_3_arg
	cmp rax, 3
	je push_4_arg	
	mov rdi, qword[rbx+8]
	mov qword[rsp+rax*8], rdi
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_1_arg:
	cmp dword[rbx],1
	je push_1_float_arg
	cmp dword[rbx],2
	je push_1_float_arg
	mov rcx, qword[rbx+8]
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_1_float_arg:
	movq xmm0, qword[rbx+8]
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_2_arg:
	cmp dword[rbx],1
	je push_2_float_arg
	cmp dword[rbx],2
	je push_2_float_arg
	mov rdx, qword[rbx+8]	
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_2_float_arg:
	movq xmm1, qword[rbx+8]
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_3_arg:
	cmp dword[rbx],1
	je push_3_float_arg
	cmp dword[rbx],2
	je push_3_float_arg
	mov r8, qword[rbx+8]	
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_3_float_arg:
	movq xmm2, qword[rbx+8]
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_4_arg:
	cmp dword[rbx],1
	je push_4_float_arg
	cmp dword[rbx],2
	je push_4_float_arg
	mov r9, qword[rbx+8]	
	add rax,byte 1
	add rbx,16
	jmp pushing0
push_4_float_arg:
	movq xmm3, qword[rbx+8]
	add rax,byte 1
	add rbx,16
	jmp pushing0
func0:
	mov rax, 0
	call r13	
popping3:
	add rsp , r14		
	add rsp, 56
	mov rdx, qword[rsp+48]		;rux::uint64 is_double 20	
	cmp rdx, 0
	je funcend0
	cmp rdx, 3
	je funcend0
	movq rax, xmm0
funcend0:			
	mov rbx , qword[rsp+8]
	mov rsi , qword[rsp+16]
	mov r14 , qword[rsp+24]
	mov r13 , qword[rsp+32]
	mov rdi , qword[rsp+40]
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
	mov rsi, rcx;
	mov rcx, rdx;	
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
	mov rsi, rcx;
	mov rcx, rdx;	
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
	mov rdi , rcx;
	mov rcx, r8;	
	mov rsi , rdx;
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
	mov rdi , rcx;
	mov rcx, r8;
	mov rsi , rdx;
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
	mov rbx, r9
	mov rsi , rcx;
	mov rcx, r8;	
	mov rdi , rdx;
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

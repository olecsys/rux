global RuntimeInvoke

RuntimeInvoke:	
	push ecx
	push edi
	push esi
	push ebx
	mov esi, [esp+20]				;void* pAddress 16
	mov ecx, dword[esp+24]				;DWORD is_double 20
	mov ebx, [esp+28]				;DWORD* pParams 24
	mov edi, dword[esp+32]				;DWORD ulParamsCount 28	
	push ecx
	push pushing0
	push func0
	mov eax, edi
	cmp eax, 0
	je func0
	sub eax,byte 1
	imul eax,12
pushing0:		
	mov edi, dword[ebx+eax]
	cmp edi, 2
	jl bits32
	mov edi, ebx
	add edi, eax
	add edi,8
	push dword[edi]
	sub edi,4
	push dword[edi]
	cmp eax, 0
	je func0
	sub eax,byte 12
	jmp pushing0
bits32:
	mov edi, ebx
	add edi, eax
	add edi,4
	push dword[edi]
	cmp eax, 0
	je func0
	sub eax,byte 12
	jmp pushing0
func0:
	call esi
popping1:
	pop esi
	cmp esi, func0
	je popping2
	jmp popping1
popping2:
	pop edi
	cmp edi, pushing0
	je popping3
	jmp popping1
popping3:
	pop ecx
	pop ebx
	pop esi
	pop edi
	cmp ecx,0
	je funcend0
	cmp ecx,1
	je float_label
	cmp ecx,3
	je funcend1
	sub esp, 8
	fstp qword[esp]
	mov eax, dword[esp]
	mov edx, dword[esp+4]
	add esp, 8
	jmp funcend1
float_label:
	sub esp, 4
	fstp dword[esp]
	mov eax, dword[esp]
	add esp, 4
funcend0:
	mov edx,0	
funcend1:
	pop ecx
	ret

global AsmLeftShift

AsmLeftShift:	
	push ecx
	push esi
	push edi
	mov ecx, [esp+20];
	mov esi, [esp+16];
	mov edi, esi;
	sub ecx, 4;
	shr ecx, 2;
	xor eax, eax;
	cld;
loop0:
	lodsd;
	rcl eax, 1;
	stosd;
	loop loop0;
	adc ecx, 0;
	pop edi
	pop esi
	pop ecx
	ret

global AsmRightShift

AsmRightShift:	
	push ecx
	push esi
	push edi
	mov ecx, [esp+20];
	mov esi, [esp+16];
	add esi, ecx;
	sub esi, 4;
	mov edi, esi;
	shr ecx, 2;
	xor eax, eax;
	std;
loop1:
	lodsd;
	rcr eax, 1;
	stosd;
	loop loop1;
	adc ecx, 0;
	cld;
	pop edi
	pop esi
	pop ecx
	ret

global AsmMinus

AsmMinus:	
	push ecx
	push edi
	push esi
	mov ecx, [esp+24];
	mov edi , [esp+16];
	mov esi , [esp+20];
	clc;
__cycle0: 
	lodsd;
	sbb     [edi], eax;
	lea     edi, [edi+4];
	loop    __cycle0;	
	pop esi
	pop edi
	pop ecx
	ret

global AsmPlus

AsmPlus:	
	push ecx
	push edi
	push esi
	mov ecx, [esp+24];
	mov edi , [esp+16];
	mov esi , [esp+20];
	clc;
__cycle1:
	lodsd;
	adc     eax, [edi];
	stosd;
	loop    __cycle1;
	pop esi
	pop edi
	pop ecx
	ret

global AsmCompare

AsmCompare:	
	push ecx
	push edi
	push esi
	push ebx
	mov ebx, [esp+32];
	mov ecx, [esp+28];
	mov esi , [esp+20];
	mov edi , [esp+24];
__cycle2:
	mov     eax, [esi+ecx*4-4];
	cmp     eax, [edi+ecx*4-4];
	loopz  __cycle2;		
	jb __less;
	ja __more;		
	jmp __exit;
__less:
	sub dword[ebx], 1;
	jmp __exit;
__more:
	add dword[ebx], 1;
	jmp __exit;
__exit:	
	pop ebx
	pop esi
	pop edi
	pop ecx
	ret

global ObjectInvoke

ObjectInvoke:	
	push ecx
	push edi
	push esi
	push ebx
	mov esi, [esp+20]				;void* pAddress 16
	mov ecx, dword[esp+24]			;DWORD is_double 20
	push ecx
	mov ecx, [esp+32]				;DWORD* pParams 24
	mov ebx, [esp+36]				;DWORD* pParams 24
	mov edi, dword[esp+40]			;DWORD ulParamsCount 28
	push pushing
	push func2
	mov eax, edi
	cmp eax, 0
	je func2
	sub eax,byte 1
pushing:
	push dword[ebx+eax*4]	
	cmp eax, 0
	je func2
	sub eax,byte 1
	jmp pushing
func2:
	call esi
popping7:
	pop esi
	cmp esi, func2
	je popping8
	jmp popping7
popping8:
	pop edi
	cmp edi, pushing
	je popping9
	jmp popping7
popping9:
	pop ecx
	pop ebx
	pop esi
	pop edi
	cmp ecx,0
	je funcend2
	cmp ecx,2
	je float_label1
	sub esp, 8
	fstp qword[esp]
	mov eax, dword[esp]
	mov edx, dword[esp+4]
	add esp, 8
	jmp funcend3
float_label1:
	sub esp, 4
	fstp dword[esp]
	mov eax, dword[esp]
	add esp, 4
funcend2:
	mov edx,0	
funcend3:
	pop ecx
	ret

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_asm.h>
#if defined( __LINUX__ ) && defined( __ARM_ARCH_6__ )
/*__attribute__( (naked) )
int64 __cdecl CdeclInvoke( void* fn_address_ptr , uint32 is_double ,  uint32* params_ptr , uint32 params_count )
{	
	asm volatile( ".code 32\n\t"
		"push {r4-r8,fp,ip,lr}\n\t"		
		"mov r5,#0\n\t"		
		"mov r6,r1\n\t"
		"mov r7,r2\n\t"		
		"mov r4,#4\n\t"
		"mul r8,r3,r4\n\t"		
		"mov r4,r0\n\t"
		"1:\n\t"
		"cmp r5,r8\n\t"
		"beq 7f\n\t"
		"cmp r5,#0\n\t"
		"beq 3f\n\t"
		"cmp r5,#4\n\t"
		"beq 4f\n\t"
		"cmp r5,#8\n\t"
		"beq 5f\n\t"
		"cmp r5,#12\n\t"
		"beq 6f\n\t"
		"mov r5,r8\n\t"
		"sub r5,#4\n\t"
		"2:\n\t"
		"ldr ip,[r7,r5]\n\t"				
		"push { ip }\n\t"
		"sub r5,#4\n\t"
		"cmp r5,#12\n\t"
		"beq 7f\n\t"
		"b 2b\n\t"
		"3:\n\t"		
		"ldr r0,[r7,r5]\n\t"
		"add r5,#4\n\t"		
		"b 1b\n\t"
		"4:\n\t"		
		"ldr r1,[r7,r5]\n\t"	
		"add r5,#4\n\t"
		"b 1b\n\t"
		"5:\n\t"
		"ldr r2,[r7,r5]\n\t"		
		"add r5,#4\n\t"
		"b 1b\n\t"
		"6:\n\t"
		"ldr r3,[r7,r5]\n\t"
		"add r5,#4\n\t"
		"b 1b\n\t"
		"7:\n\t"	
		"mov r5,r8\n\t"		
		"blx r4\n\t"				
		"cmp r5,#20\n\t"
		"blt 9f\n\t"
		"8:\n\t"		
		"pop { ip }\n\t"
		"sub r5,#4\n\t"
		"9:\n\t"		
		"cmp r5,#16\n\t"
		"bgt 8b\n\t"	
		"cmp r6,#1\n\t"
		"beq 10f\n\t"
		"mov r1,#0\n\t"
		"10:\n\t"				
		"pop {r4-r8,fp,ip,pc}" );
};*/
int64 __cdecl ObjectInvoke( void* fn_address_ptr , uint32 is_double , void* object_ptr , uint32* params_ptr , uint32 params_count )
{
	return 0;
};
void __cdecl AsmLeftShift( void* buffer_ptr , uint32 length )
{
};
void __cdecl AsmRightShift( void* buffer_ptr , uint32 length )
{
};
void __cdecl AsmMinus( void* buffer_ptr0 , void* buffer_ptr1 , uint32 length )
{
};
void __cdecl AsmPlus( void* buffer_ptr0 , void* buffer_ptr1 , uint32 length )
{
};
void __cdecl AsmCompare( void* buffer_ptr0 , void* buffer_ptr1 , uint32 length , int32* result )
{
};
#endif
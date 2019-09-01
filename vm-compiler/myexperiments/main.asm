;https://www.tutorialspoint.com/assembly_programming/assembly_system_calls.htm

section .text
global _start

_start:

	push 1
	pop ebx;


	mov ecx,0
	mov edx,0
	;mov eex,0
	;mov efx,0

	mov eax,1 ;system call number (sys_exit);
	;mov ebx,0 ;return code 0 (success)
	int 0x80 ; call kernel

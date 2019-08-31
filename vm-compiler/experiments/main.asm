section .text
global _start

_start:

	push 1
	pop ebx;

	mov eax,1 ;system call number (sys_exit);
	;mov ebx,0 ;return code 0 (success)
	int 0x80 ; call kernel

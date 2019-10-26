section	.text
   global _start     ;must be declared for linker (ld)
	
_start:	            ;tells linker entry point
   ;mov	edx,len     ;message length
   ;mov	ecx,msg     ;message to write
   ;mov	ebx,1       ;file descriptor (stdout)
   ;mov	eax,4       ;system call number (sys_write)
   ;int	0x80        ;call kernel

   ;try to create a floating point value, in register xmm0
   ;and try to extract the significant or exponent
   ;to see if we can do that

	;movss xmm0, 1.0
	mov eax,num
	and eax, 07FFFFFh
	; eax should contain now the 22 bit mantissa
	; print out eax
	; push eax on stack,
	; call printi 

	
   mov	eax,1       ;system call number (sys_exit)
   int	0x80        ;call kernel

section	.data
num REAL4 -1.5
msg db 'Hello, world!', 0xa  ;string to be printed
len equ $ - msg     ;length of the string

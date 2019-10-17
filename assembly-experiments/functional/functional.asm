section	.text
   global _start     ;must be declared for linker (ld)
	
_start:	            ;tells linker entry point

	; you can push labels on stack in assembly

   push subr ; pushing label on stack

   pop eax ; 
   call eax;

   mov eax,1; sys_exit
   int 0x80;


subr:

   mov	edx,len     ;message length
   mov	ecx,msg     ;message to write
   mov	ebx,1       ;file descriptor (stdout)
   mov	eax,4       ;system call number (sys_write)
   int	0x80        ;call kernel
	
   ret;

section	.data
msg db 'Hello, world!', 0xa  ;string to be printed
len equ $ - msg     ;length of the string

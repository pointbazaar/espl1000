section .text		;must be declared for linker (ld)
global _start		;                        
_start:   		;tell linker entry point 
	mymem 10 0	;                        
	mov eax [mymem]	;                        
	mov eax,1 	;system call number (sys_exit)
	mov ebx,0 	;return code 0 indicates success
	int 0x80  	;call kernel             

section .text		;must be declared for linker (ld)
	global _start	;                        
_start:   		;tell linker entry point 
	jmp main  	;                        
main:     		;                        
	mov eax,0 	;                        
	push eax  	;                        
	mov eax,1 	;                        
	push eax  	;                        
	pop ebx   	;                        
	pop eax   	;                        
	add eax,ebx	;                        
	push eax  	;                        
	mov eax,4 	;sys_writ                
	mov ebx,1 	;std_out                 
	pop ecx   	;                        
	add ecx,0x30	;                        
	push ecx  	;                        
	mov ecx,esp	;                        
	mov edx,1 	;                        
	int 0x80  	; call kernel            
	pop edx   	;                        
	mov edx,0 	;                        
	push edx  	;                        
	mov eax,0 	;                        
	push eax  	;                        
	mov eax,1 	;system call number (sys_exit)
	mov ebx,0 	;return code 0 indicates success
	int 0x80  	;call kernel             
section .data		;                        
	achar db '0'	;                        
section .bss		;                        
	buffer:	resb	64	;                        

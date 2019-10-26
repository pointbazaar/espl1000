section	.text
   global _start     ;must be declared for linker (ld)
	
_start:	            ;tells linker entry point

	mov eax,34		; we want to print some integer
   	push eax

	call Builtin_puti
	pop eax
	
   mov	eax,1       ;system call number (sys_exit)
   int	0x80        ;call kernel


Builtin_puti:


	mov esi,[esp]	; get our argument

	; see if it is negative
	mov ebx,0
	cmp esi,ebx
	jl .isneg
	jmp .ispos

	.isneg:
		; print '-'
		
		push '-'
		mov	edx,1     ;message length
		mov	ecx,esp     ; char* , message to write
		mov	ebx,1       ;file descriptor (stdout)
		mov	eax,4       ;system call number (sys_write)
		int	0x80        ;call kernel
		
		pop eax

		;make esi positive
		mov eax,esi
		mov edx,-1
		imul edx
		;eax should be positive now
		mov esi,edx
	
	.ispos:

	; loop while esi >0
	.loop:

		mov eax,esi
		mov edx, 0 ;dividen high half=0
		mov ebx, 10;dividend low half
		div ebx
		; edx should now contain the remainder
		; eax should now contain the quotient
		mov esi,eax
		
		;push the digit, convert it to a char first
		add edx,'0'
		push edx
	
		mov	edx,1     ;message length
		mov	ecx,esp     ; char* , message to write
		mov	ebx,1       ;file descriptor (stdout)
		mov	eax,4       ;system call number (sys_write)
		int	0x80        ;call kernel
	
		pop eax
	
		mov edx,0
		cmp esi,edx
		; jump if esi>0
		jg .loop
	
	ret

section	.data
msg db 'Hello, world!', 0xa  ;string to be printed
len equ $ - msg     ;length of the string

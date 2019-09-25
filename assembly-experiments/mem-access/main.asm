section .text
	global _start

_start:
	;try some direct memory access
	;mov eax,'x'
	;push eax

	;allocate memory with mmap system call
	mov eax,192	;mmap2
	xor ebx,ebx	;addr=NULL
	mov ecx,4096	; len=4096 ;length of segment to be allocated?
	mov edx,0x7	; prot = PROT_READ | PROT_WRITE | PROT_EXEC
	mov esi,0x22	; flags=MAP_PRIVATE | MAP_ANONYMOUS
	mov edi,-1	; fd=-1
	xor ebp,ebp	; offset=0
	int 0x80	;make call
	;syscall

	;put a char in that memory location
	mov ebx,'a'
	mov [eax],ebx

	;hope that we can get the char out and push it on stack
	mov ebx,[eax]
	push ebx

         ;print the char on stack
         pop ecx ; //pop digit into ecx
         ;add ecx,0x30 ; //add ascii offset
         push ecx ;  //push it back on stack
         mov ecx,esp ;
	mov eax,4 ;sys_write
	mov ebx,1 ;stdout
         ;val length
         mov edx,1;
         int 0x80;
	
	;exit
	mov eax,1
	int 0x80



section	.text
   global _start
	
_start:	        ;entry point

;allocating memory
mov eax, 192	;syscall 
xor ebx, ebx	;addr = NULL
mov ecx, 8  	;allocation size
mov edx, 0x7	;prot = PROT_READ | PROT_WRITE | PROT_EXEC
mov esi, 0x22	;flags = MAP_PRIVATE | MAP_ANONYMOUS
mov edi, -1		;fd = -1
xor ebp, ebp	;offset = 0
int 0x80		;call mmap

;reading integer from stack
pop ecx

;moving integer to heap
mov [eax], ecx

;pushing address to stack
push eax

;preparing for write syscall
mov eax, 4
mov ebx, 0
pop ecx
mov edx, 8
int 0x80		;call write to stdout

;program exit
mov	eax, 1      ;system call number (sys_exit)
mov ebx, 0
int	0x80        ;call kernel

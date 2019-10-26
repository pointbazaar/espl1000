; this program is just me trying out 
; to work with floating point values in assembly
; https://stackoverflow.com/questions/42181811/how-to-add-two-numbers-integer-and-a-float-in-nasm

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
	;mov eax,num

	;mov dword eax, __float32__(1.0)
	;and eax, 07FFFFFh
	; eax should contain now the 22 bit mantissa

	;DEBUG: TODO: remove this
	mov eax,13

	; pick out the last digit by using %10
	; https://stackoverflow.com/questions/8021772/assembly-language-how-to-do-modulo
	
	mov ebx,10 ; divisor is 10
	div ebx  ; divide eax by ebx=10
	; edx now contains the remainder
	mov eax,edx

	add eax, 0x30	; convert digit to ascii
	push 49 ; push ascii '1'
	mov ecx,esp ;for printchar_on_stack
	call printchar_on_stack ;print the char
	pop eax

	mov	eax,1       ;system call number (sys_exit)
	int	0x80        ;call kernel


printchar_on_stack:
	; this subroutine is to print one char, which is on the stack
	; the stack should not be changed by this subroutine

	;mov ecx, esp
	
	;put a pointer to our string in ecx
	;because we are printing the char at top of stack, esp will suffice

	mov eax,4 ;syscall number: sys_write
	mov ebx,1 ;write to stdout
	mov edx,1 ;length to be printed
	int 0x80  ;call kernel		
	
	ret


section	.data
	
	msg db 'Hello, world!', 0xa  ;string to be printed
	len equ $ - msg     ;length of the string

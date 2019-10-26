; this program is just me trying out 
; to work with floating point values in assembly
; https://stackoverflow.com/questions/42181811/how-to-add-two-numbers-integer-and-a-float-in-nasm
; https://steve.hollasch.net/cgindex/coding/ieeefloat.html


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

	mov dword eax, __float32__(1.1875)
	;https://www.intel.com/content/www/us/en/programmable/documentation/eis1410764818924.html#eis1410767329165

	; to obtain the mantissa
	;and eax, 0xb 0111 1111 1111 1111 1111 1111; bitmask the mantissa
	;and eax, 0b011111111111111111111111; bitmask the mantissa
	; eax should contain now the 22 bit mantissa

	; obtain the exponent, it is 1 for negative numbers, 0 for positives
	;and eax, 0b1000 0000 0000 0000 0000 0000 0000 0000
	and eax, 0b10000000000000000000000000000000
	shr eax, 31 


	; pick out the last digit by using %10
	; https://stackoverflow.com/questions/8021772/assembly-language-how-to-do-modulo

	
		xor edx,edx
		mov ebx,10 ; divisor is 10
		div ebx  ; divide eax by ebx=10
		; edx now contains the remainder
		mov eax,edx
	
		add eax, 0x30	; convert digit to ascii 
		push eax ;push our ascii value on stack
		;push 49 ; push ascii '1'
		
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

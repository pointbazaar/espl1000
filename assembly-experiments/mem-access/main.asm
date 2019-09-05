section .text
	global _start

_start:
	;try some direct memory access
	 ;mov eax,4
         ;mov ebx,1
	mov eax,'x'
	push eax

         ;print the char on stack
         pop ecx ; //pop digit into ecx
         add ecx,0x30 ; //add ascii offset
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



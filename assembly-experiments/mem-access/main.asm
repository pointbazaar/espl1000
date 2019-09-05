section .text
	global _start

_start:
	;try some direct memory access
	;mov eax,'x'
	;push eax

	;access memory at location 0,write some char in it
	mov eax,'e';
	mov [0],eax;

	;hope that we can get the char out and push it on stack
	mov ebx,[0]
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



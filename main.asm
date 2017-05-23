%macro return 1
	mov rbx, %1
	mov rax, 0x1 ; sys_exit
	int 0x80
%endmacro

%macro printf 2
	
section .text
	global main

main:
	return 0
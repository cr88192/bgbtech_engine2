;extern printf

section .data
tststr db "asm test string\n", 0

section .text
bits 32

basm_main:
push ebp
mov ebp, esp

;getbase ecx

push tststr
;lea eax, [ecx+$tststr]
;push eax

call printf
;call _ptst
pop ecx

pop ebp
ret

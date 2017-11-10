%macro printf 2
    mov rdx, %2
    mov rcx, %1
    mov rbx, 1    ; stdout
    mov rax, 4    ; sys_write
    int 0x80
%endmacro

%macro scanf 2
    mov rdx, %2
    mov rcx, %1
    mov rbx, 0x2    ; stdin
    mov rax, 0x3    ; sys_read
    int 0x80
%endmacro

%macro return 1
    mov rbx, %1
    mov rax, 0x1    ; sys_exit
    int 0x80
%endmacro

%define victory byte 0x01
%define draw    byte 0x02
%define nothing byte 0x03

section .bss
    board resb 9        ; tabuleiro
    square resb 3       ; formato para impressão

    x resb 1            ; salvar o movimento em x
    y resb 1            ; salvar o movimento em y

    fool resb 10        ; salvar os caracteres de fim-de-linha

    player resb 1       ; salvar o jogador atual

    stats resb 1        ; armazena o status 1 - vitoria, 2 - empate, 3 - nada

    index resb 3        ; índice da linha e da coluna

section .data
    eol db 0xa                                                  ; fim-de-linha
    empty db ' '                                                ; espaço em branco

    choose_your_move db 0xa, 'Jogador $, faca sua jogada', 0xa  ; mensagem de escolha de jogada
    choose_your_move_len equ $ - choose_your_move               ; tamanho da mensagem de escolha de jogada

    victory_    db 0xa, 'Jogador $ venceu', 0xa                 ; mensagem de vitória
    victory_len equ $ - victory_                                ; tamanho da mensagem de vitória

    draw_ db 0xa, 'Empate ', 0xa                                ; mensagem de empate
    draw_len equ $ - draw_                                      ; tamanho da mensagem de empate

section .text
    global main

; ############################################################################
; # Main
; ############################################################################
main:
    printf victory_, victory_len

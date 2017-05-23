%macro printf 2
    mov rdx, %2
    mov rcx, %1
    mov rbx, 0x1    ; stdout
    mov rax, 0x4    ; sys_write
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
    call init
    call print_board

    main.game:
        call make_move                              ; faz a jogada
        call print_board                            ; imprime o tabuleiro
        call check_game_status                      ; verifica o status do jogo

        cmp byte [stats], nothing                   ; compara o status e vê se houve algo
        jne main.game.end_game                      ; se sim, o que ?

        main.game.next_round:                       ; se não
            call change_player                      ; troca o jogador atual
            jmp main.game                           ; mais um turno

        main.game.end_game:                         ; se sim
            cmp byte [stats], draw                  ; vê se foi empate
            je main.game.end_game.draw              ; e vai

            main.game.end_game.victory:             ; se não
                mov al, byte [player]               ; insere o vencedor na mensagem de vitória
                mov byte [victory_ + 8], al
                printf victory_, victory_len        ; imprime
                jmp main.game.end_game.end          ; encerra o jogo

            main.game.end_game.draw:                ; se sim
                printf draw_, draw_len              ; imprime que houve empate
                jmp main.game.end_game.end          ; encerra o jogo

            main.game.end_game.end:                 ; encerra o jogo
                return 0                            ; return 0

; ############################################################################
; # Inicia o jogo
; ############################################################################
init:
    init.board:
        mov rcx, 3 * 3                          ; inicia o contador do loop
        mov rbx, 0x0                            ; move o tabuleiro para rbx     
        init.board.for:
            push rcx                            ; 'push' o contador do loop

            mov byte [board + rbx], byte ' '    ; move ' ' para a posição atual do tabuleiro
            inc rbx                             ; vai para a próxima posição

            pop rcx                             ; 'pop' contador do loop
            loop init.board.for                 ; loop

    init.square:
        mov byte [square + 0], byte '['         ; move '[' para a primeira posição
        mov byte [square + 1], byte ' '         ; move ' ' para a segunda posição
        mov byte [square + 2], byte ']'         ; move ']' para a terceira posição

    init.player:
        call change_player                      ; chama a função para trocar o jogador (seta para 'x' na primeira vez)

    init.index:
        mov byte[index + 0], byte ' '           ; indice[0] = ' '
        mov byte[index + 1], byte ' '           ; indice[1] = ' '
        mov byte[index + 2], byte 0xa           ; indice[2] = '\n'
    ret

; ############################################################################
; # Troca o jogador atual
; ############################################################################
change_player:
    cmp byte [player], byte 'X'         ; vê se 'x' está na variável jogador
    je  change_player.o                 ; muda para 'o' se sim
    jne change_player.x                 ; muda para 'x' se não

    change_player.o:
        mov byte [player], byte 'O'     ; muda player para 'o'
        jmp change_player.done          ; termina a função

    change_player.x:
        mov byte [player], byte 'X'     ; muda player para 'x'
        jmp change_player.done          ; termina a função

    change_player.done:
        ret                             ; retorna

; ############################################################################
; # Recebe o input, verificar se foi válido e atualizar o tabuleiro
; ############################################################################
make_move:
    make_move.get_input:
        mov al, byte [player]
        mov byte [choose_your_move + 9], al
        printf choose_your_move, choose_your_move_len

        scanf x, 1                                          ; recebe o input em x
        scanf fool, 1                                       ; recebe o '\n'

        scanf y, 1                                          ; recebe o input em y
        scanf fool, 1                                       ; recebe o '\n'
        
        cmp byte [x], '3'                                   ; vê se o input é maior ou igual a 3
        jge make_move.get_input                             ; retorna se for

        cmp byte [y], '3'                                   ; vê se o input é maior ou igual a 3
        jge make_move.get_input                             ; retorna se for

    make_move.move:
        and byte [x], 0x0F                                  ; máscara para converter para número
        and byte [y], 0x0F                                  ; máscara para converter para número

        mov rbx, 0x0                                        ; rbx irá salvar a posição desejada
        mov rcx, 0x3                                        ; loop
        make_move.move.row:
            add bl, [x]                                     ; a linha adiciona três vezes (x * 3)
            loop make_move.move.row

        make_move.move.col: 
            add bl, [y]                                     ; coluna só uma vez
        
        cmp byte [board + rbx], byte ' '                    ; vê se o tabuleiro está vazio
        jne make_move.get_input                             ; pede novo input se não

    make_move.done:
        mov al, byte [player]                               ; move o player para o registrador al 
        mov byte [board + rbx], al                          ; move al para a posição do tabuleiro
    ret

; ############################################################################
; # Imprime o tabuleiro
; ############################################################################
print_board:
    print_board.col:
        mov byte [index + 1], '1'           ; índice da coluna
        mov rcx, 0x3                        ; loop 3 vezes              
        print_board.col.while:  
            push rcx                        ; push o contador do loop
            printf index, 2                 ; imprime o índice
            printf empty, 1                 ; imprime espaço em branco
            inc byte [index + 1]            ; incrementa o índice
            pop rcx                         ; pop o contador do loop
            loop print_board.col.while      ; repete
        printf eol, 1                       ; imprime fim-de-linha

    mov rdx, 0x0                            ; contador
    mov rcx, 3 * 3                          ; inicia o loop
    mov rbx, 0x0                            ; contador da posição atual do tabuleiro
    mov byte [index + 1], byte '1'          ; índice da linha
    printf eol, 1                           ; imprime uma lina em branco
    print_board.print:  
        push rcx                            ; 'push' contador do loop   
        push rdx                            ; 'push' contador da coluna atual

        mov al, byte [board + rbx]          ; move a posição atual para o registrador 'al'
        mov byte [square + 1], al           ; move o conteúdo de 'al' para o 'square'
        
        push rbx                            ; 'push' o contador da posição atual do tabuleiro   
        printf square, 3                    ; imprime 'square'
        pop rbx                             ; 'pop' o contador da posição atual do tabuleiro
        inc rbx                             ; incrementa o contador da posição atual do tabuleiro

        pop rdx                             ; 'pop' contador da coluna atual        
        inc rdx                             ; incrementa contador da coluna atual
        cmp rdx, 0x3                        ; vê se já está na última posição (3)
        jne print_board.print.end_loop      ; vai para a próxima iteração se não

        print_board.print.end_of_line:
            push rbx                        ; 'push' o contador da posição atual do tabuleiro
            printf index, 3                 ; imprime o índice da linha
            pop rbx                         ; 'pop' o contador da posição atual do tabuleiro
            inc byte [index + 1]            ; incrementa o índice da linha      
            mov rdx, 0x0                    ; reseta o contador da coluna atual

        print_board.print.end_loop:
            pop  rcx                        ; 'pop' o contador do loop
            loop print_board.print          ; repete tudo
    ret

; ############################################################################
; # Verifica o status do jogo
; ############################################################################
%macro check_for_equality 3
    mov al, 0x0                                             ; inica rax para 0
     or al, byte [board + %1]                               ; or a primeira coluna
    and al, byte [board + %2]                               ; and a segunda coluna
    xor al, byte [board + %3]                               ; xor a terceira coluna
    cmp al, byte 0x00                                       ; se todos forem iguais, o valor será 0
    je check_game_status.victory                            ; vitória se for zero
%endmacro

check_game_status:

    ; verifica a horizontal     
    check_game_status.horizontal:
        mov rbx, 0x0                                        ; inicia a posição do tabuleiro em zero
        mov rcx, 0x2
        check_game_status.horizontal.while: 
            cmp byte [board + rbx], byte ' '                ; se a primeira posição estiver vazia
            je check_game_status.horizontal.while.next      ; pule para a pŕoxima iteração

            check_for_equality rbx + 0, rbx + 1, rbx + 2    ; compara as linhas e vai para vitória se forem iguais
            add rbx, byte 0x03  
            
            check_game_status.horizontal.while.next:
                loop check_game_status.horizontal.while     ; loop

    ; verifica a vertical   
    check_game_status.vertical:
        mov rbx, 0x0
        mov rcx, 0x2
        check_game_status.vertical.while: 
            cmp byte [board + rbx], byte ' '                ; se a primeira posição estiver vazia
            je check_game_status.vertical.while.next        ; pule para a pŕoxima iteração

            check_for_equality rbx + 0, rbx + 3, rbx + 6    ; compara as linhas e vai para vitória se forem iguais
            add rbx, byte 0x01  
            
            check_game_status.vertical.while.next:
                loop check_game_status.vertical.while       ; loop
    
    check_game_status.main_diagonal:
        cmp byte [board + 4], byte ' '                      ; se o meio estiver vazio
        je check_game_status.second_diagonal                ; vai direto

        check_for_equality 0, 4, 8                          ; compara as linhas e vai para vitória se forem iguais
    
    check_game_status.second_diagonal:
        cmp byte [board + 4], byte ' '                      ; se o meio estiver vazio
        je check_game_status.next_round                     ; vai direto

        check_for_equality 6, 4, 2                          ; compara as linhas e vai para vitória se forem iguais
    
    check_game_status.next_round:
        mov rcx, 3 * 3                                      ; loop por todo o tabuleiro
        mov rax, 0x0                                        ; zero o rax
        jmp check_game_status.next_round.while              ; pula para o loop

        check_game_status.next_round.exists:
            mov byte [stats], nothing                       ; se existe próximo turno, retorna nothing
            ret

        check_game_status.next_round.while:
            cmp byte [board + rax], byte ' '                ; vê se tem alguma posição vazia
            je check_game_status.next_round.exists          ; se tiver retorna nothing
            inc rax                                         ; se não via pra próxima posição
            loop check_game_status.next_round.while         ; loop
        
    check_game_status.draw:
        mov byte [stats], draw                              ; retorna draw em caso de empate
        ret
    
    check_game_status.victory:
        mov byte [stats], victory                           ; retorna victory em caso de vitória
        ret 
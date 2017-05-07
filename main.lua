function draw_board(board, dumb)
    io.write('   ')
    for i = 1,3 do io.write(i, '  ') end
    io.write('\n')

    for i, row in ipairs(board) do
        io.write(i, ' ')
        for j, square in ipairs(row) do
            if (square == dumb) then
                io.write('[ ]')
            else
                io.write('[', square, ']')
            end
        end
        io.write('\n')
    end
end

function update_game_state(board, dumb, player, x, y)
    local win, draw, continue = 1, 2, 3

    -- Faz a jogada do jogador
    board[x][y] = player

    -- Horizontal e Vertical
    for i = 1, 3 do
        -- Se o meio estiver vazio, continue 
        if not (board[i][i] == dumb) then
            -- Horizontal
            if (board[i][1] == board[i][2] and board[i][1] == board[i][3]) then return win end
            -- Vertical
            if (board[1][i] == board[2][i] and board[1][i] == board[3][i]) then return win end
        end
    end

    if board[2][2] ~= dumb then
        -- Diagonal principal
        if (board[1][1] == board[2][2] and board[1][1] == board[3][3]) then return win end
        -- Diagonal Secundária
        if (board[3][1] == board[2][2] and board[3][1] == board[1][3]) then return win end
    end

    for i = 1, 3 do
        for j = 1, 3 do
            if (board[i][j] == dumb) then return continue end
        end
    end

    return draw
end

function main()
    dumb = '0' -- Variável tola
    board =    -- Tabuleiro
    {
        { dumb, dumb, dumb },
        { dumb, dumb, dumb },
        { dumb, dumb, dumb }
    }
    player = 'x' -- Variável do jogador
    in_game = true -- Variável de estado do jogo
    
    while (in_game) do
        draw_board(board, dumb)
        
        io.write("Jogador ", player, " faca sua jogada !\n")
        local x = io.read('*n')
        local y = io.read('*n')

        if ((x < 1 or x > 3) or (y < 1 or y > 3)) then io.write("Jogada fora do tabuleiro !\nJogue novamente !\n") 
        else
            if (board[x][y] ~= dumb) then io.write("Escolha uma casa vazia !\n")
            else
                local res = update_game_state(board, dumb, player, x, y)                
                -- Em caso de vitória
                if     res == 1 then
                    io.write("Jogador ", player, " venceu !\n")
                    in_game = false
                
                -- Em caso de empate
                elseif res == 2 then
                    io.write("Empatou !\n")
                    in_game = false
                
                -- Caso nada tenha acontecido
                 else   
                    -- Troca o jogador
                    if (player == 'x') then player = 'o' else player = 'x' end
                end
            end 
        end 

    end -- While
    draw_board(board, dumb)
end

main()
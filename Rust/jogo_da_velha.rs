use std::io;

fn draw_board(board: &Vec< Vec<char> >, dumb: &char)
{
    for i in 0..3 { print!(" {:?} ", i); }
    println!("");
    
    let mut i: u8 = 0;
    for row in board
    {
        for element in row
        {
            if *element == *dumb { print!("[ ]"); }
            else { print!("[{}]", element); }                
        }
        
        print!(" {}\n", i);
        i = i + 1;
    }
}

// Enum para o estado do jogo
enum GameState 
{
    WINNER,
    DRAW,
    NOTHING
}

fn check_game_state(board: &Vec< Vec<char> >, dumb: &char) -> GameState
{
    for i in 0..board.len()
    {
        // Se tiver algum vazio no meio, continue
        if board[i][i] == *dumb { continue; }

        // Horizontal
        if board[i][0] == board[i][1] && board[i][0] == board[i][2] { return GameState::WINNER; }

        // Vertical
        if board[0][i] == board[1][i] && board[0][i] == board[2][i] { return GameState::WINNER; }
    }

    // Diagonais
    if board[1][1] != *dumb 
    {   
        // Diagonal Principal
        if board[0][0] == board[1][1] && board[0][0] == board[2][2] { return GameState::WINNER; }

        // Diagonal Secundária
        if board[2][0] == board[1][1] && board[2][0] == board[0][2] { return GameState::WINNER; }
    }

    // Vê se estão todos ocupados
    for row in board
    {
        for element in row
        {
            if element == dumb { return GameState::NOTHING; }
        }
    }

    // Empate
    return GameState::DRAW;
}

// Loop do jogo 
fn new_game()
{
    // Dumb para espaços vazios
    let dumb: char = '0';

    // Variável para encerrar o jogo
    let mut in_game: bool = true;

    // Tabuleiro do jogo
    let mut board: Vec< Vec<char> > = vec![vec![dumb; 3]; 3]; 

    // Jogador x
    let mut player: char = 'x';

    // Mostra o tabuleiro a primeira vez
    draw_board(&board, &dumb);
    
    // Loop principal
    while in_game
    {   
        println!("\nJogador {:?}, faça sua jogada:", player);

        let mut s = String::new();
        io::stdin()
            .read_line(&mut s)
            .expect("Falha ao ler a linha !");
        
        let v: Vec<u32> = s.split(" ")
            .map(|x: &str| x.trim().parse::<u32>()
                    .expect("Falha ao converter para inteiro !"))
            .collect();

        let row: usize = v[0] as usize;
        let col: usize = v[1] as usize;
        
        if row > 2 || col > 2 { println!("Jogada fora do tabuleiro ! Jogue novamente !"); }
        else
        {   
            if board[row][col] == dumb
            { 
                board[row][col] = player;

                match check_game_state(&board, &dumb)
                {   
                    // Se houve vencedor
                    GameState::WINNER => 
                    {
                        println!("\nJogador {:?} Venceu !", player);
                        in_game = false;
                    },

                    // Se houve empate
                    GameState::DRAW =>
                    {
                        println!("\nEmpate !");
                        in_game = false; 
                    },

                    // Se não houve nada, troca o jogador e continue o jogo
                    GameState::NOTHING =>
                    {
                        if player == 'x' { player = 'o'; } else { player = 'x'; }
                    }
                }
            }
            else
            {
                println!("Casa ocupada ! Jogue novamente !");
            }
        }

        draw_board(&board, &dumb);
    }
}

// Entrada do programa
fn main()
{
    let mut again: bool = true;

    while again
    {
        new_game();

        println!("\nDeseja jogar novamente ? (s/n)");
        let mut s = String::new();
        io::stdin()
            .read_line(&mut s)
            .expect("Falha ao ler a linha !");

        match s.trim()
        {
            "s" => {},

            _ => { again = false },
        }
    } 
    println!("\nObrigado por jogar !");
}
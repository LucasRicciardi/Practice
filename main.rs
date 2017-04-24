use std::io;

fn draw_board(board: &Vec< Vec<char> >)
{
	for row in board
	{
		for element in row
		{
			print!("[{}]", element);
		}
		println!("");
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
		if board[0][i] == board[1][i] && board[i][0] == board[2][i] { return GameState::WINNER; }
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

fn main()
{
	// Dumb para espaços vazios
	let dumb = '0';

	// Variável para encerrar o jogo
	let mut in_game = true;

	// Tabuleiro do jogo
	let mut board: Vec< Vec<char> > = vec![vec![dumb; 3]; 3]; 

	// Jogador x
	let mut player = 'x';

	// Loop principal
	while in_game
	{	
		println!("\nJogador {:?}, faça sua jogada:", player);

		let mut s = String::new();
		io::stdin()
			.read_line(&mut s)
			.expect("Falha ao ler a linha !");
		
		let v: Vec<u32> = s.split(" ")
			.map(|x| x.trim().parse::<u32>()
					.expect("Falha ao converter o para inteiro !"))
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
						println!("Jogador {:?} Venceu !", player);
						in_game = false;
					},

					// Se houve empate
					GameState::DRAW =>
					{
						println!("Empate !");
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

		draw_board(&board);
	}
}
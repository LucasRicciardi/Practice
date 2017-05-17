#include <stdio.h>

#define empty ' '

void print_board(char board[3][3])
{
	printf("\n  ");
	for (int i = 0; i < 3; i++)
	{
		printf(" %d ", i);
	}
	printf("\n");

	for (int i = 0; i < 3; i++)
	{
		printf("%d ", i);
		for (int j = 0; j < 3; j++)
		{
			printf("[%c]", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int make_move(char board[3][3], char player)
{
	int x, y;
	scanf("%d %d", &x, &y);
	return (board[x][y] == empty) ?
		(board[x][y] = player, 1) : (printf("\nJogue novamente !\n"), 0); 
}

enum State
{
	victory,
	draw,
	nothing
};

enum State check_game_status(char board[3][3])
{
	// Vertical e Horizontal
	for (int i = 0; i < 3; i++)
	{
		if (board[i][i] == empty) continue;

		// Vertical
		if (board[0][i] == board[1][i] && board[0][i] == board[2][i]) return victory;
		// Horizontal
		if (board[i][0] == board[i][1] && board[i][0] == board[i][2]) return victory;
	}

	// Diagonais
	if (board[1][1] != empty)
	{
		// Principal
		if (board[0][0] == board[1][1] && board[0][0] == board[2][2]) return victory;
		// Secundária
		if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return victory;
	}

	// Ainda há jogo
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == empty) return nothing;
		}
	}

	// Empate
	return draw;
}

void game()
{	
	int in_game = 1;	
	char player = 'x';
	char board[3][3] = 
	{
		{ empty, empty, empty },
		{ empty, empty, empty },
		{ empty, empty, empty }
	};
	print_board(board);

	while (in_game)
	{
		while (!make_move(board, player));

		switch (check_game_status(board))
		{
			#define end_game(msg, winner) \
				in_game = 0; 			  \
				printf(msg, winner)
			
			case victory: 
			{
				end_game("\nJogador %c venceu !\n", player);	
			} break;

			case draw:
			{
				end_game("\nEmpatou !%c", '\n');
			} break;

			case nothing:
			{
				(player == 'x') ? (player = 'o') : (player = 'x');
			} break;

			#undef end_game
		}
		print_board(board);
	}
}

int main()
{
	game();
	return 0;
}
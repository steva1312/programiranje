#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

// color codes
int WHITE = 15;
int BLUE = 9;
int RED = 12;

// global variables
const int charNums[9] = { '1','2','3','4','5','6','7','8','9' };
char board[9];
const int rows = 9;
const int columns = 17;
const int width = 5;
const int height = 3;
const int xoPosX = width / 2 + 1;
const int xoPosY = height / 2 + 1;
const int KEY_ZERO = 48;
const int KEY_NINE = 57;
const char x = 'X';
const char o = 'O'; 
const int colorX = RED;
const int colorO = BLUE;
int movesX[5];
int movesO[5];
char firstMove = x;
char activePlayer = firstMove;
int movesCount = 0;
const int combos[8][3] = {
	{1,2,3},
	{4,5,6},
	{7,8,9},
	{1,4,7},
	{2,5,8},
	{3,6,9},
	{1,5,9},
	{3,5,7}
};
bool gameOver = false;
string gameOverMsg;

// main functions
void SetBoard();
void Draw();
void MainLoop();

int main()
{
	SetBoard();
	Draw();
	MainLoop();
}

// function declarations
void SetBoard()
{
	for (int i = 0; i < 9; ++i)
		board[i] = charNums[i];
}

void LogChar(char c) {
	int color = WHITE;

	if (c == x)
		color = colorX;
	else if (c == o)
		color = colorO;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	cout << c;
}

char OppositePlayer(char player)
{
	if (player == x)
		return o;
	return x;
}

void PrintMsg()
{
	if (gameOver) // win  only
		cout << "Player " << OppositePlayer(activePlayer) << " won!" << endl;
	else if (movesCount == 9 && !gameOver) { // tie
		cout << "Tie!" << endl;
		gameOver = true;
	}
	else
		cout << "Player " << activePlayer << " is playing ...";

	if (gameOver) // both win and tie
		cout << "Press any key to play again ...";
}

void Draw()
{
	system("cls");

	char* boardptr = board;
	for (int i = 1; i <= rows; ++i)
	{
		for (int j = 1; j <= columns; ++j)
		{
			if (j % (width + 1) == 0)
				LogChar('|');
			else if (i == height || i == height * 2)
				LogChar('_');
			else if ((i == xoPosY || i == xoPosY + height || i == xoPosY + height * 2) && (j == xoPosX || j == xoPosX + (width+1) || j == xoPosX + (width+1)*2))
			{
				LogChar(*boardptr);
				++boardptr;
			}
			else
				LogChar(' ');
		}
		LogChar('\n');
	}
	PrintMsg();
}

int* GetMovesXO()
{
	if (activePlayer == x)
		return movesX;
	return movesO;
}

void CheckGameOver()
{
	int* moves = GetMovesXO();
	for (int i = 0; i < 8; ++i) // iterate through combos
	{
		const int* combo = combos[i];
		bool is_combo = true;

		for (int j = 0; j < 3; ++j) // iterate through numbers in combo
		{
			int e = combo[j];
			bool is_e = false;

			for (int l = 0; l < 5; ++l) // iterate through moves
			{
				int move = moves[l];
				if (e == move)
					is_e = true;
			}

			if (!is_e) 
				is_combo = false;
		}

		if (is_combo)
			gameOver = true;
	}
}

void ResetMoves(int* moves)
{
	for (int i = 0; i < 5; ++i)
		moves[i] = 0;
}

void Restart()
{
	SetBoard();
	ResetMoves(movesX);
	ResetMoves(movesO);
	firstMove = OppositePlayer(firstMove);
	activePlayer = firstMove;
	movesCount = 0;
	gameOver = false;

	int _ = _getch();

	Draw();
}

void MainLoop()
{
	while (1)
	{
		int key = _getch();
		if (key > KEY_ZERO && key <= KEY_NINE && !gameOver)
		{
			int index = key - KEY_ZERO - 1;
			if (board[index] == x || board[index] == o) continue;
			
			*(GetMovesXO() + movesCount / 2) = index + 1;
			++movesCount;
			board[index] = activePlayer;

			CheckGameOver();

			activePlayer = OppositePlayer(activePlayer);
			Draw();

			if (gameOver)
				Restart();
		}
	}
}
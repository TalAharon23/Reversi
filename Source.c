#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

//Tal Aharon - 208458414

/*
* Reversi game - is a two players board game,
* the players selects the board size (10x10, 15x15 or 20x20)
* and then they play against each other, each player puts a component on board (if it is a valid position)
* and the winner is who has the most components on board, or if the loser has no moves to do, the rival wins.
*/

#define ROWS 20
#define COLS 20
#define MAX_STR_PLAYER 20
#define PLAYER1 'X'
#define PLAYER2 'O' 
#define EMPTY_PLACE '_'


//Functions definitions:
void gameManager(char board[][COLS], int boardSize, char playerChar, char player1[], char player2[]);
int boardSizeInput();
void initBoard(char board[][COLS], int boardSize);
void printBoard(char board[][COLS], int boardSize);
char switchPlayers(char playerChar);
void askForRowAndCol(char playerChar, char player1[], char player2[]);
void newMove(char board[][COLS], int boardSize, char playerChar, char player1[], char player2[]);
bool isValidMove(char board[][COLS], int boardSize, char playerChar, int row, int col);
bool checkPath(char board[][COLS], int boardSize, char playerChar, char direction[], char dest, int row, int col);
void flipPath(char board[][COLS], int boardSize, char playerChar, char direction[], int row, int col);
int isFinish(char board[][COLS], int boardSize, char playerChar);
int countOfPlayerComp(char board[][COLS], int boardSize, char playerChar);
bool isBoardFull(char board[][COLS], int boardSize);
bool isThereAnyPlayerComp(char board[][COLS], int boardSize, char playerChar);
void whoWon(char board[][COLS], int boardSize, char player1[], char player2[]);


void main() {
	int boardSize = 0;
	char board[ROWS][COLS];

	char player1[MAX_STR_PLAYER], player2[MAX_STR_PLAYER];
	char playerChar = PLAYER1;

	printf("******* Reversi Game *******\n");

	gameManager(board, boardSize, playerChar, player1, player2);
}

void gameManager(char board[][COLS], int boardSize, char playerChar, char player1[], char player2[]) {
	// This function is responsible for all of the steps - from the beginning till the game finished.
	int p1Comps, p2Comps;

	boardSize = boardSizeInput(); //Ask for board size.
	initBoard(board, boardSize);

	printf("Please enter player 1: ");
	scanf("%s", player1);
	printf("Please enter player 2: ");
	scanf("%s", player2);

	printBoard(board, boardSize);
	while (!isBoardFull(board, boardSize)) { // This while runs the game till the board is'nt full.
		if (isThereAnyPlayerComp(board, boardSize, playerChar)) { // Check if the current player still has components on board.
			if (isFinish(board, boardSize, playerChar) == (-1)) { // Check if the current player has some valid move.
				newMove(board, boardSize, playerChar, player1, player2);
				printBoard(board, boardSize);
				playerChar = switchPlayers(playerChar);
			}
			else { // If the current player has no valid moves, the turn is moving to the opponent.
				if (isFinish(board, boardSize, switchPlayers(playerChar)) != (-1)) {//The opponent has no moved to do too. Game finished. 
					printf("You both don't have moves to do... Game finished!\n");
					break;
				}
				if (playerChar == PLAYER1)
					printf("%s (%c), it seems that you don't have a move to do! :( \n", player1, playerChar);
				else
					printf("%s (%c), it seems that you don't have a move to do! :( \n", player2, playerChar);
				playerChar = switchPlayers(playerChar);
				printBoard(board, boardSize);
			}
		}
		else { // If the current player has no components left, the opponent won.
			p1Comps = countOfPlayerComp(board, boardSize, PLAYER1);
			p2Comps = countOfPlayerComp(board, boardSize, PLAYER2);
			if (playerChar == PLAYER1) {
				printf("------------%s you are the WINNER! with %d VS %d ------------\n", player2, p2Comps, p1Comps);
			}
			else
				printf("------------%s you are the WINNER! with %d VS %d ------------\n", player1, p1Comps, p2Comps);
			break;
		}
	}

	whoWon(board, boardSize, player1, player2);
}

int boardSizeInput() {
	//This function asking for board size from the players, and exit the game if pressed 0.
	int boardSize;

	printf("Please enter the board size:\n");
	printf("Press 1 for 10X10\n");
	printf("Press 2 for 15X15\n");
	printf("Press 3 for 20X20\n");
	printf("Press 0 to exit.\n");

	scanf("%d", &boardSize);
	if (boardSize == 0)
		exit();
	while (boardSize > 3 || boardSize < 1) {
		printf("Inavlid choice. \n");
		printf("Please enter the board size:\n");
		scanf("%d", &boardSize);
	}

	if (boardSize == 1)
		return 10;
	if (boardSize == 2)
		return 15;
	return 20;
}

void initBoard(char board[][COLS], int boardSize) {
	//This function initializing the board --> two components for each player.
	int i, j;
	int startingPoint = boardSize / 2;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {

			if (i == startingPoint - 1 && j == startingPoint - 1) // 6,6
				board[i][j] = PLAYER1;
			else if (i == startingPoint - 1 && j == startingPoint) //6,7
				board[i][j] = PLAYER2;
			else if (i == startingPoint && j == startingPoint - 1) //7,6
				board[i][j] = PLAYER2;
			else if (i == startingPoint && j == startingPoint) //7,7
				board[i][j] = PLAYER1;
			else
				board[i][j] = EMPTY_PLACE;
		}
	}
}

void printBoard(char board[][COLS], int boardSize) {
	//This function prints the board.
	int i, j;

	printf("  ");
	for (i = 1; i <= boardSize; i++) {
		printf("%3d", i);
	}

	printf("\n");

	for (i = 0; i < boardSize; i++) {
		if (i >= 9)
			printf("%d", i + 1);
		else
			printf("%d ", i + 1);

		for (j = 0; j < boardSize; j++) {
			printf("%3c", board[i][j]);

		}
		printf("\n");
	}
}

char switchPlayers(char playerChar)
// This function replaces between the current player to the opponent player.
{
	if (playerChar == PLAYER1)
		playerChar = PLAYER2;
	else
		playerChar = PLAYER1;
	return playerChar;
}

void askForRowAndCol(char playerChar, char player1[], char player2[]) {
	//This function asking for a position from the player. 
	if (playerChar == PLAYER1)
		printf("%s (%c) it's your turn, please enter row and column \n", player1, playerChar);
	else
		printf("%s (%c) it's your turn, please enter row and column \n", player2, playerChar);
}

void newMove(char board[][COLS], int boardSize, char playerChar, char player1[], char player2[]) {
	//This function responsible for getting from the current player a position till it's valid position for flipping opponent's components.

	int row, col;

	askForRowAndCol(playerChar, player1, player2);

	scanf("%d%d", &row, &col);

	while (!isValidMove(board, boardSize, playerChar, row - 1, col - 1)) { //Runs till the player's input is a valid move.
		askForRowAndCol(playerChar, player1, player2);
		scanf("%d%d", &row, &col);
	}
	board[row - 1][col - 1] = playerChar;
}

bool isValidMove(char board[][COLS], int boardSize, char playerChar, int row, int col) {
	//This function gets position, and return if this position valid:
	//If the position is on the board frame.
	//If the position has some potential to flip opponent's component on any direction.
	//Direction: Up, UpLeft, UpRight, Left, Right, DownLeft, DownRight, Down.

	bool flag = false;
	if (row < 0 || row > boardSize - 1 || col < 0 || col > boardSize - 1 || board[row][col] != EMPTY_PLACE) { //checking if the move doesnt deviate from the mat or if there is some component there.
		printf("Invalid move.\n");
		return false;
	}


	if (checkPath(board, boardSize, playerChar, "UP", playerChar, row, col)) {
		flipPath(board, boardSize, playerChar, "UP", row, col);
		flag = true;
	}
	if (checkPath(board, boardSize, playerChar, "UR", playerChar, row, col)) {
		flipPath(board, boardSize, playerChar, "UR", row, col);
		flag = true;
	}
	if (checkPath(board, boardSize, playerChar, "UL", playerChar, row, col)) {
		flipPath(board, boardSize, playerChar, "UL", row, col);
		flag = true;
	}
	if (checkPath(board, boardSize, playerChar, "L", playerChar, row, col)) {
		flipPath(board, boardSize, playerChar, "L", row, col);
		flag = true;
	}
	if (checkPath(board, boardSize, playerChar, "R", playerChar, row, col)) {
		flipPath(board, boardSize, playerChar, "R", row, col);
		flag = true;
	}
	if (checkPath(board, boardSize, playerChar, "DL", playerChar, row, col)) {
		flipPath(board, boardSize, playerChar, "DL", row, col);
		flag = true;
	}
	if (checkPath(board, boardSize, playerChar, "DR", playerChar, row, col)) {
		flipPath(board, boardSize, playerChar, "DR", row, col);
		flag = true;
	}
	if (checkPath(board, boardSize, playerChar, "D", playerChar, row, col)) {
		flipPath(board, boardSize, playerChar, "D", row, col);
		flag = true;
	}


	if (flag == false) { //If there in no good direction.
		printf("Invalid move.\n");
		return false;
	}

	return true;
}

bool checkPath(char board[][COLS], int boardSize, char playerChar, char direction[], char dest, int row, int col) {
	//This function gets the direction and the destination place,
	//and returns true if the path from the exsiting position to the destination position is valid - according to the current player. 
	bool flag = false;

	if (direction == "UP") {
		if (board[row - 1][col] == switchPlayers(playerChar)) {
			row--;
			while (board[row][col] == switchPlayers(playerChar)) {
				row--;
			}
			if (board[row][col] == dest) {
				flag = true;
			}
			else flag = false;
		}
	}

	else if (direction == "UR") {
		if (board[row - 1][col + 1] == switchPlayers(playerChar)) {
			row--;
			col++;
			while (board[row][col] == switchPlayers(playerChar)) {
				row--;
				col++;
			}
			if (board[row][col] == dest) {
				flag = true;
			}
			else flag = false;
		}
	}

	else if (direction == "UL") {
		if (board[row - 1][col - 1] == switchPlayers(playerChar)) {
			row--;
			col--;
			while (board[row][col] == switchPlayers(playerChar)) {
				row--;
				col--;
			}
			if (board[row][col] == dest) {
				flag = true;
			}
			else flag = false;
		}
	}

	else if (direction == "L") {
		if (board[row][col - 1] == switchPlayers(playerChar)) {
			col--;
			while (board[row][col] == switchPlayers(playerChar)) {
				col--;
			}
			if (board[row][col] == dest) {
				flag = true;
			}
			else flag = false;
		}
	}

	else if (direction == "R") {
		if (board[row][col + 1] == switchPlayers(playerChar)) {
			col++;
			while (board[row][col] == switchPlayers(playerChar)) {
				col++;
			}
			if (board[row][col] == dest) {
				flag = true;
			}
			else flag = false;
		}
	}

	else if (direction == "DL") {
		if (board[row + 1][col - 1] == switchPlayers(playerChar)) {
			row++;
			col--;
			while (board[row][col] == switchPlayers(playerChar)) {
				row++;
				col--;
			}
			if (board[row][col] == dest) {
				flag = true;
			}
			else flag = false;
		}
	}

	else if (direction == "DR") {
		if (board[row + 1][col + 1] == switchPlayers(playerChar)) {
			row++;
			col++;
			while (board[row][col] == switchPlayers(playerChar)) {
				row++;
				col++;
			}
			if (board[row][col] == dest) {
				flag = true;
			}
			else flag = false;
		}
	}

	else if (direction == "D") {
		if (board[row + 1][col] == switchPlayers(playerChar)) {
			row++;
			while (board[row][col] == switchPlayers(playerChar)) {
				row++;
			}
			if (board[row][col] == dest) {
				flag = true;
			}
			else flag = false;
		}
	}
	return flag;
}

void flipPath(char board[][COLS], int boardSize, char playerChar, char direction[], int row, int col) {
	//The function gets the direction and the position, then flips all the opponent's components on the direction. 

	if (direction == "UP") {
		row--;
		while (board[row][col] == switchPlayers(playerChar)) {
			board[row][col] = playerChar;
			row--;
		}
	}
	else if (direction == "UR") {
		row--;
		col++;
		while (board[row][col] == switchPlayers(playerChar)) {
			board[row][col] = playerChar;
			row--;
			col++;
		}
	}
	else if (direction == "UL") {
		row--;
		col--;
		while (board[row][col] == switchPlayers(playerChar)) {
			board[row][col] = playerChar;
			row--;
			col--;
		}
	}
	else if (direction == "L") {
		col--;
		while (board[row][col] == switchPlayers(playerChar)) {
			board[row][col] = playerChar;
			col--;
		}
	}
	else if (direction == "R") {
		col++;
		while (board[row][col] == switchPlayers(playerChar)) {
			board[row][col] = playerChar;
			col++;
		}
	}
	else if (direction == "DL") {
		row++;
		col--;
		while (board[row][col] == switchPlayers(playerChar)) {
			board[row][col] = playerChar;
			row++;
			col--;
		}
	}
	else if (direction == "DR") {
		row++;
		col++;
		while (board[row][col] == switchPlayers(playerChar)) {
			board[row][col] = playerChar;
			row++;
			col++;
		}
	}
	else if (direction == "D") {
		row++;
		while (board[row][col] == switchPlayers(playerChar)) {
			board[row][col] = playerChar;
			row++;
		}
	}
}

int isFinish(char board[][COLS], int boardSize, char playerChar) {
	// The function returns:
	// (-1) ---> If there is a move for the player.
	//  0   ---> If the board is'nt full, but there is no move for the player (means now it should be the opponent turn).
	//  1   ---> The game finished - board is full!
	int i, j;

	if (isBoardFull(board, boardSize)) { // If the board is full --> finished!
		return 1;
	}

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {

			if (board[i][j] == playerChar) {

				if ((checkPath(board, boardSize, playerChar, "UP", EMPTY_PLACE, i, j)) || //if there is a move to do --> if not finished.
					(checkPath(board, boardSize, playerChar, "UR", EMPTY_PLACE, i, j)) ||
					(checkPath(board, boardSize, playerChar, "UL", EMPTY_PLACE, i, j)) ||
					(checkPath(board, boardSize, playerChar, "L", EMPTY_PLACE, i, j)) ||
					(checkPath(board, boardSize, playerChar, "R", EMPTY_PLACE, i, j)) ||
					(checkPath(board, boardSize, playerChar, "DL", EMPTY_PLACE, i, j)) ||
					(checkPath(board, boardSize, playerChar, "DR", EMPTY_PLACE, i, j)) ||
					(checkPath(board, boardSize, playerChar, "D", EMPTY_PLACE, i, j))) {
					return -1;
				}
			}
		}

	}

	return 0; // if the board isn't full and there is no player components.
}

int countOfPlayerComp(char board[][COLS], int boardSize, char playerChar) {
	// This function gets the player, and returns his number of components.
	int count = 0;
	int i, j;

	for (i = 0; i < boardSize; i++)
	{
		for (j = 0; j < boardSize; j++)
		{
			if (board[i][j] == playerChar)
				count++;
		}
	}

	return count;
}

bool isBoardFull(char board[][COLS], int boardSize) {
	// This function checks if the board is full, if it does --> return true, if not --> return false.
	int i, j;
	for (i = 0; i < boardSize; i++)
	{
		for (j = 0; j < boardSize; j++)
		{
			if (board[i][j] == EMPTY_PLACE)
				return false;
		}
	}
	return true;
}

bool isThereAnyPlayerComp(char board[][COLS], int boardSize, char playerChar) {
	//This function gets a player and checks if he still has components on board.
	int i, j;
	for (i = 0; i < boardSize; i++)
	{
		for (j = 0; j < boardSize; j++) {
			if (board[i][j] == playerChar)
				return true;
		}
	}
	return false;
}
void whoWon(char board[][COLS], int boardSize, char player1[], char player2[]) {
	//The function return checks who has more compnents on board, and prints who won and how much components each player has.

	int p1Comps = countOfPlayerComp(board, boardSize, PLAYER1);
	int p2Comps = countOfPlayerComp(board, boardSize, PLAYER2);

	if (p1Comps > p2Comps) {
		printf("------------%s you are the WINNER! with %d VS %d ------------\n", player1, p1Comps, p2Comps);
	}
	else if (p1Comps < p2Comps) {
		printf("------------%s you are the WINNER! with %d VS %d ------------\n", player2, p2Comps, p1Comps);
	}
	else
		printf("------------%s It's a TIE! both of you with %d ------------\n", player2, p1Comps);
}

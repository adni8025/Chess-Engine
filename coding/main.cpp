#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <unordered_map>

#define PawnBlack 1
#define PawnWhite -1
#define RookBlack 2
#define RookWhite -2
#define KnightWhite -3
#define KnightBlack 3
#define BishopBlack 4
#define BishopWhite -4
#define QueenWhite -5
#define QueenBlack 5
#define KingBlack 6
#define KingWhite -6
#define Color 2  //Color to move
#define CastleSide 2   //Side to castle

using namespace std;

int board[12][12] = //Storage of chess position  
	{
	// 7 = Square off the board 
	// 0 = Empty square on the board
	//Positive number = Black piece 
	//Negative number = White piece
	// 1, -1 = pawn
	// 2, -2 = rook
	// 3, -3 = knight
	// 4, -4 = bishop
	// 5, -5 = queen
	// 6, -6 = king
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
		7,  7,  2,  3,  4,  5,  6,  4,  3,  2,  7,  7,
		7,  7,  1,  1,  1,  1,  1,  1,  1,  1,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7, -1, -1, -1, -1, -1, -1, -1, -1,  7,  7,
		7,  7, -2, -3, -4, -5, -6, -4, -3, -2,  7,  7,
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
	};

int tempBoard[12][12] = //Used to test if a position is valid
	{
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
		7,  7,  2,  3,  4,  5,  6,  4,  3,  2,  7,  7,
		7,  7,  1,  1,  1,  1,  1,  1,  1,  1,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,
		7,  7, -1, -1, -1, -1, -1, -1, -1, -1,  7,  7,
		7,  7, -2, -3, -4, -5, -6, -4, -3, -2,  7,  7,
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
		7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
	};
int turn = 0; //0 is white to move -- 1 is black to move 
int TotalMoves = 0;
int draw = 0, whiteWin = 0, blackWin = 0;
int moveSel = 0;
int dx = 0;
int dy = 0;
int oX = 0;
int oY = 0;
sf::Sprite selectedSprite;
// Constants
const int BOARD_SIZE = 8;
const int WINDOW_SIZE = 800;
const std::string IMAGE_PATH = "imgs/";
const int TILE_SIZE = WINDOW_SIZE / BOARD_SIZE;

class chess
{
private:
	struct pos
	{
		int x;
		int	y;
	} kingW, kingB;

	int totalMovesplayed = 0;
	int blackWins = 0, whiteWins = 0, stalemate = 0;

	int countMoves = 1;

	int resetFiftyMoveRule = 1, countFiftyMoveRule = 0;
	
	int castleRights[Color][CastleSide] = //Index Color 0 = white -- Index Color 1 = black -- Index CastleSide 0 = Queen Side -- Index CastleSide 1 = King side
	{
		1, 1, //1 = allowed to castle, 0= not allowed to castle 
		1, 1,
	};
	int hasBlackKingMoved = 0, hasWhiteKingMoved = 0, hasLeftBlackRookMoved = 0, 
	hasRightBlackRookMoved = 0, hasLeftWhiteRookMoved = 0, hasRightWhiteRookMoved = 0, 
	targetSquarex = 0, targetSquarey = 0, validSquare = 0; //Initalizing variables to check if rooks and kings have moved for castling and en passant squares

public:
string RemoveSpacesFromAllMoves(string longListWithSpaces)
{
	int count = 0;
	string allPossibleMoves = "";
	for(int i = 0; i<longListWithSpaces.length() - 1; i++)
	{
		if(longListWithSpaces[i] == ' ' && longListWithSpaces[i+1] != ' ') //Makre sure list with moves won't have spaces in last 5 i values
		{
			allPossibleMoves+=longListWithSpaces[i+1];
			allPossibleMoves+=longListWithSpaces[i+2];
			allPossibleMoves+=longListWithSpaces[i+3];
			allPossibleMoves+=longListWithSpaces[i+4];
			if(longListWithSpaces[i+5] == '=')
			{
				allPossibleMoves+=longListWithSpaces[i+5];
				allPossibleMoves+=longListWithSpaces[i+6];
			}
			allPossibleMoves+=' ';
			count++;
		}
		TotalMoves = count;
	}
	
	return allPossibleMoves;
}
void FENtoBoard(string positionFEN)
{
	for(int i = 2; i<10; i++)
	{
		for(int j = 2; j<10; j++)
		{
			board[j][i] = 0;
		}
	}
	int row = 2;
	int spacingCol = 0;
	int numSpaces = 0;
	for(int i= 0; i<positionFEN.length() && row<10; i++)
	{
		if(positionFEN[i] == '/')
		{
			spacingCol=0;
			row++;
			continue;
		}
		else if(isdigit(positionFEN[i]))
		{
			spacingCol += (positionFEN[i] - '0');
			continue;
		}
		else if(positionFEN[i] == ' ')
		{
			if(numSpaces == 0)
			{
				if(positionFEN[i+1] == 'w')
				{
					turn = 0;
				}
				else if(positionFEN[i+1] == 'b')
				{
					turn = 1;
				}
			}
			else if(numSpaces == 1)
			{
				int j = i+1;
				while(true)
				{
					if(positionFEN[j] != 'k' && positionFEN[j] != 'K' && positionFEN[j] != 
					'q' && positionFEN[j] != 'Q' && positionFEN[j] != 'H' && positionFEN[j] != 'A'
					 && positionFEN[j] != 'h' && positionFEN[j] != 'a')
					{
						break;
					}
					else if(positionFEN[j] == 'k')
					{
						castleRights[1][1] = 1;
					}
					else if(positionFEN[j] == 'K')
					{
						castleRights[0][1] = 1;
					}
					else if(positionFEN[j] == 'q')
					{
						castleRights[1][0] = 1;
					}
					else if(positionFEN[j] == 'Q')
					{
						castleRights[0][0] = 1;
					}
					j++;
				}
			}
			numSpaces++;
		}
		else if(positionFEN[i] == 'p' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = PawnBlack;
		}
		else if(positionFEN[i] == 'r' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = RookBlack;
		}
		else if(positionFEN[i] == 'n' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = KnightBlack;
		}
		else if(positionFEN[i] == 'b' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = BishopBlack;
		}
		else if(positionFEN[i] == 'q' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = QueenBlack;
		}
		else if(positionFEN[i] == 'k' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = KingBlack;
		}
		else if(positionFEN[i] == 'P' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = PawnWhite;
		}
		else if(positionFEN[i] == 'R' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = RookWhite;
		}
		else if(positionFEN[i] == 'N' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = KnightWhite;
		}
		else if(positionFEN[i] == 'B' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = BishopWhite;
		}
		else if(positionFEN[i] == 'Q' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = QueenWhite;
		}
		else if(positionFEN[i] == 'K' && board[row][spacingCol+2] != 7)
		{
			board[row][spacingCol+2] = KingWhite;
		}
		spacingCol++;
	}
}
void kingPosW() 
{
	for (int i = 2; i < 10; i++)
	{
		for (int j = 2; j < 10; j++)
		{
			if (board[i][j] == KingWhite)
			{
				kingW.x = j;
				kingW.y = i;
			}
		}
	}
}
void kingPosB()
{
	for (int i = 2; i < 10; i++)
	{
		for (int j = 2; j < 10; j++)
		{
			if (board[i][j] == KingBlack)
			{
				kingB.x = j;
				kingB.y = i;
			}
		}
	}
}
void CopyingToTempBoard()
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			tempBoard[i][j] = board[i][j];
		}
	}
}
void CopyingToMainBoard()
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			board[i][j] = tempBoard[i][j];
		}
	}
}
string convertIndexToSqr(int x, int y)
{
	int z = 10 - y;
	char c = (char)x + 95;
	string a = to_string(z);
	return c + a;
}
int IsOpposingKingNearby(int myKingX, int myKingY, int oppKingX, int oppKingY)
{
	kingPosW();
	kingPosB();
	if(myKingX==kingW.x && myKingY==kingW.y && oppKingX==kingB.x && oppKingY==kingB.y)
	{
		if(board[myKingY-1][myKingX] == KingBlack || board[myKingY-1][myKingX-1] == KingBlack || board[myKingY][myKingX-1] == KingBlack
		|| board[myKingY+1][myKingX-1] == KingBlack || board[myKingY+1][myKingX] == KingBlack || board[myKingY+1][myKingX+1] == KingBlack 
		|| board[myKingY][myKingX+1] == KingBlack || board[myKingY-1][myKingX+1] == KingBlack)
		{
			return 1;
		}
	}
	else if(myKingX==kingB.x && myKingY==kingB.y && oppKingX==kingW.x && oppKingY==kingW.y)
	{
		if(board[myKingY-1][myKingX] == KingWhite || board[myKingY-1][myKingX-1] == KingWhite || board[myKingY][myKingX-1] == KingWhite
		|| board[myKingY+1][myKingX-1] == KingWhite || board[myKingY+1][myKingX] == KingWhite || board[myKingY+1][myKingX+1] == KingWhite 
		|| board[myKingY][myKingX+1] == KingWhite || board[myKingY-1][myKingX+1] == KingWhite)
		{
			return 1;
		}
	}
	return 0;
}

int PawnCheckW(int oldX, int oldY, int kingX, int kingY)
{
	//Check if pawn is attacking king up and to the left
	if (board[oldY - 1][oldX - 1] == KingBlack && (oldX - 1 == kingX && oldY - 1 == kingY))
	{
		return 1;
	}

	//Check if pawn is attacking king up and to the right
	if (board[oldY - 1][oldX + 1] == KingBlack && (oldX + 1 == kingX && oldY - 1 == kingY))
	{
		return 1;
	}

	return 0;
}
int PawnCheckB(int oldX, int oldY, int kingX, int kingY)
{
	//Check if pawn is attacking king down and to the left
	if (board[oldY + 1][oldX - 1] == KingWhite && (oldX - 1 == kingX && oldY + 1 == kingY))
	{
		return 1;
	}

	//Check if pawn is attacking king down and to the right
	if (board[oldY + 1][oldX + 1] == KingWhite && (oldX + 1 == kingX && oldY + 1 == kingY))
	{
		return 1;
	}

	return 0;
}
int RookCheckW(int oldX, int oldY, int kingX, int kingY)
{
	for (int i = oldX - 1; i >= 0; i--) //Loop for rook moves to the left
	{
		if (board[oldY][i] == KingBlack && (i == kingX && oldY == kingY))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldX + 1; i <= 11; i++) //Loop for rook moves to the right
	{
		if (board[oldY][i] == KingBlack && (kingX == i && kingY == oldY))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldY - 1; i >= 0; i--) //Loop for rook moves up
	{
		if (board[i][oldX] == KingBlack && (kingY == i && oldX == kingX))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[i][oldX] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldY + 1; i <= 11; i++) //Loop for rook moves down
	{
		if (board[i][oldX] == KingBlack && (kingY == i && oldX == kingX))
		{
			//Allow the piece to move if squares in between are empty or if there is an enemy piece where we want to move
			return 1;
		}
		else if (board[i][oldX] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}
	return 0;
}
int RookCheckB(int oldX, int oldY, int kingX, int kingY)
{
	for (int i = oldX - 1; i >= 0; i--) //Loop for rook moves to the left
	{
		if (board[oldY][i] == KingWhite && (i == kingX && oldY == kingY))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldX + 1; i <= 11; i++) //Loop for rook moves to the right
	{
		if (board[oldY][i] == KingWhite && (kingX == i && kingY == oldY))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldY - 1; i >= 0; i--) //Loop for rook moves up
	{
		if (board[i][oldX] == KingWhite && (kingY == i && oldX == kingX))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[i][oldX] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldY + 1; i <= 11; i++) //Loop for rook moves down
	{
		if (board[i][oldX] == KingWhite && (kingY == i && oldX == kingX))
		{
			//Allow the piece to move if squares in between are empty or if there is an enemy piece where we want to move
			return 1;
		}
		else if (board[i][oldX] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}
	return 0;
}
int KnightCheckW(int oldX, int oldY, int kingX, int kingY)
{
	if (board[oldY - 2][oldX + 1] == KingBlack && (oldX + 1 == kingX && oldY - 2 == kingY)) //up right -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY - 2][oldX - 1] == KingBlack && (oldX - 1 == kingX && oldY - 2 == kingY)) //up left -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY - 1][oldX - 2] == KingBlack && (oldX - 2 == kingX && oldY - 1 == kingY)) //left up -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY + 1][oldX - 2] == KingBlack && (oldX - 2 == kingX && oldY + 1 == kingY)) //left down -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY + 2][oldX - 1] == KingBlack && (oldX - 1 == kingX && oldY + 2 == kingY)) //down left -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY + 2][oldX + 1] == KingBlack && (oldX + 1 == kingX && oldY + 2 == kingY)) //down right -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY - 1][oldX + 2] == KingBlack && (oldX + 2 == kingX && oldY - 1 == kingY)) //right down -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY + 1][oldX + 2] == KingBlack && (oldX + 2 == kingX && oldY + 1 == kingY)) //right up -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	return 0;
}
int KnightCheckB(int oldX, int oldY, int kingX, int kingY)
{
	if (board[oldY - 2][oldX + 1] == KingWhite && (oldX + 1 == kingX && oldY - 2 == kingY)) //up right -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY - 2][oldX - 1] == KingWhite && (oldX - 1 == kingX && oldY - 2 == kingY)) //up left -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY - 1][oldX - 2] == KingWhite && (oldX - 2 == kingX && oldY - 1 == kingY)) //left up -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY + 1][oldX - 2] == KingWhite && (oldX - 2 == kingX && oldY + 1 == kingY)) //left down -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY + 2][oldX - 1] == KingWhite && (oldX - 1 == kingX && oldY + 2 == kingY)) //down left -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY + 2][oldX + 1] == KingWhite && (oldX + 1 == kingX && oldY + 2 == kingY)) //down right -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY - 1][oldX + 2] == KingWhite && (oldX + 2 == kingX && oldY - 1 == kingY)) //right down -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	if (board[oldY + 1][oldX + 2] == KingWhite && (oldX + 2 == kingX && oldY + 1 == kingY)) //right up -- Allow move if square is empty or enemy 
	{
		return 1;
	}
	return 0;
}
int BishopCheckW(int oldX, int oldY, int kingX, int kingY)
{
	int i = oldX - 1; //Diagonal up left
	int j = oldY - 1;
	while (i >= 0 && j >= 0 && board[j][i] != 7)
	{
		if (board[j][i] == KingBlack && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}

		i--;
		j--;
	}

	i = oldX - 1;//Diagonal down left
	j = oldY + 1;
	while (i >= 0 && j <= 11 && board[j][i] != 7)
	{
		if (board[j][i] == KingBlack && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}

		i--;
		j++;
	}

	i = oldX + 1;//Diagonal down right
	j = oldY + 1;
	while (i <= 11 && j <= 11 && board[j][i] != 7)
	{
		if (board[j][i] == KingBlack && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}
		i++;
		j++;
	}

	i = oldX + 1;//Diagonal up right
	j = oldY - 1;
	while (i <= 11 && j >= 0 && board[j][i] != 7)
	{
		if (board[j][i] == KingBlack && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}
		i++;
		j--;
	}
	return 0;
}
int BishopCheckB(int oldX, int oldY, int kingX, int kingY)
{
	int i = oldX - 1; //Diagonal up left
	int j = oldY - 1;
	while (i >= 0 && j >= 0 && board[j][i] != 7)
	{
		if (board[j][i] == KingWhite && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}

		i--;
		j--;
	}

	i = oldX - 1;//Diagonal down left
	j = oldY + 1;
	while (i >= 0 && j <= 11 && board[j][i] != 7)
	{
		if (board[j][i] == KingWhite && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}

		i--;
		j++;
	}

	i = oldX + 1;//Diagonal down right
	j = oldY + 1;
	while (i <= 11 && j <= 11 && board[j][i] != 7)
	{
		if (board[j][i] == KingWhite && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}
		i++;
		j++;
	}

	i = oldX + 1;//Diagonal up right
	j = oldY - 1;
	while (i <= 11 && j >= 0 && board[j][i] != 7)
	{
		if (board[j][i] == KingWhite && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}
		i++;
		j--;
	}
	return 0;
}
int QueenCheckW(int oldX, int oldY, int kingX, int kingY)
{
	int i = oldX - 1; //Diagonal up left
	int j = oldY - 1;
	while (i >= 0 && j >= 0)
	{
		if (board[j][i] == KingBlack && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}

		i--;
		j--;
	}

	i = oldX - 1;//Diagonal down left
	j = oldY + 1;
	while (i >= 0 && j <= 11)
	{
		if (board[j][i] == KingBlack && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}

		i--;
		j++;
	}

	i = oldX + 1;//Diagonal down right
	j = oldY + 1;
	while (i <= 11 && j <= 11)
	{
		if (board[j][i] == KingBlack && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}
		i++;
		j++;
	}

	i = oldX + 1;//Diagonal up right
	j = oldY - 1;
	while (i <= 11 && j >= 0)
	{
		if (board[j][i] == KingBlack && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}
		i++;
		j--;
	}


	for (int i = oldX - 1; i >= 0; i--) //Loop for rook moves to the left
	{
		if (board[oldY][i] == KingBlack && (i == kingX && oldY == kingY))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldX + 1; i <= 11; i++) //Loop for rook moves to the right
	{
		if (board[oldY][i] == KingBlack && (kingX == i && kingY == oldY))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldY - 1; i >= 0; i--) //Loop for rook moves up
	{
		if (board[i][oldX] == KingBlack && (kingY == i && oldX == kingX))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[i][oldX] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldY + 1; i <= 11; i++) //Loop for rook moves down
	{
		if (board[i][oldX] == KingBlack && (kingY == i && oldX == kingX))
		{
			//Allow the piece to move if squares in between are empty or if there is an enemy piece where we want to move
			return 1;
		}
		else if (board[i][oldX] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	return 0;
}
int QueenCheckB(int oldX, int oldY, int kingX, int kingY)
{
	int i = oldX - 1; //Diagonal up left
	int j = oldY - 1;
	while (i >= 0 && j >= 0)
	{
		if (board[j][i] == KingWhite && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}

		i--;
		j--;
	}

	i = oldX - 1;//Diagonal down left
	j = oldY + 1;
	while (i >= 0 && j <= 11)
	{
		if (board[j][i] == KingWhite && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}

		i--;
		j++;
	}

	i = oldX + 1;//Diagonal down right
	j = oldY + 1;
	while (i <= 11 && j <= 11)
	{
		if (board[j][i] == KingWhite && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}
		i++;
		j++;
	}

	i = oldX + 1;//Diagonal up right
	j = oldY - 1;
	while (i <= 11 && j >= 0)
	{
		if (board[j][i] == KingWhite && (i == kingX && j == kingY))
		{
			return 1;
		}
		else if (board[j][i] != 0)
		{
			break;
		}
		i++;
		j--;
	}


	for (int i = oldX - 1; i >= 0; i--) //Loop for rook moves to the left
	{
		if (board[oldY][i] == KingWhite && (i == kingX && oldY == kingY))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldX + 1; i <= 11; i++) //Loop for rook moves to the right
	{
		if (board[oldY][i] == KingWhite && (kingX == i && kingY == oldY))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldY - 1; i >= 0; i--) //Loop for rook moves up
	{
		if (board[i][oldX] == KingWhite && (kingY == i && oldX == kingX))
		{
			//Check if there is a king in the rooks path 
			return 1;
		}
		else if (board[i][oldX] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	for (int i = oldY + 1; i <= 11; i++) //Loop for rook moves down
	{
		if (board[i][oldX] == KingWhite && (kingY == i && oldX == kingX))
		{
			//Allow the piece to move if squares in between are empty or if there is an enemy piece where we want to move
			return 1;
		}
		else if (board[i][oldX] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}

	return 0;
}

int IsKingInCheckW(int kingX, int kingY)
{
	//Loop through every square on the board
	for (int i = 2; i < 10; i++)
	{
		for (int j = 2; j < 10; j++)
		{
			if (board[j][i] == PawnBlack && PawnCheckB(i, j, kingX, kingY) == 1)
			{
				return 1;
			}
			if (board[j][i] == RookBlack && RookCheckB(i, j, kingX, kingY) == 1)
			 {
			 	return 1;
			}
			if (board[j][i] == KnightBlack && KnightCheckB(i, j, kingX, kingY) == 1)
			{
				return 1;
			}
			if (board[j][i] == BishopBlack && BishopCheckB(i, j, kingX, kingY) == 1)
			{
				return 1;
			}
			if (board[j][i] == QueenBlack && QueenCheckB(i, j, kingX, kingY) == 1)
			{
				return 1;
			}
		}
	}
	return 0;
}
int IsKingInCheckB(int kingX, int kingY)
{
	//Loop through every square on the board
	for (int i = 2; i < 10; i++)
	{
		for (int j = 2; j < 10; j++)
		{
			if (board[j][i] == PawnWhite && PawnCheckW(i, j, kingX, kingY) == 1)
			{
				return 1;
			}
			 if (board[j][i] == RookWhite && RookCheckW(i, j, kingX, kingY) == 1)
			{
			 	return 1;
			}
			if (board[j][i] == KnightWhite && KnightCheckW(i, j, kingX, kingY) == 1)
			{
				return 1;
			}
			if (board[j][i] == BishopWhite && BishopCheckW(i, j, kingX, kingY) == 1)
			{
				return 1;
			}
			if (board[j][i] == QueenWhite && QueenCheckW(i, j, kingX, kingY) == 1)
			{
				return 1;
			}
		}
	}
	return 0;
}

string PawnMovesW(int oldX, int oldY)
{ 
	
	string allMoves;
	kingPosW();
	CopyingToTempBoard();
	CopyingToMainBoard();
		if (oldY == 8 && board[oldY - 2][oldX] == 0 && board[oldY - 1][oldX] == 0)
		{
			board[oldY][oldX] = 0;
			board[oldY - 2][oldX] = PawnWhite;
			if (IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard(); //If the move is made on the main board and the white king is still in check, go back to the old position stored in the temp board
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY - 2) + " ");
				CopyingToMainBoard(); //If the move is made on the main board and it is legal add the move to the move list, go back to the old position stored in the temp board to continue checking posible moves
			}
		}
		if (board[oldY - 1][oldX] == 0)
		{
			if(oldY == 3)
			{
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX] = KnightWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY - 1) + "=N ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX] = BishopWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY - 1) + "=B ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX] = RookWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY - 1) + "=R ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX] = QueenWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY - 1) + "=Q ");
					CopyingToMainBoard();
				}
			}
			else if(oldY != 3)
			{
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX] = PawnWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY - 1) + " ");
					CopyingToMainBoard();
				}
			}
		}
		if (board[oldY - 1][oldX - 1] > 0 && board[oldY - 1][oldX - 1] <= 5)
		{
			if(oldY == 3)
			{
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX-1] = KnightWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX - 1, oldY - 1) + "=N ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX-1] = BishopWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX - 1, oldY - 1) + "=B ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX-1] = RookWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX - 1, oldY - 1) + "=R ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX-1] = QueenWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX - 1, oldY - 1) + "=Q ");
					CopyingToMainBoard();
				}
			}
			else if(oldY != 3)
			{
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX-1] = PawnWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX - 1, oldY - 1) + " ");
					CopyingToMainBoard();
				}
			}
		}
		if (board[oldY - 1][oldX + 1] > 0 && board[oldY - 1][oldX + 1] <= 5)
		{
			if(oldY == 3)
			{
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX+1] = KnightWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX + 1, oldY - 1) + "=N ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX+1] = BishopWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX + 1, oldY - 1) + "=B ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX+1] = RookWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX + 1, oldY - 1) + "=R ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX+1] = QueenWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX + 1, oldY - 1) + "=Q ");
					CopyingToMainBoard();
				}
			}
			else if(oldY != 3)
			{
				board[oldY][oldX] = 0;
				board[oldY - 1][oldX + 1] = PawnWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX + 1, oldY - 1) + " ");
					CopyingToMainBoard();
				}
			}
		}
	return allMoves;
}
string PawnMovesB(int oldX, int oldY)
{ 
	//Implement en passant
	//Maybe Use variable tracking last pawn move and allow en passant if its the right pawn
	//Maybe set the tempboard to the previous position
	//use that to check if last move was correct pawn move for en passant
	string allMoves;
	kingPosB();
	CopyingToTempBoard();
	CopyingToMainBoard();
		if (oldY == 3 && board[oldY + 2][oldX] == 0 && board[oldY + 1][oldX] == 0)
		{
			board[oldY][oldX] = 0;
			board[oldY + 2][oldX] = PawnBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard(); //If the move is made on the main board and the white king is still in check, go back to the old position stored in the temp board
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY + 2) + " ");
				CopyingToMainBoard(); //If the move is made on the main board and it is legal add the move to the move list, go back to the old position stored in the temp board to continue checking posible moves
			}
		}
		if (board[oldY + 1][oldX] == 0)
		{
			if(oldY == 8)
			{
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX] = KnightBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY + 1) + "=N ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX] = BishopBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY + 1) + "=B ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX] = RookBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY + 1) + "=R ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX] = QueenBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY + 1) + "=Q ");
					CopyingToMainBoard();
				}
			}
			else if(oldY != 8)
			{
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX] = PawnBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY + 1) + " ");
					CopyingToMainBoard();
				}
			}
		}
		if (board[oldY + 1][oldX - 1] < 0 && board[oldY + 1][oldX - 1] >= -5)
		{
			if(oldY==8)
			{
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX-1] = KnightBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY + 1) + "=N ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX-1] = BishopBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY + 1) + "=B ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX-1] = RookBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY + 1) + "=R ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX-1] = QueenBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY + 1) + "=Q ");
					CopyingToMainBoard();
				}				
			}
			else if(oldY != 8)
			{
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX - 1] = PawnBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX - 1, oldY + 1) + " ");
					CopyingToMainBoard();
				}
			}
		}
		if (board[oldY + 1][oldX + 1] < 0 && board[oldY + 1][oldX + 1] >= -5)
		{
			if(oldY == 8)
			{
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX+1] = KnightBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY + 1) + "=N ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX+1] = BishopBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY + 1) + "=B ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX+1] = RookBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY + 1) + "=R ");
					CopyingToMainBoard();
				}
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX+1] = QueenBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY + 1) + "=Q ");
					CopyingToMainBoard();
				}
			}
			else if(oldY != 8)
			{
				board[oldY][oldX] = 0;
				board[oldY + 1][oldX + 1] = PawnBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX + 1, oldY + 1) + " ");
					CopyingToMainBoard();
				}
			}
		}
	return allMoves;
}
string RookMovesW(int oldX, int oldY)
{
	string allMoves;
	kingPosW();
	CopyingToTempBoard();
	CopyingToMainBoard();
		for (int i = oldX - 1; i >= 0; i--) //Loop for rook moves to the left
		{
			if (board[oldY][i] >= 0 && board[oldY][i] <= 5)
			{
				board[oldY][oldX] = 0;
				board[oldY][i] = RookWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, oldY) + " ");
					CopyingToMainBoard();
					if(board[oldY][i] != 0)
					{break;}
				}
			}
			else if (board[oldY][i] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}
		for (int i = oldX + 1; i <= 11; i++) //Loop for rook moves to the right
		{
			if (board[oldY][i] >= 0 && board[oldY][i] <= 5)
			{
				board[oldY][oldX] = 0;
				board[oldY][i] = RookWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, oldY) + " ");
					CopyingToMainBoard();
					if(board[oldY][i] != 0)
					{break;}
				}
			}
			else if (board[oldY][i] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}

		for (int i = oldY - 1; i >= 0; i--) //Loop for rook moves up
		{
			if (board[i][oldX] >= 0 && board[i][oldX] <= 5)
			{
				board[oldY][oldX] = 0;
				board[i][oldX] = RookWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, i) + " ");
					CopyingToMainBoard();
					if(board[i][oldX] != 0)
					{break;}
				}
			}
			else if (board[i][oldX] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}

		for (int i = oldY + 1; i <= 11; i++) //Loop for rook moves down
		{
			if (board[i][oldX] >= 0 && board[i][oldX] <= 5)
			{
				board[oldY][oldX] = 0;
				board[i][oldX] = RookWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, i) + " ");
					CopyingToMainBoard();
					if(board[i][oldX] != 0)
					{break;}
				}
			}
			else if (board[i][oldX] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}
	return allMoves;
}
string RookMovesB(int oldX, int oldY)
{
	string allMoves;
	kingPosB();
	CopyingToTempBoard();
	CopyingToMainBoard();
		for (int i = oldX - 1; i >= 0; i--) //Loop for rook moves to the left
		{
			if (board[oldY][i] <= 0 && board[oldY][i] >= -5)
			{
				board[oldY][oldX] = 0;
				board[oldY][i] = RookBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, oldY) + " ");
					CopyingToMainBoard();
					if(board[oldY][i] != 0)
					{break;}
				}
			}
			else if (board[oldY][i] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}
		for (int i = oldX + 1; i <= 11; i++) //Loop for rook moves to the right
		{
			if (board[oldY][i] <= 0 && board[oldY][i] >= -5)
			{
				board[oldY][oldX] = 0;
				board[oldY][i] = RookBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, oldY) + " ");
					CopyingToMainBoard();
					if(board[oldY][i] != 0)
					{break;}
				}
			}
			else if (board[oldY][i] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}

		for (int i = oldY - 1; i >= 0; i--) //Loop for rook moves up
		{
			if (board[i][oldX] <= 0 && board[i][oldX] >= -5)
			{
				board[oldY][oldX] = 0;
				board[i][oldX] = RookBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, i) + " ");
					CopyingToMainBoard();
					if(board[i][oldX] != 0)
					{break;}
				}
			}
			else if (board[i][oldX] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}

		for (int i = oldY + 1; i <= 11; i++) //Loop for rook moves down
		{
			if (board[i][oldX] <= 0 && board[i][oldX] >= -5)
			{
				board[oldY][oldX] = 0;
				board[i][oldX] = RookBlack;
				if (IsKingInCheckB(kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, i) + " ");
					CopyingToMainBoard();
					if(board[i][oldX] != 0)
					{break;}
				}
			}
			else if (board[i][oldX] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}
	return allMoves;
}
string KnightMovesW(int oldX, int oldY) 
{
	string allMoves;
	kingPosW();
	CopyingToTempBoard();
	CopyingToMainBoard();
	if (board[oldY-2][oldX+1] >= 0 && board[oldY-2][oldX+1] <= 5) //up right -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY-2][oldX+1] = KnightWhite;
		if(IsKingInCheckW(kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY-2) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY - 2][oldX - 1] >= 0 && board[oldY-2][oldX-1] <= 5) //up left -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY-2][oldX-1] = KnightWhite;
		if(IsKingInCheckW(kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY-2) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY - 1][oldX - 2] >= 0 && board[oldY - 1][oldX - 2] <= 5) //left up -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY-1][oldX-2] = KnightWhite;
		if(IsKingInCheckW(kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-2, oldY-1) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY + 1][oldX - 2] >= 0 && board[oldY + 1][oldX - 2] <= 5) //left down -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY+1][oldX-2] = KnightWhite;
		if(IsKingInCheckW(kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();	
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-2, oldY+1) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY + 2][oldX - 1] >= 0 && board[oldY + 2][oldX - 1] <= 5) //down left -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY+2][oldX-1] = KnightWhite;
		if(IsKingInCheckW(kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();	
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY+2) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY + 2][oldX + 1] >= 0 && board[oldY + 2][oldX + 1] <= 5) //down right -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY+2][oldX+1] = KnightWhite;
		if(IsKingInCheckW(kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();	
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY+2) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY - 1][oldX + 2] >= 0 && board[oldY - 1][oldX + 2] <= 5) //right down -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY-1][oldX+2] = KnightWhite;
		if(IsKingInCheckW(kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();	
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+2, oldY-1) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY + 1][oldX + 2] >= 0 && board[oldY + 1][oldX + 2] <= 5) //right up -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY+1][oldX+2] = KnightWhite;
		if(IsKingInCheckW(kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();	
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+2, oldY+1) + " ");
			CopyingToMainBoard();
		}
	}
	return allMoves;
}
string KnightMovesB(int oldX, int oldY) 
{
	string allMoves;
	kingPosB();
	CopyingToTempBoard();
	CopyingToMainBoard();
	if (board[oldY - 2][oldX + 1] <= 0 && board[oldY - 2][oldX + 1] >= -5) //up right -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY-2][oldX+1] = KnightBlack;
		if(IsKingInCheckB(kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY-2) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY - 2][oldX - 1] <= 0 && board[oldY - 2][oldX - 1] >= -5) //up left -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY-2][oldX-1] = KnightBlack;
		if(IsKingInCheckB(kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY-2) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY - 1][oldX - 2] <= 0 && board[oldY - 1][oldX - 2] >= -5) //left up -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY-1][oldX-2] = KnightBlack;
		if(IsKingInCheckB(kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-2, oldY-1) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY + 1][oldX - 2] <= 0 && board[oldY + 1][oldX - 2] >= -5) //left down -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY+1][oldX-2] = KnightBlack;
		if(IsKingInCheckB(kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();	
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-2, oldY+1) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY + 2][oldX - 1] <= 0 && board[oldY + 2][oldX - 1] >= -5) //down left -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY+2][oldX-1] = KnightBlack;
		if(IsKingInCheckB(kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();	
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY+2) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY + 2][oldX + 1] <= 0 && board[oldY + 2][oldX + 1] >= -5) //down right -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY+2][oldX+1] = KnightBlack;
		if(IsKingInCheckB(kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();	
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY+2) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY - 1][oldX + 2] <= 0 && board[oldY - 1][oldX + 2] >= -5) //right down -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY-1][oldX+2] = KnightBlack;
		if(IsKingInCheckB(kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();	
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+2, oldY-1) + " ");
			CopyingToMainBoard();
		}
	}
	if (board[oldY + 1][oldX + 2] <= 0 && board[oldY + 1][oldX + 2] >= -5) //right up -- Allow move if square is empty or enemy 
	{
		board[oldY][oldX] = 0;
		board[oldY+1][oldX+2] = KnightBlack;
		if(IsKingInCheckB(kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();	
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+2, oldY+1) + " ");
			CopyingToMainBoard();
		}
	}
	return allMoves;
}
string BishopMovesW(int oldX, int oldY)
{
	string allMoves;
	kingPosW();
	CopyingToTempBoard();
	CopyingToMainBoard();

	int i = oldX - 1; //Diagonal up left
	int j = oldY - 1;
	while (i>=0 && j>=0)
	{
		if (board[j][i] >= 0  && board[j][i] <= 5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = BishopWhite;
			if (IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i--;
		j--;
	}

	i = oldX - 1;//Diagonal down left
	j = oldY + 1;
	while (i >= 0 && j <= 9)
	{
		if (board[j][i] >= 0  && board[j][i] <= 5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = BishopWhite;
			if (IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i--;
		j++;
	}

	i = oldX + 1;//Diagonal down right
	j = oldY + 1;
	while (i <= 9 && j <= 9)
	{
		if (board[j][i] >= 0  && board[j][i] <= 5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = BishopWhite;
			if (IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i++;
		j++;
	}

	i = oldX + 1;//Diagonal up right
	j = oldY - 1;
	while (i <= 9 && j >= 0)
	{
		if (board[j][i] >= 0  && board[j][i] <= 5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = BishopWhite;
			if (IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i++;
		j--;
	}
	return allMoves;
}
string BishopMovesB(int oldX, int oldY)
{
	string allMoves;
	kingPosB();
	CopyingToTempBoard();
	CopyingToMainBoard();

	int i = oldX - 1; //Diagonal up left
	int j = oldY - 1;
	while (i>=0 && j>=0)
	{
		if (board[j][i] <= 0  && board[j][i] >= -5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = BishopBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i--;
		j--;
	}

	i = oldX - 1;//Diagonal down left
	j = oldY + 1;
	while (i >= 0 && j <= 9)
	{
		if (board[j][i] <= 0  && board[j][i] >= -5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = BishopBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i--;
		j++;
	}

	i = oldX + 1;//Diagonal down right
	j = oldY + 1;
	while (i <= 9 && j <= 9)
	{
		if (board[j][i] <= 0  && board[j][i] >= -5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = BishopBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i++;
		j++;
	}

	i = oldX + 1;//Diagonal up right
	j = oldY - 1;
	while (i <= 9 && j >= 0)
	{
		if (board[j][i] <= 0  && board[j][i] >= -5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = BishopBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i++;
		j--;
	}
	return allMoves;

}
string QueenMovesW(int oldX, int oldY)
{
	string allMoves;
	kingPosW();
	CopyingToTempBoard();
	CopyingToMainBoard();
	for (int i = oldX - 1; i >= 0; i--) //Loop for rook moves to the left
	{
		if (board[oldY][i] >= 0 && board[oldY][i] <= 5)
		{
			board[oldY][oldX] = 0;
			board[oldY][i] = QueenWhite;
			if (IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, oldY) + " ");
				CopyingToMainBoard();
				if(board[oldY][i] != 0)
				{break;}
			}
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}
	for (int i = oldX + 1; i <= 11; i++) //Loop for rook moves to the right
		{
			if (board[oldY][i] >= 0 && board[oldY][i] <= 5)
			{
				board[oldY][oldX] = 0;
				board[oldY][i] = QueenWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, oldY) + " ");
					CopyingToMainBoard();
					if(board[oldY][i] != 0)
					{break;}
				}
			}
			else if (board[oldY][i] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}
	for (int i = oldY - 1; i >= 0; i--) //Loop for rook moves up
		{
			if (board[i][oldX] >= 0 && board[i][oldX] <= 5)
			{
				board[oldY][oldX] = 0;
				board[i][oldX] = QueenWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, i) + " ");
					CopyingToMainBoard();
					if(board[i][oldX] != 0)
					{break;}
				}
			}
			else if (board[i][oldX] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}
	for (int i = oldY + 1; i <= 11; i++) //Loop for rook moves down
		{
			if (board[i][oldX] >= 0 && board[i][oldX] <= 5)
			{
				board[oldY][oldX] = 0;
				board[i][oldX] = QueenWhite;
				if (IsKingInCheckW(kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, i) + " ");
					CopyingToMainBoard();
					if(board[i][oldX] != 0)
					{break;}
				}
			}
			else if (board[i][oldX] != 0)
			{
				//Stop checking if there is a piece in the way of the rooks path
				break;
			}
		}
		
	int i = oldX - 1; //Diagonal up left
	int j = oldY - 1;
	while (i>=0 && j>=0)
	{
		if (board[j][i] >= 0  && board[j][i] <= 5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = QueenWhite;
			if (IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i--;
		j--;
	}

	i = oldX - 1;//Diagonal down left
	j = oldY + 1;
	while (i >= 0 && j <= 9)
	{
		if (board[j][i] >= 0  && board[j][i] <= 5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = QueenWhite;
			if (IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i--;
		j++;
	}

	i = oldX + 1;//Diagonal down right
	j = oldY + 1;
	while (i <= 9 && j <= 9)
	{
		if (board[j][i] >= 0  && board[j][i] <= 5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = QueenWhite;
			if (IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i++;
		j++;
	}

	i = oldX + 1;//Diagonal up right
	j = oldY - 1;
	while (i <= 9 && j >= 0)
	{
		if (board[j][i] >= 0  && board[j][i] <= 5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = QueenWhite;
			if (IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i++;
		j--;
	}
	
	return allMoves;
}
string QueenMovesB(int oldX, int oldY)
{
	string allMoves;
	kingPosB();
	CopyingToTempBoard();
	CopyingToMainBoard();

	for (int i = oldX - 1; i >= 0; i--) //Loop for rook moves to the left
	{
		if (board[oldY][i] <= 0 && board[oldY][i] >= -5)
		{
			board[oldY][oldX] = 0;
			board[oldY][i] = QueenBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, oldY) + " ");
				CopyingToMainBoard();
				if(board[oldY][i] != 0)
				{break;}
			}
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}
	for (int i = oldX + 1; i <= 11; i++) //Loop for rook moves to the right
	{
		if (board[oldY][i] <= 0 && board[oldY][i] >= -5)
		{
			board[oldY][oldX] = 0;
			board[oldY][i] = QueenBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, oldY) + " ");
				CopyingToMainBoard();
				if(board[oldY][i] != 0)
				{break;}
			}
		}
		else if (board[oldY][i] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}
	for (int i = oldY - 1; i >= 0; i--) //Loop for rook moves up
	{
		if (board[i][oldX] <= 0 && board[i][oldX] >= -5)
		{
			board[oldY][oldX] = 0;
			board[i][oldX] = QueenBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, i) + " ");
				CopyingToMainBoard();
				if(board[i][oldX] != 0)
				{break;}
			}
		}
		else if (board[i][oldX] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}
	for (int i = oldY + 1; i <= 11; i++) //Loop for rook moves down
	{
		if (board[i][oldX] <= 0 && board[i][oldX] >= -5)
		{
			board[oldY][oldX] = 0;
			board[i][oldX] = QueenBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, i) + " ");
				CopyingToMainBoard();
				if(board[i][oldX] != 0)
				{break;}
			}
		}
		else if (board[i][oldX] != 0)
		{
			//Stop checking if there is a piece in the way of the rooks path
			break;
		}
	}
	
	int i = oldX - 1; //Diagonal up left
	int j = oldY - 1;
	while (i>=0 && j>=0)
	{
		if (board[j][i] <= 0  && board[j][i] >= -5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = QueenBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i--;
		j--;
	}

	i = oldX - 1;//Diagonal down left
	j = oldY + 1;
	while (i >= 0 && j <= 9)
	{
		if (board[j][i] <= 0  && board[j][i] >= -5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = QueenBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i--;
		j++;
	}

	i = oldX + 1;//Diagonal down right
	j = oldY + 1;
	while (i <= 9 && j <= 9)
	{
		if (board[j][i] <= 0  && board[j][i] >= -5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = QueenBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i++;
		j++;
	}

	i = oldX + 1;//Diagonal up right
	j = oldY - 1;
	while (i <= 9 && j >= 0)
	{
		if (board[j][i] <= 0  && board[j][i] >= -5)
		{
			board[oldY][oldX] = 0;
			board[j][i] = QueenBlack;
			if (IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(i, j) + " ");
				CopyingToMainBoard();
				if(board[j][i] != 0)
				{break;}
			}
		}
		else if (board[j][i] != 0)
		{break;}

		i++;
		j--;
	}
	
	return allMoves;
}
string KingMovesW(int oldX, int oldY)
{
	string allMoves;
	kingPosW();
	kingPosB();
	CopyingToTempBoard();
	CopyingToMainBoard();
	//Castling 
	if(castleRights[0][1] == 1) //White kingside
	{
		if(IsKingInCheckW(kingW.x, kingW.y) == 0 && kingW.x==6 && kingW.y==9 && board[9][7] == 0 && board[9][8] == 0 && board[9][9] == RookWhite)
		{
			board[oldY][oldX] = 0;
			board[oldY][oldX+1] = KingWhite;
			kingPosW();
			kingPosB();
			if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				CopyingToMainBoard();
				board[oldY][oldX] = 0;
				board[oldY][oldX+2] = KingWhite;
				kingPosW();
				kingPosB();
				if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+2, oldY) + " ");
					CopyingToMainBoard();
				}
			}
		}
	}
	kingPosW();
	kingPosB();
	if(castleRights[0][0] == 1) //White Queenside
	{
		if(IsKingInCheckW(kingW.x, kingW.y) == 0 && kingW.x==6 && kingW.y==9 && board[9][5] == 0 && board[9][4] == 0 && board[9][3] == 0 && board[9][2] == RookWhite)
		{
			board[oldY][oldX] = 0;
			board[oldY][oldX - 1] = KingWhite;
			kingPosW();
			kingPosB();
			if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				CopyingToMainBoard();
				board[oldY][oldX] = 0;
				board[oldY][oldX-2] = KingWhite;
				kingPosW();
				kingPosB();
				if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-2, oldY) + " ");
					CopyingToMainBoard();
				}
			}
		}
	}
	if(board[oldY-1][oldX] >= 0 && board[oldY-1][oldX] <= 5) //up
	{
		board[oldY][oldX] = 0;
		board[oldY-1][oldX] = KingWhite;
		kingPosW();
		kingPosB();
		if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY-1) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY-1][oldX-1] >= 0 && board[oldY-1][oldX-1] <= 5) //up left
	{
		board[oldY][oldX] = 0;
		board[oldY-1][oldX-1] = KingWhite;
		kingPosW();
		kingPosB();
		if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY-1) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY][oldX-1] >= 0 && board[oldY][oldX-1] <= 5) //left
	{
		board[oldY][oldX] = 0;
		board[oldY][oldX-1] = KingWhite;
		kingPosW();
		kingPosB();
		if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY+1][oldX-1] >= 0 && board[oldY+1][oldX-1] <= 5) //down left
	{
		board[oldY][oldX] = 0;
		board[oldY+1][oldX-1] = KingWhite;
		kingPosW();
		kingPosB();
		if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY+1) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY+1][oldX] >= 0 && board[oldY+1][oldX] <= 5) //down
	{
		board[oldY][oldX] = 0;
		board[oldY+1][oldX] = KingWhite;
		kingPosW();
		kingPosB();
		if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY+1) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY+1][oldX+1] >= 0 && board[oldY+1][oldX+1] <= 5) //down right
	{
		board[oldY][oldX] = 0;
		board[oldY+1][oldX+1] = KingWhite;
		kingPosW();
		kingPosB();
		if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY+1) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY][oldX+1] >= 0 && board[oldY][oldX+1] <= 5) //right
	{
		board[oldY][oldX] = 0;
		board[oldY][oldX+1] = KingWhite;
		kingPosW();
		kingPosB();
		if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY-1][oldX+1] >= 0 && board[oldY-1][oldX+1] <= 5) //up right
	{
		board[oldY][oldX] = 0;
		board[oldY-1][oldX+1] = KingWhite;
		kingPosW();
		kingPosB();
		if(IsKingInCheckW(kingW.x, kingW.y) == 1 || IsOpposingKingNearby(kingW.x, kingW.y, kingB.x, kingB.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY-1) + " ");
			CopyingToMainBoard();
		}
	}
	return allMoves;
}
string KingMovesB(int oldX, int oldY)
{
	string allMoves;
	kingPosW();
	kingPosB();
	CopyingToTempBoard();
	CopyingToMainBoard();

//Castling 
	if(castleRights[1][1] == 1) //Black kingside
	{
		if(IsKingInCheckB(kingB.x, kingB.y) == 0 && kingB.x==6 && kingB.y==2 && board[2][7] == 0 && board[2][8] == 0 && board[2][9] == RookBlack)
		{
			board[oldY][oldX] = 0;
			board[oldY][oldX+1] = KingBlack;
			kingPosW();
			kingPosB();
			if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				CopyingToMainBoard();
				board[oldY][oldX] = 0;
				board[oldY][oldX+2] = KingBlack;
				kingPosW();
				kingPosB();
				if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+2, oldY) + " ");
					CopyingToMainBoard();
				}
			}
		}
	}
	kingPosW();
	kingPosB();
	if(castleRights[1][0] == 1) //White Queenside
	{
		if(IsKingInCheckB(kingB.x, kingB.y) == 0 && kingB.x==6 && kingB.y==2 && board[2][5] == 0 && board[2][4] == 0 && board[2][3] == 0 && board[2][2] == RookBlack)
		{
			board[oldY][oldX] = 0;
			board[oldY][oldX - 1] = KingBlack;
			kingPosW();
			kingPosB();
			if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				CopyingToMainBoard();
				board[oldY][oldX] = 0;
				board[oldY][oldX-2] = KingBlack;
				kingPosW();
				kingPosB();
				if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
				{
					CopyingToMainBoard();
				}
				else
				{
					allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-2, oldY) + " ");
					CopyingToMainBoard();
				}
			}
		}
	}
	
	if(board[oldY-1][oldX] <= 0 && board[oldY-1][oldX] >= -5) //up
	{
		board[oldY][oldX] = 0;
		board[oldY-1][oldX] = KingBlack;
		kingPosW();
		kingPosB();
		if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY-1) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY-1][oldX-1] <= 0 && board[oldY-1][oldX-1] >= -5) //up left
	{
		board[oldY][oldX] = 0;
		board[oldY-1][oldX-1] = KingBlack;
		kingPosW();
		kingPosB();
		if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY-1) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY][oldX-1] <= 0 && board[oldY][oldX-1] >= -5) //left
	{
		board[oldY][oldX] = 0;
		board[oldY][oldX-1] = KingBlack;
		kingPosW();
		kingPosB();
		if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY+1][oldX-1] <= 0 && board[oldY+1][oldX-1] >= -5) //down left
	{
		board[oldY][oldX] = 0;
		board[oldY+1][oldX-1] = KingBlack;
		kingPosW();
		kingPosB();
		if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX-1, oldY+1) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY+1][oldX] <= 0 && board[oldY+1][oldX] >= -5) //down
	{
		board[oldY][oldX] = 0;
		board[oldY+1][oldX] = KingBlack;
		kingPosW();
		kingPosB();
		if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX, oldY+1) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY+1][oldX+1] <= 0 && board[oldY+1][oldX+1] >= -5) //down right
	{
		board[oldY][oldX] = 0;
		board[oldY+1][oldX+1] = KingBlack;
		kingPosW();
		kingPosB();
		if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY+1) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY][oldX+1] <= 0 && board[oldY][oldX+1] >= -5) //right
	{
		board[oldY][oldX] = 0;
		board[oldY][oldX+1] = KingBlack;
		kingPosW();
		kingPosB();
		if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY) + " ");
			CopyingToMainBoard();
		}
	}
	if(board[oldY-1][oldX+1] <= 0 && board[oldY-1][oldX+1] >= -5) //up right
	{
		board[oldY][oldX] = 0;
		board[oldY-1][oldX+1] = KingBlack;
		kingPosW();
		kingPosB();
		if(IsKingInCheckB(kingB.x, kingB.y) == 1 || IsOpposingKingNearby(kingB.x, kingB.y, kingW.x, kingW.y) == 1)
		{
			CopyingToMainBoard();
		}
		else
		{
			allMoves += (convertIndexToSqr(oldX, oldY) + convertIndexToSqr(oldX+1, oldY-1) + " ");
			CopyingToMainBoard();
		}
	}
	return allMoves;
}

string AllMovesW(string FEN)
{
	FENtoBoard(FEN);
	kingPosW();
	kingPosB();
	string allMoves = " ";
	CopyingToTempBoard();
	CopyingToMainBoard();

	if(validSquare == 1 && turn == 0)
	{
		if(board[targetSquarey+1][targetSquarex+1] == PawnWhite)
		{
			board[targetSquarey+1][targetSquarex+1] = 0;
			board[targetSquarey][targetSquarex] = PawnWhite;
			board[targetSquarey+1][targetSquarex] = 0;
			if(IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves+=(convertIndexToSqr(targetSquarex+1, targetSquarey+1) + convertIndexToSqr(targetSquarex, targetSquarey) + " ");
				CopyingToMainBoard();
			}
		}
		if(board[targetSquarey+1][targetSquarex-1] == PawnWhite)
		{
			board[targetSquarey+1][targetSquarex-1] = 0;
			board[targetSquarey][targetSquarex] = PawnWhite;
			board[targetSquarey+1][targetSquarex] = 0;
			if(IsKingInCheckW(kingW.x, kingW.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves+=(convertIndexToSqr(targetSquarex-1, targetSquarey+1) + convertIndexToSqr(targetSquarex, targetSquarey) + " ");
				CopyingToMainBoard();
			}
		}
	}

	for(int i = 2; i < 10; i++)
	{
		for(int j = 2; j < 10; j++)
		{
			if(board[j][i] == PawnWhite)
			{
				allMoves+= PawnMovesW(i, j); 
			}
			if(board[j][i] == RookWhite)
			{
				allMoves+=RookMovesW(i, j);
			}
			if(board[j][i] == KnightWhite)
			{
				allMoves+=KnightMovesW(i, j);
			}
			if(board[j][i] == BishopWhite)
			{
				allMoves+=BishopMovesW(i, j);
			}
			if(board[j][i] == QueenWhite)
			{
				allMoves+= QueenMovesW(i, j);
			}
			if(board[j][i] == KingWhite)
			{
				allMoves+= KingMovesW(i, j);
			}
		}
	}
	return RemoveSpacesFromAllMoves(allMoves);
}
string AllMovesB(string FEN)
{
	FENtoBoard(FEN);
	kingPosW();
	kingPosB();
	string allMoves = " ";
	CopyingToTempBoard();
	CopyingToMainBoard();

	if(validSquare == 1 && turn == 1)
	{
		if(board[targetSquarey-1][targetSquarex+1] == PawnBlack)
		{
			board[targetSquarey-1][targetSquarex+1] = 0;
			board[targetSquarey][targetSquarex] = PawnBlack;
			board[targetSquarey-1][targetSquarex] = 0;
			if(IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves+=(convertIndexToSqr(targetSquarex+1, targetSquarey-1) + convertIndexToSqr(targetSquarex, targetSquarey) + " ");
				CopyingToMainBoard();
			}
		}
		if(board[targetSquarey-1][targetSquarex-1] == PawnBlack)
		{
			board[targetSquarey-1][targetSquarex-1] = 0;
			board[targetSquarey][targetSquarex] = PawnBlack;
			board[targetSquarey-1][targetSquarex] = 0;
			if(IsKingInCheckB(kingB.x, kingB.y) == 1)
			{
				CopyingToMainBoard();
			}
			else
			{
				allMoves+=(convertIndexToSqr(targetSquarex-1, targetSquarey-1) + convertIndexToSqr(targetSquarex, targetSquarey) + " ");
				CopyingToMainBoard();
			}
		}
	}



	for(int i = 2; i < 10; i++)
	{
		for(int j = 2; j < 10; j++)
		{
			if(board[j][i] == PawnBlack)
			{
				allMoves+= PawnMovesB(i, j); 
			}
			if(board[j][i] == RookBlack)
			{
				allMoves+=RookMovesB(i, j);
			}
			if(board[j][i] == KnightBlack)
			{
				allMoves+=KnightMovesB(i, j);
			}
			if(board[j][i] == BishopBlack)
			{
				allMoves+=BishopMovesB(i, j);
			}
			if(board[j][i] == QueenBlack)
			{
				allMoves+= QueenMovesB(i, j);
			}
			if(board[j][i] == KingBlack)
			{
				allMoves+= KingMovesB(i, j);
			}
		}
	}
	return RemoveSpacesFromAllMoves(allMoves);
}
string convertToIndexString(const string& movesStr) 
{
  	string indexMoves;

    for (size_t i = 0; i < movesStr.length(); i += 5) 
		{ // Each normal move takes 5 characters including the space

        char startCol = movesStr[i];
        char startRow = movesStr[i + 1];
        char endCol = movesStr[i + 2];
        char endRow = movesStr[i + 3];
				// Convert columns from letters to numbers (0-7)
				int startIndexX = 2+startCol - 'a';
				int endIndexX = 2+endCol - 'a';

				// Convert rows from numbers to 0-based indexes (0-7)
				int startIndexY = 2+'8' - startRow;
				int endIndexY = 2+'8' - endRow;

				// Append converted indexes to the string
				indexMoves += to_string(startIndexX) + to_string(startIndexY) + to_string(endIndexX) + to_string(endIndexY);
				if((movesStr[i+4]) == ' ') //Normal moves
				{
					//Nothing
				}
				else if((movesStr[i+4]) != ' ') //For promotions
				{
					if(movesStr[i+5] == 'N')
					{indexMoves += 'N';}
					else if(movesStr[i+5] == 'B')
					{indexMoves += 'B';}
					else if(movesStr[i+5] == 'R')
					{indexMoves += 'R';}
					else if(movesStr[i+5] == 'Q')
					{indexMoves += 'Q';}
					i+=2;
				}
				indexMoves+=" ";
    }

    return indexMoves;
}

mt19937 createSeededGenerator() 
{
    // Using time as a seed for random number generator
    unsigned seed = static_cast<unsigned>(time(nullptr));
    return mt19937(seed);
}
string ChooseRandomMove(string AllMoves)
{
	int spaceCount = 0;
	string randomMove = "";

	static mt19937 generator = createSeededGenerator();
  uniform_int_distribution<> range(0, TotalMoves - 1);

	if(TotalMoves != 0)
	{
	int random = range(generator);

	for(int i=0; i<AllMoves.length(); i++)
		{

			if(spaceCount == 0 && random == 0)
			{
				randomMove += AllMoves[0];
				randomMove += AllMoves[1];
				randomMove += AllMoves[2];
				randomMove += AllMoves[3];
				if(AllMoves[4] == ' ')
				{
					break;
				}
				else
				{
					randomMove += AllMoves[4];
					randomMove += AllMoves[5];
					break;
				}
			}
			if(AllMoves[i] == ' ')
			{
				spaceCount++;
				if(spaceCount == random)
				{
					randomMove += AllMoves[i+1];
					randomMove += AllMoves[i+2];
					randomMove += AllMoves[i+3];
					randomMove += AllMoves[i+4];
					if(AllMoves[i+5] == ' ')
					{
						break;
					}
					else
					{
						randomMove += AllMoves[i+5];
						randomMove += AllMoves[i+6];
						break;
					}
				}
			}
		}
	}
	return randomMove;
}
string BoardtoFEN()
{
	string FEN = "";
	 int row = 2;
	 int letter = 2;
	 int spacingCount = 0;
	 while(true)
	 {
	 	if(row == 9 && letter == 10)
	 	{
			if(spacingCount!=0)
			{
				FEN+=to_string(spacingCount);
			}
	 		break;
	 	}
		if(spacingCount == 8)
		{
			FEN+=to_string(spacingCount);
			spacingCount = 0;
		}
	 	if(letter == 10)
	 	{
	 		letter = 2;
			if(spacingCount != 0)
			{
				FEN+=to_string(spacingCount);
				spacingCount = 0;
			}
			if(row<9)
			{FEN+='/';}
	 		row++;
	 	}
	 	if(board[row][letter] == 0)
	 	{
	 		spacingCount++;
	 		letter++;
	 		continue;
	 	}
	 	if(board[row][letter] != 0)
	 	{
	 		if(spacingCount!=0)
	 		{
	 		FEN+=to_string(spacingCount);
			spacingCount=0;
	 		}

	 		if(board[row][letter] == PawnBlack)
	 		{
	 			FEN+='p';
	 		}
	 		if(board[row][letter] == RookBlack)
	 		{
	 			FEN+='r';
	 		}
	 		if(board[row][letter] == KnightBlack)
	 		{
	 			FEN+='n';
	 		}
	 		if(board[row][letter] == BishopBlack)
	 		{
	 			FEN+='b';
	 		}
	 		if(board[row][letter] == QueenBlack)
	 		{
	 			FEN+='q';
	 		}
	 		if(board[row][letter] == KingBlack)
	 		{
	 			FEN+='k';
	 		}
	 		if(board[row][letter] == PawnWhite)
	 		{
	 			FEN+='P';
	 		}
	 		if(board[row][letter] == RookWhite)
	 		{
	 			FEN+='R';
	 		}
	 		if(board[row][letter] == KnightWhite)
	 		{
	 			FEN+='N';
	 		}
	 		if(board[row][letter] == BishopWhite)
	 		{
	 			FEN+='B';
	 		}
	 		if(board[row][letter] == QueenWhite)
	 		{
	 			FEN+='Q';
	 		}
	 		if(board[row][letter] == KingWhite)
	 		{
	 			FEN+='K';
	 		}
	 	}
	 	letter++;
	 }

	 FEN+=' ';
	 if(turn == 0)
	 {
		FEN+='w';
	 }
	 else if(turn == 1)
	 {
		FEN+='b';
	 }

	 FEN+=' ';
	 if(castleRights[0][1] == 1)
	 {
		FEN+='K';	
	 }
	 if(castleRights[0][0] == 1)
	 {
		FEN+='Q';
	 }
	 if(castleRights[1][1] == 1)
	 {
		FEN+='k';	
	 }
	 if(castleRights[1][0] == 1)
	 {
		FEN+='q';
	 }
	 if(castleRights[0][1] == 0 && castleRights[0][0] == 0 && castleRights[1][1] == 0 && castleRights[1][0] == 0)
	 {
		FEN+='-';
	 }
	 FEN+=' ';
	 if(validSquare == 1)
	 {
		FEN+=convertIndexToSqr(targetSquarex, targetSquarey);
	 }
	 else if(validSquare == 0)
	 {
		FEN+='-';
	 }

	FEN+=' ';
	FEN+=to_string(countFiftyMoveRule);
	FEN+=' ';
	FEN+=to_string(countMoves);

	return FEN;
}
string tempBoardtoFEN()
{
	string FEN = "";
	 int row = 2;
	 int letter = 2;
	 int spacingCount = 0;
	 while(true)
	 {
	 	if(row == 9 && letter == 10)
	 	{
			if(spacingCount!=0)
			{
				FEN+=to_string(spacingCount);
			}
	 		break;
	 	}
		if(spacingCount == 8)
		{
			FEN+=to_string(spacingCount);
			spacingCount = 0;
		}
	 	if(letter == 10)
	 	{
	 		letter = 2;
			if(spacingCount != 0)
			{
				FEN+=to_string(spacingCount);
				spacingCount = 0;
			}
			if(row<9)
			{FEN+='/';}
	 		row++;
	 	}
	 	if(tempBoard[row][letter] == 0)
	 	{
	 		spacingCount++;
	 		letter++;
	 		continue;
	 	}
	 	if(tempBoard[row][letter] != 0)
	 	{
	 		if(spacingCount!=0)
	 		{
	 		FEN+=to_string(spacingCount);
			spacingCount=0;
	 		}

	 		if(tempBoard[row][letter] == PawnBlack)
	 		{
	 			FEN+='p';
	 		}
	 		if(tempBoard[row][letter] == RookBlack)
	 		{
	 			FEN+='r';
	 		}
	 		if(tempBoard[row][letter] == KnightBlack)
	 		{
	 			FEN+='n';
	 		}
	 		if(tempBoard[row][letter] == BishopBlack)
	 		{
	 			FEN+='b';
	 		}
	 		if(tempBoard[row][letter] == QueenBlack)
	 		{
	 			FEN+='q';
	 		}
	 		if(tempBoard[row][letter] == KingBlack)
	 		{
	 			FEN+='k';
	 		}
	 		if(tempBoard[row][letter] == PawnWhite)
	 		{
	 			FEN+='P';
	 		}
	 		if(tempBoard[row][letter] == RookWhite)
	 		{
	 			FEN+='R';
	 		}
	 		if(tempBoard[row][letter] == KnightWhite)
	 		{
	 			FEN+='N';
	 		}
	 		if(tempBoard[row][letter] == BishopWhite)
	 		{
	 			FEN+='B';
	 		}
	 		if(tempBoard[row][letter] == QueenWhite)
	 		{
	 			FEN+='Q';
	 		}
	 		if(tempBoard[row][letter] == KingWhite)
	 		{
	 			FEN+='K';
	 		}
	 	}
	 	letter++;
	 }

	 FEN+=' ';
	 if(turn == 0)
	 {
		FEN+='w';
	 }
	 else if(turn == 1)
	 {
		FEN+='b';
	 }

	 FEN+=' ';
	 if(castleRights[0][1] == 1)
	 {
		FEN+='K';	
	 }
	 if(castleRights[0][0] == 1)
	 {
		FEN+='Q';
	 }
	 if(castleRights[1][1] == 1)
	 {
		FEN+='k';	
	 }
	 if(castleRights[1][0] == 1)
	 {
		FEN+='q';
	 }
	 if(castleRights[0][1] == 0 && castleRights[0][0] == 0 && castleRights[1][1] == 0 && castleRights[1][0] == 0)
	 {
		FEN+='-';
	 }
	 FEN+=' ';
	 if(validSquare == 1)
	 {
		FEN+=convertIndexToSqr(targetSquarex, targetSquarey);
	 }
	 else if(validSquare == 0)
	 {
		FEN+='-';
	 }

	FEN+=' ';
	FEN+=to_string(countFiftyMoveRule);
	FEN+=' ';
	FEN+=to_string(countMoves);

	return FEN;
}
void UpdateCastleRights()
{
	
	if(hasWhiteKingMoved == 1 || hasRightWhiteRookMoved == 1 || board[9][9] != -2 || board[9][6] != -6)
	{
		castleRights[0][1] = 0;
	}
	if(hasWhiteKingMoved == 1 || hasLeftWhiteRookMoved == 1 || board[9][2] != -2 || board[9][6] != -6)
	{
		castleRights[0][0] = 0;
	}
	if(hasBlackKingMoved == 1 || hasRightBlackRookMoved == 1 || board[2][9] != 2 || board[2][6] != 6)
	{
		castleRights[1][1] = 0;
	}
	if(hasBlackKingMoved == 1 || hasLeftBlackRookMoved == 1 || board[2][2] != 2 || board[2][6] != 6)
	{
		castleRights[1][0] = 0;
	}
}
void resetAllValuesAfterGameEnd()
{
		kingPosW(); 
		kingPosB();
		UpdateCastleRights();
		countMoves = 1, countFiftyMoveRule = 0, hasBlackKingMoved = 0, hasWhiteKingMoved = 0, hasLeftBlackRookMoved = 0, 
		hasRightBlackRookMoved = 0, hasLeftWhiteRookMoved = 0, hasRightWhiteRookMoved = 0, 
		targetSquarex = 0, targetSquarey = 0, validSquare = 0;	
}
int IsMatingMaterial(string FEN)
{
	FENtoBoard(FEN);
	int isPawnW = 0, isRookW = 0, isKnightW = 0, isBishopW = 0, isQueenW = 0; 
	int isPawnB = 0, isRookB = 0, isKnightB = 0, isBishopB = 0, isQueenB = 0; 
	for(int i = 2; i<10; i++)
	{
		for(int j = 2; j<10; j++)
		{
			if(board[j][i] == PawnWhite)
			{
				isPawnW++;
			}
			if(board[j][i] == RookWhite)
			{
				isRookW++;
			}
			if(board[j][i] == KnightWhite)
			{
				isKnightW++;
			}
			if(board[j][i] == BishopWhite)
			{
				isBishopW++;
			}
			if(board[j][i] == QueenWhite)
			{
				isQueenW++;
			}
			if(board[j][i] == PawnBlack)
			{
				isPawnB++;
			}
			if(board[j][i] == RookBlack)
			{
				isRookB++;
			}
			if(board[j][i] == KnightBlack)
			{
				isKnightB++;
			}
			if(board[j][i] == BishopBlack)
			{
				isBishopB++;
			}
			if(board[j][i] == QueenBlack)
			{
				isQueenW++;
			}
		}
	}


	if(isKnightW == 2 && isKnightB == 0 && isPawnW == 0 && isRookW == 0 && isQueenW == 0 && isPawnB == 0 && isRookB == 0 && isQueenB == 0)
	{
		return 0;
	}


	if(isKnightB == 2 && isKnightW == 0 && isPawnW == 0 && isRookW == 0 && isQueenW == 0 && isPawnB == 0 && isRookB == 0 && isQueenB == 0)
	{
		return 0;
	}
	

	if(isPawnW == 0 && isRookW == 0 && isQueenW == 0 && isPawnB == 0 && isRookB == 0 && isQueenB == 0)
	{
		if(isBishopW == 1 && isKnightW == 0 && isKnightB <= 1 && isBishopB <= 1)
		{
			if(isKnightB == 1 && isBishopB == 1)
			{
				return 1;
			}
			return 0;
		}
		if(isBishopW == 0 && isKnightW <= 2 && isKnightB <= 1 && isBishopB <= 1)
		{
			if((isKnightW == 0 || isKnightW == 1) && isKnightB == 1 && isBishopB == 1)
			{
				return 1;
			}
			else if(isKnightW == 2 && (isKnightB != 0 || isBishopB != 0))
			{
				return 1;
			}
			return 0;
		} 
	
		if(isBishopB == 1 && isKnightB == 0 && isKnightW <= 1 && isBishopW <= 1)
		{
			if(isKnightW == 1 && isBishopW == 1)
			{
				return 1;
			}
			return 0;
		}
		if(isBishopB == 0 && isKnightB <= 2 && isKnightW <= 1 && isBishopW <= 1)
		{
			if((isKnightB == 0 || isKnightB == 1) && isKnightW == 1 && isBishopW == 1)
			{
				return 1;
			}
			else if(isKnightB == 2 && (isKnightW != 0 || isBishopW != 0))
			{
				return 1;
			}
			return 0;
		}
	}

	return 1;
}
void makeMove(string move)
{
	CopyingToTempBoard();
	CopyingToMainBoard();
	kingPosW(); 
	kingPosB();
	UpdateCastleRights();
	resetFiftyMoveRule = 0;
	validSquare = 0;
	totalMovesplayed++;

	if(IsMatingMaterial(BoardtoFEN()) == 0)
	{
		resetAllValuesAfterGameEnd();
		draw++;
		cout<<"Draw";
	}
	if(countFiftyMoveRule == 50)
	{
		draw++;
		resetAllValuesAfterGameEnd();
		cout<<"Draw";
	}
		char first = move[0] - 47;
		char second = move[1];
		char third = move[2] - 47;
		char fourth = move[3];
		int x = first-48;
		int y = 58-second;
		int xx = third-48;
		int yy = 58-fourth;

		if(board[y][x] == PawnWhite || board[y][x] == PawnBlack || board[yy][xx] != 0)
		{
			resetFiftyMoveRule = 1;
		}
		countFiftyMoveRule++;
		if(resetFiftyMoveRule == 1)
		{
			countFiftyMoveRule = 0;
		}
		
		if(move.length() == 4)
		{
			if(board[y][x] == KingBlack && y == 2 && x == 6 && hasBlackKingMoved == 0)
			{
				hasBlackKingMoved = 1;
			}
			if(board[y][x] == KingWhite && y == 9 && x == 6 && hasWhiteKingMoved == 0)
			{
				hasWhiteKingMoved = 1;
			}
			if(board[y][x] == RookBlack && y == 2 && x == 2 && hasLeftBlackRookMoved == 0) //Black 'A' rook
			{
				hasLeftBlackRookMoved = 1;
			}
			if(board[y][x] == RookBlack && y == 2 && x == 9 && hasRightBlackRookMoved == 0) //Black 'H' rook 
			{
				hasRightBlackRookMoved = 1;
			}
			if(board[y][x] == RookWhite && y == 9 && x == 2 && hasLeftWhiteRookMoved == 0) //White 'A' rook
			{
				hasLeftWhiteRookMoved = 1;
			}
			if(board[y][x] == RookWhite && y == 9 && x == 9 && hasRightWhiteRookMoved == 0) //White 'H' rook
			{
				hasRightWhiteRookMoved = 1;
			}

			if(board[y][x] == KingWhite && y == 9 && x == 6 && yy == 9 && xx == 8 && castleRights[0][1] == 1 && board[9][9] == RookWhite)
			{
				board[9][9] = 0;
				board[9][7] = RookWhite;
			}
			if(board[y][x] == KingWhite && y == 9 && x == 6 && yy == 9 && xx == 4 && castleRights[0][0] == 1 && board[9][2] == RookWhite)
			{
				board[9][2] = 0;
				board[9][5] = RookWhite;
			}
			if(board[y][x] == KingBlack && y == 2 && x == 6 && yy == 2 && xx == 8 && castleRights[1][1] == 1 && board[2][9] == RookBlack)
			{
				board[2][9] = 0;
				board[2][7] = RookBlack;
			}
			if(board[y][x] == KingBlack && y == 2 && x == 6 && yy == 2 && xx == 4 && castleRights[1][0] == 1 && board[2][2] == RookBlack)
			{
				board[2][2] = 0;
				board[2][5] = RookBlack;
			}
			
			if((board[y][x] == PawnWhite || board[y][x] == PawnBlack) && x != xx && board[yy][xx] == 0)
			{
				if(board[y][x] == PawnWhite)
				{
					board[yy+1][xx] = 0;
				}
				else if(board[y][x] == PawnBlack)
				{
					board[yy-1][xx] = 0;
				}
			}

			if(board[y][x] == PawnBlack && y + 2 == yy && (board[yy][xx+1] == PawnWhite || board[yy][xx-1] == PawnWhite))
			{
				if(board[yy][xx+1] == PawnWhite)
				{
					board[yy][xx] = 0;
					board[y][x] = 0;
					board[yy][xx+1] = 0;
					board[yy-1][xx] = PawnWhite;

					if (IsKingInCheckW(kingW.x, kingW.y) == 1)
					{
						CopyingToMainBoard();
					}
					else
					{
						validSquare = 1;
						targetSquarey = y + 1;
						targetSquarex = x;
						CopyingToMainBoard();
					}
				}


				if(board[yy][xx-1] == PawnWhite)
				{
					board[yy][xx] = 0;
					board[y][x] = 0;
					board[yy][xx-1] = 0;
					board[yy-1][xx] = PawnWhite;
					if (IsKingInCheckW(kingW.x, kingW.y) == 1)
					{
						CopyingToMainBoard();
					}
					else
					{
						validSquare = 1;
						targetSquarey = y + 1;
						targetSquarex = x;
						CopyingToMainBoard();
					}
				}
			}
			if(board[y][x] == PawnWhite && y - 2 == yy && (board[yy][xx+1] == PawnBlack || board[yy][xx-1] == PawnBlack))
			{
				if(board[yy][xx+1] == PawnBlack)
				{
					board[yy][xx] = 0;
					board[y][x] = 0;
					board[yy][xx+1] = 0;
					board[yy+1][xx] = PawnBlack;

					if (IsKingInCheckB(kingB.x, kingB.y) == 1)
					{
						CopyingToMainBoard();
					}
					else
					{
						validSquare = 1;
						targetSquarey = y - 1;
						targetSquarex = x;
						CopyingToMainBoard();
					}
				}

				if(board[yy][xx-1] == PawnBlack)
				{
					board[yy][xx] = 0;
					board[y][x] = 0;
					board[yy][xx-1] = 0;
					board[yy+1][xx] = PawnBlack;
				
					if (IsKingInCheckB(kingB.x, kingB.y) == 1)
					{
						CopyingToMainBoard();
					}
					else
					{
						validSquare = 1;
						targetSquarey = y - 1;
						targetSquarex = x;
						CopyingToMainBoard();
					}
				}
			}

		board[yy][xx] = board[y][x];
		board[y][x] = 0;

		}
		else if(move.length() == 6)
		{
			char fifth = move[5];

			if(fifth == 'Q' && turn == 0)
			{
			board[yy][xx] = QueenBlack;
			board[y][x] = 0;
			}
			if(fifth == 'R' && turn == 0)
			{
			board[yy][xx] = RookBlack;
			board[y][x] = 0;
			}
			if(fifth == 'N' && turn == 0)
			{
			board[yy][xx] = KnightBlack;
			board[y][x] = 0;
			}
			if(fifth == 'B' && turn == 0)
			{
			board[yy][xx] = BishopBlack;
			board[y][x] = 0;
			}
			if(fifth == 'Q' && turn == 1)
			{
			board[yy][xx] = QueenWhite;
			board[y][x] = 0;
			}
			if(fifth == 'R' && turn == 1)
			{
			board[yy][xx] = RookWhite;
			board[y][x] = 0;
			}
			if(fifth == 'N' && turn == 1)
			{
			board[yy][xx] = KnightWhite;
			board[y][x] = 0;
			}
			if(fifth == 'B' && turn == 1)
			{
			board[yy][xx] = BishopWhite;
			board[y][x] = 0;
			}
		}
		if(turn == 0)
		{
			turn = 1;
		}
		else
		{
			turn = 0;
		}
	UpdateCastleRights();
}
vector<string> parseMoves(const string& movesString) {
    vector<string> moves;
    string move;
    size_t start = 0;
    size_t end = movesString.find(' ');

    while (end != string::npos) {
        move = movesString.substr(start, end - start);
        moves.push_back(move);
        start = end + 1;
        end = movesString.find(' ', start);
    }

    // Add the last move (after the last space)
    move = movesString.substr(start);
    moves.push_back(move);

    return moves;
}

string PlayGame(int numMoves, string FEN)
{
	
	kingPosW(); 
	kingPosB();
	UpdateCastleRights();
	FENtoBoard(FEN);
	string move = "";
	resetFiftyMoveRule = 0;
	validSquare = 0;
	totalMovesplayed++;
	cout<<FEN<<endl;

	if(IsMatingMaterial(FEN) == 0)
	{
		resetAllValuesAfterGameEnd();
		draw++;
		return BoardtoFEN();
	}
	if(countFiftyMoveRule == 50)
	{
		draw++;
		resetAllValuesAfterGameEnd();
		return BoardtoFEN();
	}
	if(countMoves == numMoves)
	{
		resetAllValuesAfterGameEnd();
		return BoardtoFEN();
	}
	
	if(turn == 0)
	{
		move = ChooseRandomMove(AllMovesW(FEN));
		turn = 1;
	}
	else if(turn == 1)
	{
		move = ChooseRandomMove(AllMovesB(FEN));
		turn = 0;
		countMoves++;
	}

	cout<<move<<">>> ";
	if(TotalMoves != 0)
	{
		makeMove(move);
	}
	
	if(move[0] != 0)
	{
		kingPosW(); 
		kingPosB();
		UpdateCastleRights();
		return PlayGame(numMoves, BoardtoFEN());
	}
	
	
	resetAllValuesAfterGameEnd();
	if(IsKingInCheckW(kingW.x, kingW.y) == 1)
	{
		blackWin++;
	}
	if(IsKingInCheckB(kingB.x, kingB.y) == 1)
	{
		whiteWin++;
	}
	if(IsKingInCheckB(kingB.x, kingB.y) == 0 && IsKingInCheckW(kingW.x, kingW.y) == 0)
	{
		draw++;
	}
	return "wow";
}
};

class Display 
{
private:
	const string IMAGE_PATH = "imgs/";
    
	sf::RenderWindow& window;
    vector<sf::Texture> textures;
	std::unordered_map<int, sf::Sprite> spriteMap; // Mapping piece type to sprite
    int size;
	sf::Vector2i selectedPos;
    int selectedPiece = 0;
	chess chessgame;
	bool isPieceSelected = false;
	bool isPromotionMove = false;
	uint8_t pieceToPromote = 0; //1=Queen 2=Rook 3=Bishop 4=Knight
	int xx = 0;
	int yy = 0;
	sf::Vector2f mousePosition;
public:
    Display(sf::RenderWindow& win, int tileSize) : window(win), size(tileSize) 
	{
        loadTextures();
        createSpriteMap();
    }

    void loadTextures() 
	{
        vector<string> textureFiles = 
		{
            "board.png", "PawnB.png", "PawnW.png", "RookB.png",
			"RookW.png", "KnightB.png", "KnightW.png", 
			"BishopB.png", "BishopW.png", "QueenB.png", "QueenW.png", "KingB.png",
			"KingW.png", "PromoteB.png", "PromoteW.png",
        };

        textures.resize(textureFiles.size());
        for (size_t i = 0; i < textureFiles.size(); i++) 
		{
            if (!textures[i].loadFromFile(IMAGE_PATH + textureFiles[i])) //Adding to textures here
			{
                cout<<"File "<<textureFiles[i]<<" not found."<<endl; 
            }
        }
    }
    void createSpriteMap() 
	{
		// Map for the board background
		spriteMap[0] = sf::Sprite(textures[0]);

		//Map for pieces
		spriteMap[1] = sf::Sprite(textures[1]);   // PawnBlack
		spriteMap[-1] = sf::Sprite(textures[2]);  // PawnWhite
		spriteMap[2] = sf::Sprite(textures[3]);   // RookBlack
		spriteMap[-2] = sf::Sprite(textures[4]);  // RookWhite
		spriteMap[3] = sf::Sprite(textures[5]);   // KnightBlack
		spriteMap[-3] = sf::Sprite(textures[6]);  // KnightWhite
		spriteMap[4] = sf::Sprite(textures[7]);   // BishopBlack
		spriteMap[-4] = sf::Sprite(textures[8]);  // BishopWhite
		spriteMap[5] = sf::Sprite(textures[9]);   // QueenBlack
		spriteMap[-5] = sf::Sprite(textures[10]); // QueenWhite
		spriteMap[6] = sf::Sprite(textures[11]);  // KingBlack
		spriteMap[-6] = sf::Sprite(textures[12]); // KingWhite
		
		spriteMap[7] = sf::Sprite(textures[13]); //Promotions White
		spriteMap[8] = sf::Sprite(textures[14]); //Promotions Black

    }
    void selectPiece(int x, int y, int board[12][12]) 
	{
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		dx = mouse.x - (x-2)*size;
		dy = mouse.y - (y-2)*size;

		if(isPromotionMove)
		{
			if( (mouse.x/TILE_SIZE)==(xx - 2) && (mouse.y/TILE_SIZE)==(yy - 2) )
			{
				if(dx <= 50 && dy <= 50)
				{
					pieceToPromote = 1;
				}
				else if(dx <= 50 && dy >= 50)
				{
					pieceToPromote = 2;
				}
				else if(dx >= 50 && dy <= 50)
				{
					pieceToPromote = 3;
				}
				else if(dx >= 50 && dy >= 50)
				{
					pieceToPromote = 4;
				}
			}
			else{pieceToPromote = -1;}
			return;
		}

		oX = x;
		oY = y;
		if (board[y][x] != 0 && spriteMap.find(board[y][x]) != spriteMap.end()) 
		{
			chessgame.CopyingToTempBoard();
			selectedPiece = board[y][x];
			selectedSprite = spriteMap[board[y][x]];
			board[y][x] = 0;
			moveSel = 1;
		}
	}
	void movePiece(int x, int y, int board[12][12]) 
	{
		string allIndexMoves;
		string move = to_string(oX)+to_string(oY)+to_string(x)+to_string(y);
		int allow = 0;
		string moveToMake = chessgame.convertIndexToSqr(oX,oY)+chessgame.convertIndexToSqr(x,y);

		if (isPromotionMove) 
		{
			char promotionChar = 'Q'; // Default to Queen
			switch (pieceToPromote) 
			{
				case 1: promotionChar = 'Q'; break; // Queen
				case 2: promotionChar = 'R'; break; // Rook
				case 3: promotionChar = 'B'; break; // Bishop
				case 4: promotionChar = 'N'; break; // Knight
				default: 
                	break;
			}
			if (pieceToPromote != -1) 
			{
				moveToMake += "=";
				moveToMake += promotionChar;
				chessgame.makeMove(moveToMake); // Apply the promotion move
			}

			// Resetting state after promotion
			pieceToPromote = 0;
			isPromotionMove = false;
			chessgame.CopyingToMainBoard();
			turn = 1 - turn;
			return;
    	}



		if(moveSel == 1)
		{
			chessgame.CopyingToMainBoard();
			if(turn == 0)
				{allIndexMoves = chessgame.convertToIndexString(chessgame.AllMovesW(chessgame.BoardtoFEN()));
					cout<<chessgame.AllMovesW(chessgame.BoardtoFEN())<<endl;
				}
			else
				{allIndexMoves = chessgame.convertToIndexString(chessgame.AllMovesB(chessgame.BoardtoFEN()));
					cout<<chessgame.AllMovesB(chessgame.BoardtoFEN())<<endl;
				}
		}
		cout<<allIndexMoves<<endl;
		
		for(int i = 0; i<allIndexMoves.size(); i+=5)
		{
			if(allIndexMoves[i] == move[0] && allIndexMoves[i+1] == move[1] && allIndexMoves[i+2] == move[2] && allIndexMoves[i+3] == move[3])
			{
				if(allIndexMoves[i+4] != ' ')
				{
					chessgame.CopyingToTempBoard();
					isPromotionMove = true;
					xx = x;
					yy = y;
					selectedPiece = 0;
					moveSel = 0;
					board[oX][oY] = 0;
					turn = 1-turn;
					return;
				}
				allow = 1;
				break;
			}
		}
		if (selectedPiece && allow == 1) 
		{
			chessgame.makeMove(moveToMake);
			selectedPiece = 0;
			moveSel = 0;
			allow = 0;
		}
		else if(selectedPiece && allow == 0)
		{
			board[oY][oX] = selectedPiece;
			selectedPiece = 0;
			moveSel = 0;
		}
	}
	void drawBoard(const int board[12][12]) 
	{
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		int x = 2+(pos.x / 100);
		int y = 2+(pos.y / 100);
		window.draw(spriteMap[0]); // Draw the board background

		if(isPromotionMove)
		{
			spriteMap[7+turn].setPosition( (xx-2)*size , (yy-2)*size );
			window.draw(spriteMap[7+turn]);
		}

		for (int i = 2; i < 10; i++) 
		{
			for (int j = 2; j < 10; j++) 
			{
				if(moveSel == 1)
				{
					selectedSprite.setPosition( pos.x-dx , pos.y-dy );
					window.draw(selectedSprite);
				}
				int piece = board[i][j];
				if (piece != 0 && spriteMap.find(piece) != spriteMap.end()) 
				{
					spriteMap[piece].setPosition( (j-2)*size , (i-2)*size );
					window.draw(spriteMap[piece]);
				}
			}
		}
	}
};

int main()
{
	chess chessgame;
	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "");
  	Display display(window, TILE_SIZE);

	string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	int count = 0;

	// while(true)
	// {
	// 	chessgame.PlayGame(20, FEN);
	// 	count++;
	// 	break;
	// 	if(whiteWin == 1 || blackWin == 1)
	// 	{
	// 		break;
	// 	}
	// }

	cout<<"Draws: "<<draw<<endl;
	cout<<"Black Wins: "<<blackWin<<endl;
	cout<<"White Wins: "<<whiteWin<<endl;

    while (window.isOpen()) {
        sf::Event e;
		sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x = 2+(pos.x / 100);
        int y = 2+(pos.y / 100);
        while (window.pollEvent(e)) 
		{
			// if(turn == 1) //Better way to do this?
			// {
			// 	chessgame.makeMove(chessgame.ChooseRandomMove(chessgame.AllMovesB(chessgame.BoardtoFEN())));
			// }
            if (e.type == sf::Event::Closed) 
			{
                window.close();
            }
            if (e.type == sf::Event::MouseButtonPressed) 
			{
                if (e.mouseButton.button == sf::Mouse::Left) 
				{
                    display.selectPiece(x, y, board);
                }
				//Add right click option or integrate mousebutton left into first if statement
            }
            if (e.type == sf::Event::MouseButtonReleased) 
			{
                if (e.mouseButton.button == sf::Mouse::Left) 
				{
                    display.movePiece(x, y, board);
                }
				//Add right click option or integrate mousebutton left into first if statement
            }
        }
        window.clear();
        display.drawBoard(board);
        window.display();
    }
    return 0;
}

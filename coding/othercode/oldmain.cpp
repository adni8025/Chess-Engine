#include <SFML/Graphics.hpp>
#include "iostream"
#include <string>
#include <random>
#include "ctime"
#include <cstdlib>

using namespace std;


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
#define Color 2 //Color to move
#define CastleSide 2 //Side to castle

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
int turn =0;
class Chess
{
private:
	struct pos
	{
		int x;
		int	y;
	} kingW, kingB;

	int countMoves = 1;

	int resetFiftyMoveRule = 1, countFiftyMoveRule = 0;

	int turn = 0; //0 is white to move -- 1 is black to move 

	int TotalMoves = 0;

	int castleRights[Color][CastleSide] = //Index Color 0 = white -- Index Color 1 = black -- Index CastleSide 0 = Queen Side -- Index CastleSide 1 = King side
	{
		0, 0, //1 = allowed to castle, 0= not allowed to castle 
		0, 0,
	};

	int hasBlackKingMoved = 0, hasWhiteKingMoved = 0, hasLeftBlackRookMoved = 0, 
	hasRightBlackRookMoved = 0, hasLeftWhiteRookMoved = 0, hasRightWhiteRookMoved = 0, 
	targetSquarex = 0, targetSquarey = 0, validSquare = 0; //Initalizing variables to check if rooks and kings have moved for castling and en passant squares

	int draw = 0, whiteWin = 0, blackWin = 0;
	int totalMovesplayed = 0;

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
	string ChooseRandomMove(string AllMoves)
	{
		int spaceCount = 0;
		string randomMove = "";

		random_device rd;
		mt19937 generator(rd()); 
		uniform_int_distribution<> range(0, TotalMoves-1);
		//srand((unsigned) time(NULL));
		if(TotalMoves != 0)
		{
		//int random = (rand() % TotalMoves);
		int random = range(generator);

		// for(int i=0; i<AllMoves.length(); i++)
		// 	{

		// 		if(AllMoves[i] == 'f' && AllMoves[i+1] == '7' && AllMoves[i+2] == 'f' && AllMoves[i+3] == '5' || (AllMoves[i+2] == 'f' && AllMoves[i+3] == '6'))
		// 		{
		// 			randomMove += AllMoves[i];
		// 			randomMove += AllMoves[i+1];
		// 			randomMove += AllMoves[i+2];
		// 			randomMove += AllMoves[i+3];
		// 			return randomMove;
		// 		}
		// 	}

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



int size = 100,x,y;

int main()
{
	Chess chessGame;
  int TotalMoves = 0;
	string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	//string FEN = "8/8/8/8/R4p1k/8/6P1/8 w - - 0 1";
	int numGames = 0;
	int count = 0;
	chessGame.PlayGame(1000, FEN);

    

// cout<<"Draws: "<<draw<<endl;
// cout<<"Black Wins: "<<blackWin<<endl;
// cout<<"White Wins: "<<whiteWin<<endl;


	sf::RenderWindow window(sf::VideoMode(800, 800), "");
	sf::Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;

	t1.loadFromFile("imgs/board.png");
	t2.loadFromFile("imgs/PawnW.png");
	t3.loadFromFile("imgs/PawnB.png");
  t4.loadFromFile("imgs/KnightW.png");
	t5.loadFromFile("imgs/KnightB.png");
	t6.loadFromFile("imgs/BishopW.png");
	t7.loadFromFile("imgs/BishopB.png");
  t8.loadFromFile("imgs/RookW.png");
	t9.loadFromFile("imgs/RookB.png");
	t10.loadFromFile("imgs/QueenW.png");
	t11.loadFromFile("imgs/QueenB.png");
	t12.loadFromFile("imgs/KingW.png");
	t13.loadFromFile("imgs/KingB.png");
	t14.loadFromFile("imgs/PromoteW.png");
	t15.loadFromFile("imgs/PromoteB.png");

	sf::Sprite Sboard(t1);
	sf::Sprite SPawnW(t2);
	sf::Sprite SPawnB(t3);
	sf::Sprite SKnightW(t4);
	sf::Sprite SKnightB(t5);
	sf::Sprite SBishopW(t6);
	sf::Sprite SBishopB(t7);
	sf::Sprite SRookW(t8);
	sf::Sprite SRookB(t9);
	sf::Sprite SQueenW(t10);
	sf::Sprite SQueenB(t11);
	sf::Sprite SKingW(t12);
	sf::Sprite SKingB(t13);
	sf::Sprite SPromoteW(t14);
	sf::Sprite SPromoteB(t15);
    sf::Sprite SMove;

	float dx = 0, dy = 0;
	int numPieceMoved = 0;
    

	while (window.isOpen())
	{
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		 x = (pos.x / size) + 2; // (0,0) top left of board
		 y = (pos.y / size) + 2;
		sf::Event e;

		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
			window.clear();
			if (e.type == sf::Event::MouseButtonPressed)
			{
				if (e.key.code == sf::Mouse::Left)
				{
					// std::cout << "x=" << x << " y=" << y << "\n";
					// std::cout << "pos_x=" << pos.x << " pos_y=" << pos.y << "\n";
					// std::cout << "board[y][x]=" << board[y][x] << "\n";
					// std::cout << "\n";
					if (board[y][x] != 0)
					{
						dx = pos.x - x * 100;
						dy = pos.y - y * 100;
						if (board[y][x] == PawnBlack && turn == 1)
						{
							numPieceMoved = PawnBlack;
							SMove = SPawnB;
							board[y][x] = 0;
						}
						if (board[y][x] == PawnWhite && turn == 0)
						{
							numPieceMoved = PawnWhite;
							SMove = SPawnW;
							board[y][x] = 0;
						}
						if (board[y][x] == RookBlack && turn == 1)
						{
							numPieceMoved = RookBlack;
							SMove = SRookB;
							board[y][x] = 0;
						}
						if (board[y][x] == RookWhite && turn == 0)
						{
							numPieceMoved = RookWhite;
							SMove = SRookW;
							board[y][x] = 0;
						}
						if (board[y][x] == KnightWhite && turn == 0)
						{
							numPieceMoved = KnightWhite;
							SMove = SKnightW;
							board[y][x] = 0;
						}
						if (board[y][x] == KnightBlack && turn == 1)
						{
							numPieceMoved = KnightBlack;
							SMove = SKnightB;
							board[y][x] = 0;
						}
						if (board[y][x] == BishopWhite && turn == 0)
						{
							numPieceMoved = BishopWhite;
							SMove = SBishopW;
							board[y][x] = 0;
						}
						if (board[y][x] == BishopBlack && turn == 1)
						{
							numPieceMoved = BishopBlack;
							SMove = SBishopB;
							board[y][x] = 0;
						}
						if (board[y][x] == QueenWhite && turn == 0)
						{
							numPieceMoved = QueenWhite;
							SMove = SQueenW;
							board[y][x] = 0;
						}
						if (board[y][x] == QueenBlack && turn == 1)
						{
							numPieceMoved = QueenBlack;
							SMove = SQueenB;
							board[y][x] = 0;
						}
						if (board[y][x] == KingWhite && turn == 0)
						{
							numPieceMoved = KingWhite;
							SMove = SKingW;
							board[y][x] = 0;
						}
						if (board[y][x] == KingBlack && turn == 1)
						{
							numPieceMoved = KingBlack;
							SMove = SKingB;
							board[y][x] = 0;
						}
            if (board[y][x] == 0)
						{
							//
						}
					}
				}
			}
		}
          
    window.clear();
		window.draw(Sboard);
    for (int i = 2; i < 10; i++)
	  {
			for (int j = 2; j < 10; j++)
			{
				if (board[i][j] != 0)
				{
					if (board[i][j] == PawnBlack)
					{
						SPawnB.setPosition((j-2) * size, (i-2) * size);
						window.draw(SPawnB);
					}
					if (board[i][j] == PawnWhite)
					{
						SPawnW.setPosition((j-2) * size, (i-2) * size);
						window.draw(SPawnW);
					}
					if (board[i][j] == RookBlack)
					{
						SRookB.setPosition((j-2) * size, (i-2) * size);
						window.draw(SRookB);
					}
					if (board[i][j] == RookWhite)
					{
						SRookW.setPosition((j-2) * size, (i-2) * size);
						window.draw(SRookW);
					}
					if (board[i][j] == KnightWhite)
					{
						SKnightW.setPosition((j-2) * size, (i-2) * size);
						window.draw(SKnightW);
					}
					if (board[i][j] == KnightBlack)
					{
						SKnightB.setPosition((j-2) * size, (i-2) * size);
						window.draw(SKnightB);
					}
					if (board[i][j] == BishopBlack)
					{
						SBishopB.setPosition((j-2) * size, (i-2) * size);
						window.draw(SBishopB);
					}
					if (board[i][j] == BishopWhite)
					{
						SBishopW.setPosition((j-2) * size, (i-2) * size);
						window.draw(SBishopW);
					}
					if (board[i][j] == QueenWhite)
					{
						SQueenW.setPosition((j-2) * size, (i-2) * size);
						window.draw(SQueenW);
					}
					if (board[i][j] == QueenBlack)
					{
						SQueenB.setPosition((j-2) * size, (i-2) * size);
						window.draw(SQueenB);
					}
					if (board[i][j] == KingBlack)
					{
						SKingB.setPosition((j-2) * size, (i-2) * size);
						window.draw(SKingB);
					}
					if (board[i][j] == KingWhite)
					{
						SKingW.setPosition((j-2) * size, (i-2) * size);
						window.draw(SKingW);
					}
				}
			}
		}
		window.display();
	}
	return 0;
}
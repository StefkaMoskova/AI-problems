//
//  main.cpp
//  Tic Tac Toe
//
//  Created by Stefka Vladimirova Moskova on 21.11.19.
//  Copyright © 2019 Stefka Vladimirova Moskova. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

class Game
{
public:
    enum class PlayerSymbol {emptyCell = '_', human = 'X', computer = 'O'};

    struct Move
    {
        int row = 0;
        int col = 0;
    };

    static const int size = 3;
    PlayerSymbol board[size][size];
    
    int emptyTiles;

    Game() : emptyTiles(size * size)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                board[i][j] = PlayerSymbol::emptyCell;
            }
        }
    }

    void play()
    {
        cout << "Please enter the first player (0 - you; 1-computer): " << endl;
        int turn;
        cin >> turn;
        
        bool isFinished = false;

        
        printBoard();
        
        cout << "Enter your symbol coorditanes (row,col):";

        do
        {
            // the move of the player - status
            if (turn == 0)
            {
                getPlayerMove();

                if (isWinning(PlayerSymbol::human))
                {
                    cout << "YOU WIN! GONGRATULATIONS!" << endl;
                    isFinished = true;
                }
            }
            
            else
            {
                cout << endl;
                cout << "Computer Move: ";

                Move computerMove = minimax();
                 
                cout << computerMove.row << " " << computerMove.col << endl;

                board[computerMove.row][computerMove.col] = PlayerSymbol::computer;
                emptyTiles--;

                if (isWinning(PlayerSymbol::computer))
                {
                    cout << "YOU LOSE! GAME OVER!" << endl;
                    isFinished = true;
                }
            }

            if (isEqualResult())
            {
                cout << endl;
                cout << "NO ONE WINS! GAME OVER!" << endl;
                
                isFinished = true;
            }

            turn ^= 1;
            printBoard();

        } while (!isFinished);
    }

private:
    bool isEqualResult()
      {
          return emptyTiles == 0;
      }
    
    bool isWinning(PlayerSymbol player)
    {
        // check the winnings of row or col
        for (int i = 0; i < size; i++)
        {
            bool isRowWinning = true;
            bool isColWinning = true;
            
            for (int j = 0; j < size; j++)
            {
                isRowWinning &= board[i][j] == player;
                isColWinning &= board[j][i] == player;
            }
            
            if (isColWinning || isRowWinning)
                return true;
        }

        // check for diagonal wins
        bool isDiagonalWinning = true;
        
        for (int i = 0; i < size; i++)
            isDiagonalWinning &= board[i][i] == player;

        if (isDiagonalWinning)
            return true;

        isDiagonalWinning = true;
        
        for (int i = 0; i < size; i++)
            isDiagonalWinning &= board[size - i - 1][i] == player;

        return isDiagonalWinning;
    }
    
    void getPlayerMove()
     {
         bool fail = true;
         int x = -1;
         int y = -1;

         do
         {
             char c;
             cin >> c;
             x = c - '0';
             cin >> c;
             y = c - '0';

             fail = board[x][y] != PlayerSymbol::emptyCell;

             cin.clear();
             cin.ignore(numeric_limits<streamsize>::max(), '\n');

         } while (fail);

         board[x][y] = PlayerSymbol::human;
         emptyTiles--;
     }
    
    void printBoard()
    {
        for (int i = 0; i < size; i++)
        {
            cout << endl;
            cout << "|";
            
            for (int j = 0; j < size; j++)
            {
                cout << setw(3) << static_cast<char>(board[i][j]) << setw(3) << " |";
            }
        }
        
        cout << endl;
        cout << endl;
    }
    
    // search for the minimum - beta
    int getBetaPruning(int level, int alpha, int beta)
    {
        int score = numeric_limits<int>::max();
        
        if (isWinning(PlayerSymbol::human))
            return 10 - level;
       
        else if (isWinning(PlayerSymbol::computer))
            return -10 + level;
        
        else if (isEqualResult())
            return 0;

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j] == PlayerSymbol::emptyCell)
                {
                    board[i][j] = PlayerSymbol::computer;
                    emptyTiles--;

                    score = min(score, getAlphaPruning(level + 1, alpha, beta) + level);
                    beta = min(beta, score);

                    board[i][j] = PlayerSymbol::emptyCell;
                    emptyTiles++;

                    if (beta <= alpha)
                        return beta;
                }
            }
        }

        return score;
    }

    // search for the maximum - alpha
    int getAlphaPruning(int level, int alpha, int beta)
    {
        int score = numeric_limits<int>::min();
        
        if (isWinning(PlayerSymbol::human))
            return 10 - level;
        
        else if (isWinning(PlayerSymbol::computer))
            return -10 + level;
        
        else if (isEqualResult())
            return 0;


        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j] == PlayerSymbol::emptyCell)
                {
                    board[i][j] = PlayerSymbol::human;
                    emptyTiles--;

                    score = max(score, getBetaPruning(level + 1, alpha, beta) - level);
                    alpha = max(alpha, score);

                    board[i][j] = PlayerSymbol::emptyCell;
                    emptyTiles++;

                    if (beta <= alpha)
                        return alpha;
                }
            }
        }

        return score;
    }
    
    Move minimax()
    {
        int score = numeric_limits<int>::max();
        
        Move move;
        int level = 0;

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j] == PlayerSymbol::emptyCell)
                {
                    board[i][j] = PlayerSymbol::computer;
                    emptyTiles--;

                    int MAX = getAlphaPruning(level, numeric_limits<int>::min(), numeric_limits<int>::max());

                    if (MAX < score)
                    {
                        score = MAX;
                        
                        move.row = i;
                        move.col = j;
                    }

                    board[i][j] = PlayerSymbol::emptyCell;
                    emptyTiles++;
                }
            }
        }

        return move;
    }
};

int main()
{
    cout << "***IMPLEMENTING TIC TAC TOE GAME***" << endl;
    cout << "TIC TAC TOE" << endl;
    cout <<  "SYMBOLS: PLAYER = X COMPUTER = O" << endl;

    Game tictactoe;
    tictactoe.play();
    
    cin.ignore();
    
    return 0;
}


/*
    =========================================================
    TIC-TAC-TOE PROJECT - main.cpp
    =========================================================
    Person 1 -> Board Structure & Display + Final Integration & Testing
    Person 2 -> Board Operations
    Person 3 -> Abstract Player class
    Person 4 -> Difficulty enum + AIPlayer (Easy)
    Person 5 -> AIPlayer Hard / Minimax
    Person 6 -> Game class setup & menu
    Person 7 -> Game flow
    =========================================================
*/

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
    =========================================================
    PERSON 4: Difficulty enum
    =========================================================
*/

/*
    =========================================================
    PERSON 1: Board class (Structure & Display) - COMPLETE
    =========================================================
*/
class Board
{
private:
    vector<vector<char>> grid;
    int size;

public:
    Board(int size = 3)
    {
        this->size = size;
        grid.assign(size, vector<char>(size, ' '));
    }

    // display
    void display() const
    {
        cout << "   ";
        for (int col = 0; col < size; col++)
        {
            cout << " " << (col + 1) << "  ";
        }
        cout << endl;

        for (int row = 0; row < size; row++)
        {
            cout << (row + 1) << "  ";

            for (int col = 0; col < size; col++)
            {
                cout << " " << grid[row][col] << " ";
                if (col < size - 1)
                    cout << "|";
            }
            cout << endl;

            if (row < size - 1)
            {
                cout << "   ";
                for (int col = 0; col < size; col++)
                {
                    cout << "---";
                    if (col < size - 1)
                        cout << "+";
                }
                cout << endl;
            }
        }
        cout << endl;
    }

    /*
        =========================================================
        PERSON 2: Board Operations
        =========================================================
    */

    // makeMove

    // isValidMove

    // checkWin

    // isFull

    // getCell

    // reset

    // getSize
};

/*
    =========================================================
    PERSON 3: Abstract Player class
    =========================================================
*/

// Player (constructor)

// getMove (pure virtual)

// getName

// getSymbol

// setName

/*
    =========================================================
    PERSON 4 & 5: AIPlayer class
    =========================================================
*/

// AIPlayer (constructor)

// getMove (override)

// setDifficulty

// getRandomMove -- Person 4

// getBestMove -- Person 5

// evaluateBoard -- Person 5

/*
    =========================================================
    PERSON 6 & 7: Game class
    =========================================================
*/

class Game
{
private:
    Board board;
    Player *currentPlayer;
    Player *player1;
    Player *player2;

public:
    Game(Player *p1, Player *p2)
    {
        player1 = p1;
        player2 = p2;
        currentPlayer = player1;
    }

    // start -- Person 6

    // showMenu -- Person 6

    // setupPvP -- Person 6

    // setupPvC -- Person 6

    // switchPlayer -- Person 7
    void switchPlayer() { currentPlayer = (currentPlayer == player1) ? player2 : player1; }

    // handleHumanMove -- Person 7
    void handleHumanMove()
    {
        int row, col;
        while (true)
        {
            cout << currentPlayer->getName() << " (" << currentPlayer->getSymbol()
                 << "), enter your move (row and column): ";
            if (!(cin >> row >> col))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input: please enter numbers only." << endl;
                continue;
            }
            row--;
            col--;
            cin.ignore(10000, '\n');
            if (row >= 0 && row < board.getSize() && col >= 0 && col < board.getSize() && board.isValidMove(row, col))
            {
                board.makeMove(row, col, currentPlayer->getSymbol());
                return;
            }
            cout << "Invalid move! Please try again." << endl;
        }
    }

    // handleAIMove -- Person 7
    void handleAIMove()
    {
        auto [r, c] = currentPlayer->getMove(board);
        board.makeMove(r, c, currentPlayer->getSymbol());
    }

    // checkGameEnd -- Person 7
    bool checkGameEnd() { return board.checkWin(currentPlayer->getSymbol()) || board.isFull(); }

    // displayResult -- Person 7
    void displayResult()
    {
        if (board.checkWin(player1->getSymbol()))
            cout << player1->getName() << " wins\n";
        else if (board.checkWin(player2->getSymbol()))
            cout << player2->getName() << " wins\n";
        else
            cout << "It's a draw\n";
    }

    // reset -- Person 7
    void reset()
    {
        board.reset();
        currentPlayer = player1;
    }
};

/*
    =========================================================
    MAIN FUNCTION
    =========================================================
*/
int main()
{
    Board board(3);
    board.display();

    return 0;
}
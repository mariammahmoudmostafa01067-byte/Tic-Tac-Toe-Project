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
enum Difficulty  { EASY,HARD };

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
bool checkWin(char symbol) const {
    for (int i = 0; i < size; ++i) {
        bool rowWin = true, colWin = true;
        for (int j = 0; j < size; ++j) {
            if (grid[i][j] != symbol) rowWin = false;
            if (grid[j][i] != symbol) colWin = false;
        }
        if (rowWin || colWin) return true;
    }
    bool diag1Win = true, diag2Win = true;
    for (int i = 0; i < size; ++i) {
        if (grid[i][i] != symbol) diag1Win = false;
        if (grid[i][size - 1 - i] != symbol) diag2Win = false;
    }
    return diag1Win || diag2Win;
}

    // isFull
bool isFull() const {
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (grid[r][c] == ' ') return false;
        }
    }
    return true;
}

//checkGameEnd
bool checkGameEnd() const {
    return checkWin('X') || checkWin('O') || isFull();
}

    // getCell

    // reset

    // getSize

    void undoMove(int row, int col){//helper method for minimax algorithm -Mostafa 
        grid[row][col] = ' ';
        return;
    }
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

class AIPlayer :public Player {
private:
    Difficulty difficulty;
public:

// AIPlayer (constructor)
    AIPlayer(const string& name, char symbol, Difficulty difficulty):
    Player(name,symbol)
    {
        this->difficulty = difficulty;
    }

// getMove (override)
    void getMove(const Board& board,int& row, int& col)override {
        if (difficulty == EASY) {
            getRandomMove(board,row,col);
        }
        else if (difficulty == HARD) {
            getBestMove(board,row,col);
        }
    }
// setDifficulty
    void setDifficulty(Difficulty newDifficulty) {
        difficulty = newDifficulty;
    }

// getRandomMove -- Person 4
    void getRandomMove(const Board& board, int& row, int& col)const {
        vector<pair<int, int>>emptyCells;
        for (int r = 0;r < board.getSize();r++) {
            for (int c = 0;c < board.getSize();c++) {
                if (board.getCell(r, c) == ' ') {
                    emptyCells.push_back({ r, c });
                }
            }
        }
        int randomIndex = rand() % emptyCells.size();
        row = emptyCells[randomIndex].first;
        col = emptyCells[randomIndex].second;
    }

void getBestMove(Board& board, int& row, int& col) const{
    char aiSymbol = this->getSymbol();
    char opponentSymbol = (aiSymbol == 'X') ? 'O' : 'X';
    
    row = -1;
    col = -1;
    int best_score = INT_MIN;

    for(int r = 0; r < board.getSize(); r++){
        for(int c = 0; c < board.getSize(); c++){
            if(board.isValidMove(r, c)){
                board.makeMove(r, c, aiSymbol);
                int score = minimax(board,false,aiSymbol,opponentSymbol);
                board.undoMove(r, c);
                if(score >= best_score){
                    best_score = score;
                    row = r;
                    col = c;
                }
            }
        }
    }

 } //-- Person 5

int evaluateBoard(const Board& board) const{
    char aiSymbol = this->getSymbol();
    char opponentSymbol = (aiSymbol == 'X') ? 'O' : 'X';
    if(board.checkWin(aiSymbol)){
        return 10;
    } else if(board.checkWin(opponentSymbol)){
        return -10;
    } else {
        return 0;
    }
}

int minimax(Board& board,bool isAi, char ai, char player) const//isAi here means if its the Ais turn or not
{
    if (board.checkWin(ai) || board.checkWin(player) || board.isFull()){
    return evaluateBoard(board);
    }

    int best = isAi ? INT_MIN : INT_MAX;

    for (int r = 0; r < board.getSize(); r++)
    {
        for (int c = 0; c < board.getSize(); c++)
        {
            if (board.isValidMove(r, c))
            {
                board.makeMove(r, c, isAi ? ai : player);
                int score = minimax(board, !isAi, ai, player);
                board.undoMove(r,c);
                if(isAi){
                    best = max(best, score);
                }
                 else {
                    best = min(best, score);
                }
            }
        }
    }

    return best;
}
};


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

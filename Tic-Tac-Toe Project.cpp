#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <algorithm>
#include <utility>
#include <limits>
#include <cctype>

using namespace std;

enum Difficulty { EASY, HARD };

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

    bool isValidMove(int row, int col) const
    {
        if (row < 0 || row >= size || col < 0 || col >= size) return false;
        return grid[row][col] == ' ';
    }

    bool makeMove(int row, int col, char symbol)
    {
        if (!isValidMove(row, col)) return false;
        grid[row][col] = symbol;
        return true;
    }

    char getCell(int row, int col) const
    {
        return grid[row][col];
    }

    void reset()
    {
        grid.assign(size, vector<char>(size, ' '));
    }

    int getSize() const
    {
        return size;
    }

    bool checkWin(char symbol) const
    {
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

    bool isFull() const
    {
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                if (grid[r][c] == ' ') return false;
            }
        }
        return true;
    }

    bool checkGameEnd() const
    {
        return checkWin('X') || checkWin('O') || isFull();
    }

    void undoMove(int row, int col)
    {
        grid[row][col] = ' ';
    }
};

class Player
{
protected:
    string name;
    char symbol;

public:
    Player(const string& name, char symbol) : name(name), symbol(symbol) {}

    virtual void getMove(Board& board, int& row, int& col) = 0; // pure virtual

    string getName() const { return name; }
    char getSymbol() const { return symbol; }
    void setName(const string& name) { this->name = name; }

    virtual ~Player() {}
};

// Concrete Player used for PvP and the human side of PvC.
// The real input reading/validation for a turn happens in
// Game::handleHumanMove(); this override just satisfies the
// abstract Player interface.
class HumanPlayer : public Player
{
public:
    HumanPlayer(const string& name, char symbol) : Player(name, symbol) {}

    void getMove(Board& board, int& row, int& col) override
    {
        (void)board; // not used here - see Game::handleHumanMove()
        cin >> row >> col;
        row--; col--;
    }
};

class AIPlayer : public Player
{
private:
    Difficulty difficulty;

public:

    AIPlayer(const string& name, char symbol, Difficulty difficulty) :
        Player(name, symbol)
    {
        this->difficulty = difficulty;
    }

    void getMove(Board& board, int& row, int& col) override
    {
        if (difficulty == EASY) {
            getRandomMove(board, row, col);
        }
        else if (difficulty == HARD) {
            getBestMove(board, row, col);
        }
    }

    void setDifficulty(Difficulty newDifficulty)
    {
        difficulty = newDifficulty;
    }

    void getRandomMove(const Board& board, int& row, int& col) const
    {
        vector<pair<int, int>> emptyCells;
        for (int r = 0; r < board.getSize(); r++) {
            for (int c = 0; c < board.getSize(); c++) {
                if (board.getCell(r, c) == ' ') {
                    emptyCells.push_back({ r, c });
                }
            }
        }
        int randomIndex = rand() % emptyCells.size();
        row = emptyCells[randomIndex].first;
        col = emptyCells[randomIndex].second;
    }

    void getBestMove(Board& board, int& row, int& col) const
    {
        char aiSymbol = this->getSymbol();
        char opponentSymbol = (aiSymbol == 'X') ? 'O' : 'X';

        row = -1;
        col = -1;
        int best_score = INT_MIN;

        for (int r = 0; r < board.getSize(); r++) {
            for (int c = 0; c < board.getSize(); c++) {
                if (board.isValidMove(r, c)) {
                    board.makeMove(r, c, aiSymbol);
                    int score = minimax(board, false, aiSymbol, opponentSymbol);
                    board.undoMove(r, c);
                    if (score >= best_score) {
                        best_score = score;
                        row = r;
                        col = c;
                    }
                }
            }
        }
    }

    int evaluateBoard(const Board& board) const
    {
        char aiSymbol = this->getSymbol();
        char opponentSymbol = (aiSymbol == 'X') ? 'O' : 'X';
        if (board.checkWin(aiSymbol)) {
            return 10;
        } else if (board.checkWin(opponentSymbol)) {
            return -10;
        } else {
            return 0;
        }
    }

    int minimax(Board& board, bool isAi, char ai, char player) const
    {
        if (board.checkWin(ai) || board.checkWin(player) || board.isFull()) {
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
                    board.undoMove(r, c);
                    if (isAi) {
                        best = max(best, score);
                    } else {
                        best = min(best, score);
                    }
                }
            }
        }
        return best;
    }
};

class Game
{
private:
    Board board;
    Player* player1;
    Player* player2;
    Player* currentPlayer;
    bool running;

public:

    Game() : board(3), player1(nullptr), player2(nullptr),
             currentPlayer(nullptr), running(true) {}

    ~Game()
    {
        delete player1;
        delete player2;
    }

    void showMenu()
    {
        int choice;
        do {
            cout << "\n================\n";
            cout << "  TIC-TAC-TOE GAME\n";
            cout << "================\n";
            cout << "1. Player vs Player\n";
            cout << "2. Player vs Computer (Easy)\n";
            cout << "3. Player vs Computer (Hard)\n";
            cout << "4. Exit\n";
            cout << "Select game mode: ";

            if (!(cin >> choice)) {
                if (cin.eof()) {
                    cout << "\nNo more input available. Exiting game. " << endl;
                    exit(0);
                }
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } while (choice < 1 || choice > 4);

        switch (choice) {
            case 1: setupPvP(); break;
            case 2: setupPvC(EASY); break;
            case 3: setupPvC(HARD); break;
            case 4: running = false; break;
        }
    }

    void setupPvP()
    {
        string n1, n2;
        cout << "Enter name for Player 1 (X): ";
        cin >> n1;
        cout << "Enter name for Player 2 (O): ";
        cin >> n2;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        delete player1; delete player2;
        player1 = new HumanPlayer(n1, 'X');
        player2 = new HumanPlayer(n2, 'O');
    }

    void setupPvC(Difficulty difficulty)
    {
        string name;
        cout << "Enter your name: ";
        cin >> name;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        delete player1; delete player2;
        player1 = new HumanPlayer(name, 'X');
        player2 = new AIPlayer("Computer", 'O', difficulty);
    }

    void switchPlayer()
    {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }

    void handleHumanMove()
    {
        while (true)
        {
            cout << currentPlayer->getName() << " (" << currentPlayer->getSymbol()
                 << "), Enter your move ( row and column) :  ";

            string line;
            if (!getline(cin, line))
            {
                cout << "\nNo more input available. Exiting game." << endl;
                exit(0);
            }

            // Keep only the digit characters, ignoring spaces, commas, etc.
            string digits;
            for (char ch : line)
            {
                if (isdigit(static_cast<unsigned char>(ch)))
                    digits += ch;
            }

            if (digits.size() != 2)
            {
                cout << "Invalid input: please enter a row and a column." << endl;
                continue;
            }

            int row = (digits[0] - '0') - 1;
            int col = (digits[1] - '0') - 1;

            if (row >= 0 && row < board.getSize() && col >= 0 && col < board.getSize() && board.isValidMove(row, col))
            {
                board.makeMove(row, col, currentPlayer->getSymbol());
                return;
            }
            cout << "Invalid move! Please try again." << endl;
        }
    }

    void handleAIMove()
    {
        int row, col;
        currentPlayer->getMove(board, row, col);
        board.makeMove(row, col, currentPlayer->getSymbol());
        cout << currentPlayer->getName() << " placed at ("
             << row + 1 << ", " << col + 1 << ")" << endl;
    }

    bool checkGameEnd()
    {
        return board.checkWin(currentPlayer->getSymbol()) || board.isFull();
    }

    void displayResult() const
    {
        if (board.checkWin(player1->getSymbol()))
            cout << player1->getName() << " wins\n";
        else if (board.checkWin(player2->getSymbol()))
            cout << player2->getName() << " wins\n";
        else
            cout << "It's a draw\n";
    }

    void reset()
    {
        board.reset();
        currentPlayer = player1;
    }

    void start()
    {
        running = true;
        while (running)
        {
            showMenu();
            if (!running) break;

            reset();

            bool gameOver = false;
            while (!gameOver)
            {
                board.display();
                cout << currentPlayer->getName() << "'s turn ("
                     << currentPlayer->getSymbol() << ")\n";

                if (dynamic_cast<AIPlayer*>(currentPlayer)) {
                    handleAIMove();
                } else {
                    handleHumanMove();
                }

                gameOver = checkGameEnd();
                if (!gameOver) switchPlayer();
            }

            board.display();
            displayResult();

            cout << "Play again? (y/n): ";
            char again = 'n';
            if (!(cin >> again) || tolower(again) != 'y') running = false;
        }
        cout << "Thanks for playing!\n";
    }
};

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    Game game;
    game.start();

    return 0;
}

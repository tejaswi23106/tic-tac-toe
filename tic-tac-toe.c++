// Tic-Tac-Toe with Difficulty Modes
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

#define COMPUTER 1
#define HUMAN 2
#define SIDE 3
#define COMPUTERMOVE 'O'
#define HUMANMOVE 'X'

int difficulty = 3; // 1 = Easy, 2 = Medium, 3 = Hard

void showBoard(vector<vector<char>>& board) {
    for (int i = 0; i < SIDE; i++) {
        cout << "\t\t\t ";
        for (int j = 0; j < SIDE; j++) {
            cout << board[i][j];
            if (j < 2) cout << " | ";
        }
        cout << "\n";
        if (i < 2) cout << "\t\t\t-----------\n";
    }
    cout << "\n";
}

void showInstructions() {
    cout << "\nChoose a cell numbered from 1 to 9 as below and play\n\n";
    cout << "\t\t\t 1 | 2 | 3 \n";
    cout << "\t\t\t-----------\n";
    cout << "\t\t\t 4 | 5 | 6 \n";
    cout << "\t\t\t-----------\n";
    cout << "\t\t\t 7 | 8 | 9 \n\n";
}

void initialise(vector<vector<char>>& board) {
    for (int i = 0; i < SIDE; i++)
        for (int j = 0; j < SIDE; j++)
            board[i][j] = '*';
}

void declareWinner(int whoseTurn) {
    cout << ((whoseTurn == COMPUTER) ? "COMPUTER" : "HUMAN") << " has won\n";
}

bool rowCrossed(vector<vector<char>>& board) {
    for (int i = 0; i < SIDE; i++)
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != '*')
            return true;
    return false;
}

bool columnCrossed(vector<vector<char>>& board) {
    for (int i = 0; i < SIDE; i++)
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != '*')
            return true;
    return false;
}

bool diagonalCrossed(vector<vector<char>>& board) {
    return (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '*') ||
           (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '*');
}

bool gameOver(vector<vector<char>>& board) {
    return rowCrossed(board) || columnCrossed(board) || diagonalCrossed(board);
}

int minimax(vector<vector<char>>& board, int depth, bool isAI, int alpha, int beta, int maxDepth) {
    if (gameOver(board)) return isAI ? -10 : 10;
    if (depth >= maxDepth || depth >= 9) return 0;

    if (isAI) {
        int bestScore = -999;
        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                if (board[i][j] == '*') {
                    board[i][j] = COMPUTERMOVE;
                    int score = minimax(board, depth + 1, false, alpha, beta, maxDepth);
                    board[i][j] = '*';
                    bestScore = max(score, bestScore);
                    alpha = max(alpha, bestScore);
                    if (beta <= alpha) return bestScore;
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 999;
        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                if (board[i][j] == '*') {
                    board[i][j] = HUMANMOVE;
                    int score = minimax(board, depth + 1, true, alpha, beta, maxDepth);
                    board[i][j] = '*';
                    bestScore = min(score, bestScore);
                    beta = min(beta, bestScore);
                    if (beta <= alpha) return bestScore;
                }
            }
        }
        return bestScore;
    }
}

int bestMove(vector<vector<char>>& board, int moveIndex) {
    // Easy mode — random move
    if (difficulty == 1) {
        vector<int> options;
        for (int i = 0; i < SIDE; i++)
            for (int j = 0; j < SIDE; j++)
                if (board[i][j] == '*') options.push_back(i * 3 + j);
        return options[rand() % options.size()];
    }

    // Medium mode — 25% chance of random move
    if (difficulty == 2 && rand() % 4 == 0) {
        vector<int> options;
        for (int i = 0; i < SIDE; i++)
            for (int j = 0; j < SIDE; j++)
                if (board[i][j] == '*') options.push_back(i * 3 + j);
        return options[rand() % options.size()];
    }

    // Hard mode or depth-limited Minimax
    int maxDepth = (difficulty == 2) ? 3 : 9;
    int x = -1, y = -1, bestScore = -999;

    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            if (board[i][j] == '*') {
                board[i][j] = COMPUTERMOVE;
                int score = minimax(board, moveIndex + 1, false, -1000, 1000, maxDepth);
                board[i][j] = '*';
                if (score > bestScore) {
                    bestScore = score;
                    x = i;
                    y = j;
                }
            }
        }
    }
    return x * 3 + y;
}

void playTicTacToe(int whoseTurn) {
    vector<vector<char>> board(SIDE, vector<char>(SIDE));
    int moveIndex = 0, x = 0, y = 0;
    initialise(board);
    showInstructions();

    while (!gameOver(board) && moveIndex != SIDE * SIDE) {
        int n;
        if (whoseTurn == COMPUTER) {
            n = bestMove(board, moveIndex);
            x = n / SIDE;
            y = n % SIDE;
            board[x][y] = COMPUTERMOVE;
            cout << "COMPUTER has put a " << COMPUTERMOVE << " in cell " << (n + 1) << "\n\n";
            showBoard(board);
            moveIndex++;
            whoseTurn = HUMAN;
        } else {
            cout << "You can insert in the following positions : ";
            for (int i = 0; i < SIDE; i++)
                for (int j = 0; j < SIDE; j++)
                    if (board[i][j] == '*')
                        cout << (i * 3 + j) + 1 << " ";
            cout << "\n\nEnter the position = ";
            cin >> n;
            n--;
            x = n / SIDE;
            y = n % SIDE;

            while (n < 0 || n >= 9 || board[x][y] != '*') {
                cout << "Invalid or occupied position. Enter again: ";
                cin >> n;
                n--;
                x = n / SIDE;
                y = n % SIDE;
            }

            board[x][y] = HUMANMOVE;
            cout << "\nHUMAN has put a " << HUMANMOVE << " in cell " << (n + 1) << "\n\n";
            showBoard(board);
            moveIndex++;
            whoseTurn = COMPUTER;
        }
    }

    if (!gameOver(board) && moveIndex == SIDE * SIDE)
        cout << "It's a draw\n";
    else {
        whoseTurn = (whoseTurn == COMPUTER) ? HUMAN : COMPUTER;
        declareWinner(whoseTurn);
    }
}

int main() {
    srand(time(0));
    cout << "\n-------------------------------------------------------------------\n\n";
    cout << "\t\t\t Tic-Tac-Toe\n";
    cout << "\n-------------------------------------------------------------------\n\n";
    cout << "Choose difficulty level:\n1. Easy\n2. Medium\n3. Hard\nYour choice: ";
    cin >> difficulty;

    char cont = 'y';
    do {
        char choice;
        cout << "Do you want to start first?(y/n) : ";
        cin >> choice;
        if (choice == 'n')
            playTicTacToe(COMPUTER);
        else if (choice == 'y')
            playTicTacToe(HUMAN);
        else
            cout << "Invalid choice\n";

        cout << "\nDo you want to quit(y/n) : ";
        cin >> cont;
    } while (cont == 'n');
    return 0;
}


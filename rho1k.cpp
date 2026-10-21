#include <fstream>
#include <iostream>
#include <string>
#include <thread>

std::string versionDesc = "Sudoken! v1.12.1\n\n";

void loadBoardFromFile(std::string& filename, int board[9][9]) {
    std::ifstream file(filename);

    if (file.is_open()) {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                // Reads integers directly, including the 0s
                file >> board[row][col];
            }
        }
        file.close();
        std::cout << versionDesc;
        std::cout << "Board file " << filename << " loaded successfully!\n\n";
    }
    else {
        std::cout << "Error: Could not open file.\n\n";
    }
}

void checkBoardStart(int board[9][9], int boardStart[9][9]) {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board[row][col] == 0) {
                boardStart[row][col] = 0; // cell is empty
            }
            else {
                boardStart[row][col] = 1; // cell is filled at start: no changes can be made to that cell
            }
        }
    }
}

void printBoard(int board[9][9]) {
    std::cout << "     1 2 3  4 5 6  7 8 9(X)" << std::endl;
    std::cout << "   +---------------------+" << std::endl;
    for (int rows = 0; rows <= 8; rows++) {
        if (rows % 3 == 0)
            if (rows != 0)
                std::cout << "   |                     |\n";

        std::cout << " " << rows + 1 << " | ";
        for (int cols = 0; cols <= 8; cols++) {
            if (cols % 3 == 0)
                if (cols != 0)
                    std::cout << " ";
            if (board[rows][cols] == 0) {
                std::cout << "  ";
            }
            else {
                std::cout << board[rows][cols] << " ";
            }
        }
        std::cout << "|";
        std::cout << std::endl;
    }
    std::cout << "(Y)+---------------------+\n\n";
}

void clearScreen() {
    // \033[H moves the cursor to the top-left corner
    // \033[2J clears the entire screen
    std::cout << "\033[H\033[2J";
}

bool checkWin(int board[9][9]) {
    // 1. Check if the board is fully filled first
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] == 0) return false; // Found an empty space, game not over
        }
    }

    // 2. Check rows and cols for uniqueness
    for (int i = 0; i < 9; i++) {
        bool rowSeen[10] = { false };
        bool colSeen[10] = { false };

        for (int j = 0; j < 9; j++) {
            // Check row 'i'
            int rowVal = board[i][j];
            if (rowSeen[rowVal] == true) return false; // Duplicate in row
            rowSeen[rowVal] = true;

            // Check column 'i'
            int colVal = board[j][i];
            if (colSeen[colVal] == true) return false; // Duplicate in column
            colSeen[colVal] = true;
        }
    }

    // 3. Check all nine 3x3 subgrids
    for (int gridRow = 0; gridRow < 9; gridRow += 3) {
        for (int gridCol = 0; gridCol < 9; gridCol += 3) {

            bool gridSeen[10] = { false };

            for (int r = 0; r < 3; ++r) {
                for (int c = 0; c < 3; ++c) {
                    int val = board[gridRow + r][gridCol + c];
                    if (gridSeen[val] == true) return false; // Duplicate in subgrid
                    gridSeen[val] = true;
                }
            }
        }
    }

    return true; // Survived all checks, board is perfectly solved
}

bool isSafe(int board[9][9], int row, int col, int num) {
    // Check if the number is already present in the current row
    for (int x = 0; x < 9; x++) {
        if (board[row][x] == num) {
            return false;
        }
    }

    // Check if the number is already present in the current column
    for (int x = 0; x < 9; x++) {
        if (board[x][col] == num) {
            return false;
        }
    }

    // Check if the number is already present in the 3x3 subgrid
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }
    // Number can be safely placed
    return true;
}

bool solveSudokuBacktracking(int board[9][9]) {
    int row = -1;
    int col = -1;
    bool isEmpty = true;

    // Find an empty cell in the Sudoku board
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                row = i;
                col = j;
                isEmpty = false;
                break;
            }
        }
        if (!isEmpty) {
            break;
        }
    }

    // If no empty cell is found, Sudoku is solved
    if (isEmpty) {
        return true;
    }

    // Try placing numbers 1 to 9 in the empty cell
    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;

            // Recursively attempt to solve the rest of the board
            if (solveSudokuBacktracking(board)) {
                return true;
            }

            // If placing num does not lead to a solution, backtrack
            board[row][col] = 0;
        }
    }
    return false;
}

int main()
{
    while (true) {
        clearScreen();
        std::cout << versionDesc;

        std::cout << "MAIN MENU\n\n";
        std::cout << "Sudoku is a very... game. Let's play!\n\n";

        int menuSelect;
        std::cout << "1: Load Game\n"
            << "2: Quit\n\n>> ";
        std::cin >> menuSelect;

        if (std::cin.fail()) {
            std::cin.clear(); // Clears the error state in the terminal
            std::cin.ignore(10000, '\n'); // Discards the bad input characters
            std::cout << "Invalid selection, quitting program\n\nLoading...";
            menuSelect = 2;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        if (menuSelect == 2) {
            clearScreen();
            std::cout << versionDesc;

            std::cout << "Until next time!\n";
            return 0;
        }
        std::cin.ignore();

        clearScreen();
        std::cout << versionDesc; // just because ;)
        std::cout << "LOAD GAME\n\n";

        std::string filename;
        std::cout << "Enter file name to load board (i.e. board.txt)\n>> ";
        std::getline(std::cin, filename);
        // ifstream is used for reading files
        std::ifstream inf{ filename };

        // if unable to open input file
        if (!inf)
        {
            // print an error and exit
            std::cerr << "Uh oh, " << filename << " could not be opened for reading!\n";
            continue;
        }

        clearScreen();

        std::string answer;
        bool solved = false;

        // Declare the 2D array to store the board
        int board[9][9];


        loadBoardFromFile(filename, board);
        //std::cout << "Value at 8, 7 is " << board[6][7] << "\n";

        int boardStart[9][9];
        checkBoardStart(board, boardStart);


        // to debug boardStart (first half)
        /*for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                std::cout << boardStart[i][j] << " ";
            }
            std::cout << std::endl;
        }*/

        while (true) {
            printBoard(board);

            int action;
            std::cout << "1: Fill cell\n";
            std::cout << "2: Clear cell\n";
            std::cout << "3: Reset board\n";
            std::cout << "4: Show solution\n>> ";

            std::cin >> action;

            if (std::cin.fail()) {
                std::cin.clear(); // Clears the error state in the terminal
                std::cin.ignore(10000, '\n'); // Discards the bad input characters
                action = -1; // Force the switch statement to hit 'default'
            }

            switch (action) {
            case 1: { // fill cell //
                int x, y;
                std::cout << "Select a cell by typing in the format: X Y (for instance, 2 3)\n>> ";
                std::cin >> x >> y;

                // Validation loop
                while (boardStart[y - 1][x - 1] == 1) {
                    clearScreen();
                    std::cout << versionDesc;
                    std::cout << "You have selected x = " << x << ", y = " << y << "\n" << std::endl;

                    printBoard(board);
                    std::cout << "That cell cannot be changed.\n";
                    std::cout << "Please select a valid cell using format X Y (for instance, 7 4)\n>> ";
                    std::cin >> x >> y;
                }

                std::cout << "Enter a number from 1 to 9 \n>> ";
                std::cin >> board[y - 1][x - 1];

                clearScreen();
                std::cout << versionDesc;
                std::cout << "The value of cell " << x << ", " << y << " has changed to " << board[y - 1][x - 1] << "\n\n";

                // Check win condition after a move is made
                if (checkWin(board)) {
                    printBoard(board);

                    std::cout << "========================================\n";
                    std::cout << "     VICTORY! You solved the board! \n";
                    std::cout << "========================================\n";

                    /*std::cout << "Press Enter to return to the Main Menu...";
                    std::cin.ignore(10000, '\n');
                    std::string pause;
                    std::getline(std::cin, pause);*/

                    break; // Exits the inner game loop instead, back to main menu
                }
                break;
            }

            case 2: { // clear cell //
                int x, y;
                std::cout << "Select a cell to clear (X Y)\n>> ";
                std::cin >> x >> y;

                if (boardStart[y - 1][x - 1] == 1) {
                    clearScreen();
                    std::cout << versionDesc;
                    std::cout << "Cannot clear a starting cell!\n\n";
                }
                else {
                    board[y - 1][x - 1] = 0; // Reset the cell to empty

                    clearScreen();
                    std::cout << versionDesc;
                    std::cout << "Cell " << x << ", " << y << " cleared.\n\n";
                }
                break;
            }

            case 3: { // reset board //
                // Loop through the board and erase any cell that wasn't filled at the start
                for (int r = 0; r < 9; ++r) {
                    for (int c = 0; c < 9; ++c) {
                        if (boardStart[r][c] == 0) {
                            board[r][c] = 0;
                        }
                    }
                }
                clearScreen();
                std::cout << versionDesc;
                std::cout << "Board has been reset to its initial state.\n\n";
                break;
            }

            case 4: { // solve //
                clearScreen();
                std::cout << versionDesc;

                // 1. Wipe all user inputs and reset to the original file state
                for (int r = 0; r < 9; ++r) {
                    for (int c = 0; c < 9; ++c) {
                        if (boardStart[r][c] == 0) {
                            board[r][c] = 0;
                        }
                    }
                }

                // 2. Run the solver on the freshly cleaned board
                if (solveSudokuBacktracking(board)) {
                    std::cout << "Automated solver successful!\n\n";

                    printBoard(board);
                    /*std::cout << "\n\nPress Enter to return to the Main Menu...";
                    std::cin.ignore(10000, '\n');
                    std::string pause;
                    std::getline(std::cin, pause);

                    break; // Exits the inner game loop*/
                }
                else {
                    std::cout << "No solution exists for the original board state.\n\n";
                }
                break;
            }

            default: {
                clearScreen();
                std::cout << versionDesc;
                std::cout << "Invalid selection. Please enter a number from 1 to 4.\n\n";
                break;
            }
            }
        }
    }


    return 0;

    // When inf goes out of scope, the ifstream
    // destructor will close the file
}



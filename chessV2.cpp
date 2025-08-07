#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>

#define RESET "\033[0m"
#define WHITE "\033[1;97m" // Bright white (solid)
#define BLACK "\033[0;37m" // Dim gray (transparent)
#define EMPTY "\033[90m"   // Dark gray for empty squares

class Cursor
{
public:
    int xCord, yCord, enterCounter;
    bool counterTrigger;

public:
    Cursor() : xCord(0), yCord(0), enterCounter(0), counterTrigger(false) {}

    void up()
    {
        if (yCord < 7)
        {
            yCord++;
        }
        else if (yCord == 7)
        {
            yCord = 0;
        }
    }
    void down()
    {
        if (yCord > 0)
        {
            yCord--;
        }
        else if (yCord == 0)
        {
            yCord = 7;
        }
    }
    void left()
    {
        if (xCord > 0)
        {
            xCord--;
        }
        else if (xCord == 0)
        {
            xCord = 7;
        }
    }
    void right()
    {
        if (xCord < 7)
        {
            xCord++;
        }
        else if (xCord == 7)
        {
            xCord = 0;
        }
    }

    void updateCursor(char ch)
    {

        switch (ch)
        {
        case 'w':
            down();
            break;
        case '8':
            down();
            break;

        case 's':
            up();
            break;
        case '2':
            up();
            break;

        case 'a':
            left();
            break;
        case '4':
            left();
            break;

        case 'd':
            right();
            break;
        case '6':
            right();
            break;

        case 'q':
            down();
            left();
            break;
        case '7':
            down();
            left();
            break;

        case 'e':
            down();
            right();
            break;
        case '9':
            down();
            right();
            break;

        case 'c':
            up();
            right();
            break;
        case '3':
            up();
            right();
            break;

        case 'z':
            up();
            left();
            break;
        case '1':
            up();
            left();
            break;

        case '\n':
            counterTrigger = true;
            enterCounter++;
            break;
        default:
            break;
        }
    }
};

class Board
{ // Initilazation Movingsystem Display Clear
public:
    int arr[8][8];
    std::vector<int> whiteKilled;
    std::vector<int> blackKilled;
    int currentTurn;
    Cursor &cursor;

public:
    Board(Cursor &c) : cursor(c), currentTurn(0) {}

    void initialize()
    {

        int i, j;
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                arr[i][j] = 0;
            }
        }

        int white[] = {2, 3, 4, 5, 6, 4, 3, 2};

        for (i = 0; i < 8; i++)
        {
            arr[1][i] = 7;
            arr[6][i] = 1;
            arr[0][i] = white[i] + 6;
            arr[7][i] = white[i];
        }
    };

    void moveFromTo(int oldX, int oldY, int x, int y)
    {

        int val = arr[y][x];

        if (val != 0)
        {
            if (val > 6)
            {
                whiteKilled.push_back(val);
            }
            else
            {
                blackKilled.push_back(val);
            }
        }

        arr[y][x] = arr[oldY][oldX];
        arr[oldY][oldX] = 0;

        currentTurn = !currentTurn;
    }

    void display()
    {
        std::cout << "\033c";
        // system("clear");
        std::string p[13] = {" ", "♟", "♜", "♞", "♝", "♛", "♚", "♙", "♖", "♘", "♗", "♕", "♔"};
        // std::string p[13] = {" ","♙", "♖", "♘", "♗", "♕", "♔", "♟", "♜", "♞", "♝", "♛", "♚"};
        std::cout << "\n";

        // board
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                int val = arr[i][j];

                if (cursor.xCord == j && cursor.yCord == i)
                {
                    if (val <= 6)
                    {
                        std::cout << "\033[44;97m" << " " << "\033[44;97m" << p[arr[i][j]] << " " << RESET;
                    }
                    else
                    {
                        std::cout << "\033[44;30m" << " " << "\033[44;30m" << p[arr[i][j]] << " " << RESET;
                    }
                }
                else
                {
                    if (val == 0)
                        std::cout << " " << EMPTY << "· " << RESET;
                    else if (val <= 6)
                        std::cout << " " << WHITE << p[val] << " " << RESET;
                    else
                        std::cout << " " << BLACK << p[val] << " " << RESET;
                }
            }

            std::cout << "\n";
        }

        // player Turn

        if (currentTurn == 0)
        {
            std::cout << WHITE << "\n      White's Turn\n";
        }
        else
        {
            std::cout << "\033[1;30m" << "\n      Black's Turn\n";
        }

        // Captured pieces
        std::cout << "\n";

        std::cout << WHITE << " White Captures: " << RESET;
        for (int i = 0; i < whiteKilled.size(); i++)
            std::cout << WHITE << p[whiteKilled.at(i)] << " ";
        std::cout << "\n";

        std::cout << "\033[1;30m" << " Black Captures: " << RESET;
        for (int i = 0; i < blackKilled.size(); i++)
            std::cout << BLACK << p[blackKilled.at(i)] << " ";
        std::cout << "\n"
                  << RESET;
        std::cout << "\n";
    }
};

class MoveValidation
{
public:
    int possibleMoves[28][2];
    int arrCpy[8][8];
    int possibleMovesIndex;
    int tempCord[2];
    int simulatedTempCord[2];
    Board &board;
    int target;

public:
    MoveValidation(Board &b) : board(b), possibleMovesIndex(0) {}

    void setTarget()
    {
        target = 0;

        if (board.currentTurn)
        {
            // blacks turn;
            target = 6;
        }
    }

    int boardValue()
    {
        return board.arr[tempCord[1]][tempCord[0]];
    }

    bool boundCheck()
    {
        if (tempCord[0] >= 0 && tempCord[0] <= 7 && tempCord[1] >= 0 && tempCord[1] <= 7)
        {
            return true;
        }
        return false;
    }

    bool checkPlayedMove(int newX, int newY)
    {
        int i;
        for (i = 0; i < possibleMovesIndex; i++)
        {
            if (possibleMoves[i][0] == newX && possibleMoves[i][1] == newY)
            {
                possibleMovesIndex = 0;
                return true;
            }
        }
        possibleMovesIndex = 0;
        return false;
    }

    void copyArr()
    {
        int i, j;
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                arrCpy[i][j] = board.arr[i][j];
            }
        }
    }

    int simulatedBoardValue()
    {
        return arrCpy[simulatedTempCord[1]][simulatedTempCord[0]];
    }

    bool simulatedBoundCheck()
    {
        if (simulatedTempCord[0] >= 0 && simulatedTempCord[0] <= 7 && simulatedTempCord[1] >= 0 && simulatedTempCord[1] <= 7)
        {
            return true;
        }
        return false;
    }

    void simulatedKingXY(int &x, int &y)
    {
        int i, j;
        for (i = 0; i < 8; i++)
        {

            for (j = 0; j < 8; j++)
            {
                if (arrCpy[i][j] == 6 + target)
                {
                    x = j;
                    y = i;
                    return;
                }
            }
        }
    }

    bool simulatedKingInCheck(int oldX, int oldY, int x, int y)
    {

        int i, j;
        int kingX;
        int kingY;

        // simulating move in arrCpy;
        copyArr();
        arrCpy[y][x] = arrCpy[oldY][oldX];
        arrCpy[oldY][oldX] = 0;
        simulatedKingXY(kingX, kingY);

        // check for rook and queen
        for (i = 0; i < 4; i++)
        {
            for (j = 1; j < 9; j++)
            {
                switch (i)
                {
                case 0:
                    simulatedTempCord[0] = kingX;
                    simulatedTempCord[1] = kingY + j;
                    break;
                case 1:
                    simulatedTempCord[0] = kingX;
                    simulatedTempCord[1] = kingY - j;
                    break;
                case 2:
                    simulatedTempCord[0] = kingX + j;
                    simulatedTempCord[1] = kingY;
                    break;
                case 3:
                    simulatedTempCord[0] = kingX - j;
                    simulatedTempCord[1] = kingY;
                    break;

                default:
                    break;
                }

                if (simulatedBoundCheck())
                {
                    if (simulatedBoardValue() == 0)
                    {
                        continue;
                    }
                    else if (simulatedBoardValue() == 8 - target || simulatedBoardValue() == 11 - target)
                    {
                        return true;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    // if (non enemy queen or rook piece found)
                    break;
                }
            }
        }

        // check for bishop and queen;
        for (i = 0; i < 4; i++)
        {
            for (j = 1; j < 9; j++)
            {
                switch (i)
                {
                case 0:
                    simulatedTempCord[0] = kingX + j;
                    simulatedTempCord[1] = kingY - j;
                    break;
                case 1:
                    simulatedTempCord[0] = kingX - j;
                    simulatedTempCord[1] = kingY + j;
                    break;
                case 2:
                    simulatedTempCord[0] = kingX - j;
                    simulatedTempCord[1] = kingY - j;
                    break;
                case 3:
                    simulatedTempCord[0] = kingX + j;
                    simulatedTempCord[1] = kingY + j;
                    break;

                default:
                    break;
                }

                if (simulatedBoundCheck())
                {
                    if (simulatedBoardValue() == 0)
                    {
                        continue;
                    }
                    else if (simulatedBoardValue() == 10 - target || simulatedBoardValue() == 11 - target)
                    {
                        return true;
                    }
                    else
                    {
                        // if (non enemy queen or bishop piece found)
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }

        // if move is made by king only then
        // check for king
        {
            int dx[8] = {0, 0, 1, 1, 1, -1, -1, -1};
            int dy[8] = {1, -1, 0, 1, -1, 0, 1, -1};

            for (i = 0; i < 8; i++)
            {
                simulatedTempCord[0] = kingX + dx[i];
                simulatedTempCord[1] = kingY + dy[i];
                if (simulatedBoundCheck())
                {
                    if (simulatedBoardValue() == 12 - target)
                    {
                        return true;
                    }
                }
            }
        }

        // check for knight;

        {
            int dx[8] = {2, 2, -2, -2, 1, -1, 1, -1};
            int dy[8] = {1, -1, 1, -1, 2, 2, -2, -2};

            for (i = 0; i < 8; i++)
            {
                simulatedTempCord[0] = kingX + dx[i];
                simulatedTempCord[1] = kingY + dy[i];
                if (simulatedBoundCheck())
                {
                    if (simulatedBoardValue() == 9 - target)
                    {
                        return true;
                    }
                }
            }
        }

        // check for pawn;
        {
            int i;
            int direction = 1;
            int dx[2]={-1,1};

            if(board.currentTurn){
                direction =-1;
            }
            for(i=0;i<2;i++){

            simulatedTempCord[0]=kingX + dx[i];
            simulatedTempCord[1]=kingY + direction;

            if(simulatedBoundCheck()){
                if(simulatedBoardValue() == 7-target){
                    return true;
                }
            }

            }
            

        }
        
        return false;
    }

    void setPossibleMove(int oldX, int oldY)
    {
        if (!simulatedKingInCheck(oldX, oldY, tempCord[0], tempCord[1]))
        {
            possibleMoves[possibleMovesIndex][0] = tempCord[0];
            possibleMoves[possibleMovesIndex][1] = tempCord[1];
            possibleMovesIndex++;
        }
    }

    bool checkCell(int x, int y)
    {

        if (boardValue() == 0)
        {
            setPossibleMove(x, y);
            return true;
        }
        else if (boardValue() > 6 - target && boardValue() <= 12 - target)
        {
            setPossibleMove(x, y);
        }
        return false;
    }

    void prepareChecking()
    {
        setTarget();
    }

    void pawn(int x, int y)
    {
        int direction = -1;
        int startingYValue = 6;

        if (board.currentTurn)
        {
            direction = 1;
            startingYValue = 1;
        }

        tempCord[0] = x;
        tempCord[1] = y + direction;
        if (boundCheck() && boardValue() == 0)
        {
            setPossibleMove(x, y);

            if (y == startingYValue)
            {
                tempCord[0] = x;
                tempCord[1] = y + 2 * direction;
                if (boundCheck() && boardValue() == 0)
                    setPossibleMove(x, y);
            }
        }

        tempCord[0] = x + 1;
        tempCord[1] = y + direction;
        if (boundCheck() && boardValue() > 6 - target && boardValue() <= 12 - target)
            setPossibleMove(x, y);

        tempCord[0] = x - 1;
        tempCord[1] = y + direction;
        if (boundCheck() && boardValue() > 6 - target && boardValue() <= 12 - target)
            setPossibleMove(x, y);
    }

    void rook(int x, int y)
    {
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 1; j < 9; j++)
            {
                switch (i)
                {
                case 0:
                    tempCord[0] = x;
                    tempCord[1] = y + j;
                    break;
                case 1:
                    tempCord[0] = x;
                    tempCord[1] = y - j;
                    break;
                case 2:
                    tempCord[0] = x + j;
                    tempCord[1] = y;
                    break;
                case 3:
                    tempCord[0] = x - j;
                    tempCord[1] = y;
                    break;

                default:
                    break;
                }

                if (boundCheck())
                {
                    if (!checkCell(x, y))
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    void knight(int x, int y)
    {
        int dx[8] = {2, 2, -2, -2, 1, -1, 1, -1};
        int dy[8] = {1, -1, 1, -1, 2, 2, -2, -2};
        int i;
        for (i = 0; i < 8; i++)
        {
            tempCord[0] = x + dx[i];
            tempCord[1] = y + dy[i];
            if (boundCheck())
            {
                checkCell(x, y);
            }
        }
    }

    void bishop(int x, int y)
    {
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 1; j < 9; j++)
            {
                switch (i)
                {
                case 0:
                    tempCord[0] = x + j;
                    tempCord[1] = y - j;
                    break;
                case 1:
                    tempCord[0] = x - j;
                    tempCord[1] = y + j;
                    break;
                case 2:
                    tempCord[0] = x - j;
                    tempCord[1] = y - j;
                    break;
                case 3:
                    tempCord[0] = x + j;
                    tempCord[1] = y + j;
                    break;

                default:
                    break;
                }

                if (boundCheck())
                {
                    if (!checkCell(x, y))
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    void queen(int x, int y)
    {
        rook(x, y);
        bishop(x, y);
    }

    void king(int x, int y)
    {
        int dx[8] = {0, 0, 1, 1, 1, -1, -1, -1};
        int dy[8] = {1, -1, 0, 1, -1, 0, 1, -1};
        int i;

        for (i = 0; i < 8; i++)
        {
            tempCord[0] = x + dx[i];
            tempCord[1] = y + dy[i];
            if (boundCheck())
            {
                checkCell(x, y);
            }
        }
    }

    bool validate(int oldX, int oldY, int newX, int newY)
    {
        int selectedPiece = board.arr[oldY][oldX];
        if (selectedPiece > 6)
        {
            selectedPiece = selectedPiece - 6;
        }

        prepareChecking();

        switch (selectedPiece)
        {
        case 1:
            pawn(oldX, oldY);
            break;
        case 2:
            rook(oldX, oldY);
            break;
        case 3:
            knight(oldX, oldY);
            break;
        case 4:
            bishop(oldX, oldY);
            break;
        case 5:
            queen(oldX, oldY);
            break;
        case 6:
            king(oldX, oldY);
        default:
            break;
        }

        return checkPlayedMove(newX, newY);
    }
};

class InputHandling
{
public:
    int currentX;
    int currentY;
    int val;
    int moveX;
    int moveY;
    Cursor &cursor;
    Board &board;
    MoveValidation moveValidation;

public:
    InputHandling(Cursor &c, Board &b) : cursor(c), board(b), val(0), moveValidation(b), moveX(0), moveY(0), currentX(0), currentY(0) {}

    bool selectPeice()
    {
        val = board.arr[cursor.yCord][cursor.xCord];
        if (val == 0)
        {
            // if empty selected
            return false;
        }
        // ensuring player select their own peice
        if (board.currentTurn == 0 && val <= 6)
        {
            currentX = cursor.xCord;
            currentY = cursor.yCord;
            return true;
        }
        else if (board.currentTurn == 1 && val > 6)
        {
            currentX = cursor.xCord;
            currentY = cursor.yCord;
            return true;
        }
        return false;
    }

    void selectMove()
    {
        moveX = cursor.xCord;
        moveY = cursor.yCord;

        if (moveValidation.validate(currentX, currentY, moveX, moveY))
        {
            board.moveFromTo(currentX, currentY, moveX, moveY);
        }
    }

    void manageInput()
    {
        if (cursor.counterTrigger == true)
        {

            if (cursor.enterCounter == 1)
            {
                if (selectPeice() == false)
                {
                    cursor.enterCounter = 0;
                }
            }
            else
            {
                selectMove();
                cursor.enterCounter = 0;
            }

            cursor.counterTrigger = false;
        }
    }
};

char getch()
{
    char buf = 0;
    termios old = {};
    tcgetattr(STDIN_FILENO, &old); // get current terminal state

    termios newt = old;
    newt.c_lflag &= ~(ICANON | ECHO); // disable canonical mode and echo

    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // set new terminal state

    read(STDIN_FILENO, &buf, 1); // read 1 char

    tcsetattr(STDIN_FILENO, TCSANOW, &old); // restore old terminal state

    return buf;
}

class Game
{
private:
    Cursor c;
    Board b;
    InputHandling i;
    char ch;

public:
    bool gameStatus;

    Game() : c(), b(c), i(c, b), gameStatus(true) {}

    void run()
    {
        b.initialize();
        while (gameStatus)
        {
            b.display();
            ch = getch();
            c.updateCursor(ch);
            i.manageInput();
        }
    }
};

int main()
{
    Game g;
    g.run();

    return 0;
}
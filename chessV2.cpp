#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>

#define RESET "\033[0m"
#define WHITE "\033[1;97m" // Bright white (solid)
#define BLACK "\033[0;37m" // Dim gray (transparent)

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

using cordinateArr = std::vector<std::vector<int>>;

struct displayutility
{
    std::string p[13] = {" ", "♟", "♜", "♞", "♝", "♛", "♚", "♟", "♜", "♞", "♝", "♛", "♚"};

    // pallete 1

    std::string bgColors[4] = {
        "\033[48;2;240;217;181m", // light square = classic beige
        "\033[48;2;181;136;99m",  // dark square = classic brown
        "\033[48;2;150;200;180m", // cursor = muted teal green
        "\033[48;2;255;210;100m"  // legal move = warm golden yellow
    };

    std::string whitePieceTextColor = {
        "\033[38;2;255;255;255m" // pure white
    };

    std::string blackPieceTextColor = {
        "\033[38;2;0;0;0m" // pure black
    };

    std::string emptyTextColor = {
        "\033[30m" // black text
    };

    // pallete 2

    // std::string bgColors[4] = {
    //     "\033[47m",  // light square = white bg
    //     "\033[42m",  // dark square = green bg
    //     "\033[106m", // cursor background = light cyan bg
    //     "\033[103m"  // legal move bg = bright yellow bg
    // };

    // std::string whitePieceTextColor = {
    //     "\033[97m"};

    // std::string blackPieceTextColor = {
    //     "\033[30m"};

    // std::string emptyTextColor = {
    //     "\033[30m"};
};

class Board
{
public:
    int arr[8][8];
    std::vector<int> whiteKilled;
    std::vector<int> blackKilled;
    int currentTurn;
    Cursor &cursor;
    cordinateArr legalMoves;
    const displayutility utility;

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

    bool isLegalSquareCheck(int x, int y)
    {

        int i;
        for (i = 0; i < legalMoves.size(); i++)
        {
            if (legalMoves[i][0] == x && legalMoves[i][1] == y)
            {
                return true;
            }
        }
        return false;
    }

    void display()
    {
        std::cout << "\033c"; // Clear screen

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                int val = arr[i][j];
                bool isCursor = (cursor.xCord == j && cursor.yCord == i);
                bool isLightSquare = ((i + j) % 2 == 0);
                bool isLegalSquare = isLegalSquareCheck(j, i);

                int colorIndex;

                if (isLightSquare)
                {
                    colorIndex = 0;
                }
                else
                {
                    colorIndex = 1;
                }

                if (isCursor)
                {
                    colorIndex = 2;
                }
                else if (isLegalSquare)
                {
                    colorIndex = 3;
                }

                std::string bgColor = utility.bgColors[colorIndex];
                std::string textColor;

                if (val == 0)
                    textColor = utility.emptyTextColor;
                else if (val <= 6)
                    textColor = utility.whitePieceTextColor;
                else
                    textColor = utility.blackPieceTextColor;

                std::cout << bgColor << textColor << " " << utility.p[val] << " " << RESET;
            }
            std::cout << "\n";
        }

        // Player turn info
        if (currentTurn == 0)
        {
            std::cout << WHITE << "\n      White's Turn\n"
                      << RESET;
        }
        else
        {
            std::cout << BLACK << "\n      Black's Turn\n"
                      << RESET;
        }

        // Captured pieces
        std::cout << "\n";

        std::cout << WHITE << " White Captures: " << RESET;
        for (int i = 0; i < whiteKilled.size(); i++)
            std::cout << WHITE << utility.p[whiteKilled.at(i)] << " ";
        std::cout << "\n";

        std::cout << BLACK << " Black Captures: " << RESET;
        for (int i = 0; i < blackKilled.size(); i++)
            std::cout << BLACK << utility.p[blackKilled.at(i)] << " ";
        std::cout << "\n"
                  << RESET << "\n";
    }
};

struct DxDy
{
    const int knightDx[8] = {2, 2, -2, -2, 1, -1, 1, -1};
    const int knightDy[8] = {1, -1, 1, -1, 2, 2, -2, -2};
    const int kingDx[8] = {0, 0, 1, 1, 1, -1, -1, -1};
    const int kingDy[8] = {1, -1, 0, 1, -1, 0, 1, -1};
    const int pawnDx[2] = {1, -1};
};

enum pieces
{
    empty,
    pawn,
    rook,
    knight,
    bishop,
    queen,
    king
};

class MoveGeneration
{
public:
    Board &board;
    int tempCord[2];
    int target;
    cordinateArr moves;
    DxDy dxdy;

    MoveGeneration(Board &b) : board(b) {};

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

    void setPossibleMove()
    {
        moves.push_back({tempCord[0], tempCord[1]});
    }

    bool checkCell()
    {

        if (boardValue() == 0)
        {
            setPossibleMove();
            return true;
        }
        else if (boardValue() > 6 - target && boardValue() <= 12 - target)
        {
            setPossibleMove();
        }
        return false;
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
            setPossibleMove();

            if (y == startingYValue)
            {
                tempCord[0] = x;
                tempCord[1] = y + 2 * direction;
                if (boundCheck() && boardValue() == 0)
                    setPossibleMove();
            }
        }

        tempCord[0] = x + 1;
        tempCord[1] = y + direction;
        if (boundCheck() && boardValue() > 6 - target && boardValue() <= 12 - target)
            setPossibleMove();

        tempCord[0] = x - 1;
        tempCord[1] = y + direction;
        if (boundCheck() && boardValue() > 6 - target && boardValue() <= 12 - target)
            setPossibleMove();
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
                    if (!checkCell())
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
        int i;

        for (i = 0; i < 8; i++)
        {
            tempCord[0] = x + dxdy.knightDx[i];
            tempCord[1] = y + dxdy.knightDy[i];
            if (boundCheck())
            {
                checkCell();
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
                    if (!checkCell())
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
        int i;

        for (i = 0; i < 8; i++)
        {
            tempCord[0] = x + dxdy.kingDx[i];
            tempCord[1] = y + dxdy.kingDy[i];
            if (boundCheck())
            {
                checkCell();
            }
        }
    }

    cordinateArr generateMoves(int pieceX, int pieceY)
    {
        setTarget();
        moves.clear();

        int pieceCode = board.arr[pieceY][pieceX];

        if (pieceCode > 6)
        {
            pieceCode = pieceCode - 6;
        }

        switch (pieceCode)
        {
        case pieces::pawn:
            pawn(pieceX, pieceY);
            break;
        case pieces::rook:
            rook(pieceX, pieceY);
            break;
        case pieces::knight:
            knight(pieceX, pieceY);
            break;
        case pieces::bishop:
            bishop(pieceX, pieceY);
            break;
        case pieces::queen:
            queen(pieceX, pieceY);
            break;
        case pieces::king:
            king(pieceX, pieceY);
            break;

        default:
            break;
        }

        return moves;
    }
};

class KingSafety
{
public:
    int arrCpy[8][8];
    int simulatedTempCord[2];
    Board &board;
    MoveGeneration moveGeneration;
    int kingX;
    int kingY;
    cordinateArr legalMoves;
    int target;

    KingSafety(Board &b) : board(b), moveGeneration(b) {}

    void setTarget()
    {
        target = 0;

        if (board.currentTurn)
        {
            // blacks turn;
            target = 6;
        }
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

    void simulatedKingXY()
    {
        int i, j;
        for (i = 0; i < 8; i++)
        {

            for (j = 0; j < 8; j++)
            {
                if (arrCpy[i][j] == 6 + target)
                {
                    kingX = j;
                    kingY = i;
                    return;
                }
            }
        }
    }

    bool simulatedKingInCheck(int oldX, int oldY, int x, int y)
    {

        int i, j;
        DxDy dxdy;

        // simulating move in arrCpy;
        copyArr();
        arrCpy[y][x] = arrCpy[oldY][oldX];
        arrCpy[oldY][oldX] = 0;
        simulatedKingXY();

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

            for (i = 0; i < 8; i++)
            {
                simulatedTempCord[0] = kingX + dxdy.kingDx[i];
                simulatedTempCord[1] = kingY + dxdy.kingDy[i];
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

            for (i = 0; i < 8; i++)
            {
                simulatedTempCord[0] = kingX + dxdy.knightDx[i];
                simulatedTempCord[1] = kingY + dxdy.knightDy[i];
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
            int direction = 1;

            if (board.currentTurn)
            {
                direction = -1;
            }
            for (i = 0; i < 2; i++)
            {

                simulatedTempCord[0] = kingX + dxdy.pawnDx[i];
                simulatedTempCord[1] = kingY + direction;

                if (simulatedBoundCheck())
                {
                    if (simulatedBoardValue() == 7 - target)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    cordinateArr getLegalMoves(int pieceX, int pieceY)
    {

        int i, j;

        legalMoves.clear();

        setTarget();

        const cordinateArr possibleMoves = moveGeneration.generateMoves(pieceX, pieceY);

        for (i = 0; i < possibleMoves.size(); i++)
        {
            if (!simulatedKingInCheck(pieceX, pieceY, possibleMoves[i][0], possibleMoves[i][1]))
            {
                legalMoves.push_back({possibleMoves[i][0], possibleMoves[i][1]});
            }
        }

        return legalMoves;
    }
};

class MoveValidation
{
public:
    Board &board;
    KingSafety kingSafety;
    cordinateArr legalMoves;
    int fromX;
    int fromY;
    MoveValidation(Board &b) : board(b), kingSafety(b) {}

    cordinateArr getLegalMoves(int pieceX, int pieceY)
    {
        fromX = pieceX;
        fromY = pieceY;
        legalMoves = kingSafety.getLegalMoves(fromX, fromY);
        return legalMoves;
    }

    bool validate(int toX, int toY)
    {
        int i;

        for (i = 0; i < legalMoves.size(); i++)
        {
            if (legalMoves[i][0] == toX && legalMoves[i][1] == toY)
            {
                return true;
            }
        }

        return false;
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
        board.legalMoves.clear();
        if (moveValidation.validate(moveX, moveY))
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
                else
                {
                    board.legalMoves = moveValidation.getLegalMoves(currentX, currentY);

                    if (board.legalMoves.empty())
                    {
                        cursor.enterCounter = 0;
                    }
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
    std::cout << "\033[?25l";
    Game g;
    g.run();

    return 0;
}
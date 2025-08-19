#include <iostream>
#include <vector>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

#define RESET "\033[0m"    // Reset Ansi Sequence
#define BLUE "\033[34m"    // Blue color
#define WHITE "\033[1;97m" // Bright white (solid)
#define BLACK "\033[0;37m" // Dim gray (transparent)

struct cords
{
    int x;
    int y;
};

class Cursor
{
public:
    int xCord, yCord, enterCounter;
    bool counterTrigger;
    bool gamestatus = false;

public:
    Cursor() : xCord(4), yCord(5), enterCounter(0), counterTrigger(false), gamestatus(false) {}

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
        case 'l':
            gamestatus = true;
            break;
        default:
            break;
        }
    }
};

class LimitedCursor
{
public:
    int xCord, yCord;
    bool isWhite;
    int minY, maxY;
    int size;

public:
    void setPosition(int x, int y)
    {

        if (y == 0)
        {
            minY = 0;
            maxY = 3;
        }
        else
        {
            maxY = 7;
            minY = 4;
        }
        xCord = x;
        yCord = minY;
    }

    void up()
    {
        if (yCord < maxY)
        {
            yCord++;
        }
    }
    void down()
    {
        if (yCord > minY)
        {
            yCord--;
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
        default:
            break;
        }
    }
};

using cordinateArr = std::vector<std::vector<int>>;

struct displayutility
{

    const std::string message[5] = {"\n       White's Turn\n ", "\n       Black's Turn\n ", "\n       White Wins\n ", "\n       Black Wins\n ", "\n       DRAW\n "};

    const std::string p[13] = {" ", "♟", "♜", "♞", "♝", "♛", "♚", "♟", "♜", "♞", "♝", "♛", "♚"};

 const std::string bgColors[5][7] = {
    {
     "\033[47m", "\033[42m", "\033[106m", "\033[103m", "\033[48;5;210m", "\033[48;5;210m", "\033[48;5;214m"},
    {
     "\033[48;2;240;217;181m", "\033[48;2;181;136;99m", "\033[48;2;150;200;180m",
     "\033[48;2;255;210;100m", "\033[48;5;210m", "\033[48;5;210m", "\033[48;2;255;200;150m"},
    {
     "\033[48;2;200;200;255m", "\033[48;2;150;150;250m", "\033[48;2;220;180;255m",
     "\033[48;2;180;220;255m", "\033[48;2;255;150;200m", "\033[48;2;255;200;220m", "\033[48;2;100;150;255m"},
    {
     "\033[48;2;220;255;220m", "\033[48;2;100;200;100m", "\033[48;2;180;255;180m",
     "\033[48;2;150;255;150m", "\033[48;2;255;180;180m", "\033[48;2;200;255;200m", "\033[48;2;180;255;150m"},
    {
     "\033[48;2;255;230;180m", "\033[48;2;0;150;150m", "\033[48;2;255;200;150m",
     "\033[48;2;150;255;250m", "\033[48;2;255;150;100m", "\033[48;2;255;210;180m", "\033[48;2;255;200;50m"}
};

const std::string whitePieceTextColor[5] = {
    "\033[38;2;180;100;0m",
    "\033[38;2;180;100;0m",
    "\033[38;2;255;255;255m",
    "\033[38;2;0;150;0m",
    "\033[38;2;255;140;0m"
};

const std::string blackPieceTextColor[5] = {
    "\033[30m", "\033[30m", "\033[30m", "\033[30m", "\033[30m"
};

const std::string emptyTextColor[5] = {
    "\033[30m", "\033[30m", "\033[30m", "\033[30m", "\033[30m"
};


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
    bool isKinginCheck;
    bool isCheckMate;
    bool isDraw;
    bool moved;
    bool pawnPromote;
    int kingX, kingY;
    bool castling;
    int theme;
    int totalTheme;

    Board(Cursor &c) : cursor(c), currentTurn(0), isKinginCheck(false), isCheckMate(false), isDraw(false), moved(false), pawnPromote(false), castling(false), theme(0), totalTheme(5) {}

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

        if (castling)
        {
            castling = false;

            if (oldX - x == 2)
            {
                // queen side
                arr[y][x + 1] = arr[y][0];
                arr[y][0] = 0;
            }
            else
            {
                // king side
                arr[y][x - 1] = arr[y][7];
                arr[y][7] = 0;
            }
        }

        moved = true;
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
                std::string bgColor;
                std::string textColor;

                int colorIndex;

                if (isCursor)
                {
                    colorIndex = 2;
                }
                else if (isLegalSquare)
                {
                    colorIndex = 3;
                }
                else if (isKinginCheck && kingX == j && kingY == i)
                {
                    colorIndex = 4;
                }
                else if (isLightSquare)
                {
                    colorIndex = 0;
                }
                else
                {
                    colorIndex = 1;
                }

                bgColor = utility.bgColors[theme][colorIndex];

                if (val == 0)
                    textColor = utility.emptyTextColor[theme];
                else if (val <= 6)
                    textColor = utility.whitePieceTextColor[theme];
                else
                    textColor = utility.blackPieceTextColor[theme];

                std::cout << bgColor << textColor << " " << utility.p[val] << " " << RESET;
            }
            std::cout << "\n";
        }

        // Player turn info
        if (isCheckMate)
        {
            currentTurn = !currentTurn;
            currentTurn = currentTurn + 2;
        }

        else if (isDraw)
        {
            currentTurn = 4;
        }

        std::cout << BLUE << utility.message[currentTurn] << RESET;

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

class sounds
{
public:
    void start()
    {
        system("aplay sounds/chess_com_start_game.wav > /dev/null 2>&1 &");
    }
    void move()
    {
        system("aplay sounds/chess_com_move_piece.wav > /dev/null 2>&1 &");
    }
    void check()
    {
        system("aplay sounds/chess_com_check.wav > /dev/null 2>&1 &");
    }
    void checkmate()
    {
        system("aplay sounds/chess_com_checkmate.wav > /dev/null 2>&1 &");
    }
    void stalemate()
    {
        system("aplay sounds/chess_com_stalemate.wav > /dev/null 2>&1 &");
    }
    void gameOver()
    {
        system("aplay sounds/chess_com_game_over.wav > /dev/null 2>&1 &");
    }

    void manageSound(Board &board)
    {
        if (board.moved)
        {
            if (board.isCheckMate)
            {
                checkmate();
            }
            else if (board.isKinginCheck)
            {
                check();
            }
            else if (board.isDraw)
            {
                stalemate();
            }
            else
            {
                move();
            }
            board.moved = false;
        }
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
    bool moveSelected;
    bool pieceSelected;

public:
    InputHandling(Cursor &c, Board &b) : cursor(c), board(b), val(0), moveX(0), moveY(0), currentX(0), currentY(0), moveSelected(false) {}

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

    bool selectMove()
    {
        moveX = cursor.xCord;
        moveY = cursor.yCord;
        board.legalMoves.clear();
        return true;
    }

    void manageInput()
    {
        moveSelected = false;
        pieceSelected = false;

        if (cursor.counterTrigger == true)
        {

            if (cursor.enterCounter == 1)
            {
                pieceSelected = selectPeice();
                if (pieceSelected == false)
                {
                    cursor.enterCounter = 0;
                }
            }
            else
            {
                moveSelected = selectMove();
                cursor.enterCounter = 0;
            }

            cursor.counterTrigger = false;
        }
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
    int setKing;
    cordinateArr moves;
    DxDy dxdy;
    bool kingRookMoved[2][3];

    MoveGeneration(Board &b) : board(b), kingRookMoved{{false, false, false}, {false, false, false}} {}

    void setTarget()
    {
        target = 0;
        setKing = 0;

        if (board.currentTurn)
        {
            // blacks turn;
            setKing = 1;
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

        // castling logic
        if (!kingRookMoved[setKing][0] && !board.isKinginCheck)
        {
            // check for king side rook moved x++
            if (!kingRookMoved[setKing][1] && board.arr[7 * (1 - setKing)][7] == 2 + target)
            {
                if (board.arr[y][x + 1] == 0 && board.arr[y][x + 2] == 0)
                {
                    tempCord[0] = x + 2;
                    tempCord[1] = y;
                    setPossibleMove();
                }
            }

            // chcek queen side rook moved x--
            if (!kingRookMoved[setKing][2] && board.arr[7 * (1 - setKing)][0] == 2 + target)
            {
                if (board.arr[y][x - 1] == 0 && board.arr[y][x - 2] == 0 && board.arr[y][x - 3] == 0)
                {
                    tempCord[0] = x - 2;
                    tempCord[1] = y;
                    setPossibleMove();
                }
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
    MoveGeneration &moveGeneration;
    cords king;
    cordinateArr legalMoves;
    int target;

    KingSafety(Board &b, MoveGeneration &m) : board(b), moveGeneration(m) {}

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
                    king.x = j;
                    king.y = i;
                    return;
                }
            }
        }
    }

    bool simulatedKingInCheck(int oldX, int oldY, int x, int y, bool justCheck = false)
    {

        int i, j;
        DxDy dxdy;

        // simulating move in arrCpy;
        copyArr();

        if (!justCheck)
        {
            arrCpy[y][x] = arrCpy[oldY][oldX];
            arrCpy[oldY][oldX] = 0;
        }
        else
        {
            setTarget();
        }

        simulatedKingXY();

        // check for rook and queen
        for (i = 0; i < 4; i++)
        {
            for (j = 1; j < 9; j++)
            {
                switch (i)
                {
                case 0:
                    simulatedTempCord[0] = king.x;
                    simulatedTempCord[1] = king.y + j;
                    break;
                case 1:
                    simulatedTempCord[0] = king.x;
                    simulatedTempCord[1] = king.y - j;
                    break;
                case 2:
                    simulatedTempCord[0] = king.x + j;
                    simulatedTempCord[1] = king.y;
                    break;
                case 3:
                    simulatedTempCord[0] = king.x - j;
                    simulatedTempCord[1] = king.y;
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
                    simulatedTempCord[0] = king.x + j;
                    simulatedTempCord[1] = king.y - j;
                    break;
                case 1:
                    simulatedTempCord[0] = king.x - j;
                    simulatedTempCord[1] = king.y + j;
                    break;
                case 2:
                    simulatedTempCord[0] = king.x - j;
                    simulatedTempCord[1] = king.y - j;
                    break;
                case 3:
                    simulatedTempCord[0] = king.x + j;
                    simulatedTempCord[1] = king.y + j;
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
                simulatedTempCord[0] = king.x + dxdy.kingDx[i];
                simulatedTempCord[1] = king.y + dxdy.kingDy[i];
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
                simulatedTempCord[0] = king.x + dxdy.knightDx[i];
                simulatedTempCord[1] = king.y + dxdy.knightDy[i];
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
            int direction = -1;

            if (board.currentTurn)
            {
                direction = 1;
            }
            for (i = 0; i < 2; i++)
            {

                simulatedTempCord[0] = king.x + dxdy.pawnDx[i];
                simulatedTempCord[1] = king.y + direction;

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
            if ((board.arr[pieceY][pieceX] == 6 + target) && (possibleMoves[i][0] - pieceX == 2 || possibleMoves[i][0] - pieceX == -2))
            {
                bool val1, val2;
                if (pieceX - possibleMoves[i][0] == 2)
                {
                    val1 = simulatedKingInCheck(pieceX, pieceY, pieceX - 1, possibleMoves[i][1]);
                    val2 = simulatedKingInCheck(pieceX, pieceY, pieceX - 2, possibleMoves[i][1]);
                }
                else
                {
                    val1 = simulatedKingInCheck(pieceX, pieceY, pieceX + 1, possibleMoves[i][1]);
                    val2 = simulatedKingInCheck(pieceX, pieceY, pieceX + 2, possibleMoves[i][1]);
                }

                if (!(val1 || val2))
                {
                    legalMoves.push_back({possibleMoves[i][0], possibleMoves[i][1]});
                }
            }
            else
            {

                if (!simulatedKingInCheck(pieceX, pieceY, possibleMoves[i][0], possibleMoves[i][1]))
                {

                    legalMoves.push_back({possibleMoves[i][0], possibleMoves[i][1]});
                }
            }
        }

        return legalMoves;
    }

    void kingCordinate(int &x, int &y)
    {

        simulatedKingXY();
        x = king.x;
        y = king.y;
    }
};

class MoveValidation
{
public:
    Board &board;
    MoveGeneration &moveGeneration;
    KingSafety &kingSafety;
    cordinateArr legalMoves;
    cordinateArr piecesPosition;
    cordinateArr checking;
    int fromX; // current x
    int fromY; // cureent y
    int kingX;
    int kingY;

    MoveValidation(Board &b, MoveGeneration &m, KingSafety &king) : board(b), moveGeneration(m), kingSafety(king) {}

    // this is called when a piece is selected
    cordinateArr getLegalMoves(int pieceX, int pieceY)
    {
        fromX = pieceX;
        fromY = pieceY;
        legalMoves = kingSafety.getLegalMoves(fromX, fromY);
        return legalMoves;
    }

    void detectPromotion(int toX, int toY)
    {
        if (board.arr[fromY][fromX] == 1 || board.arr[fromY][fromX] == 7)
        {
            if (toY == 0 || toY == 7)
            {
                board.pawnPromote = true;
            }
        }
    }

    bool detectCastling(int toX, int toY)
    {

        if (board.arr[fromY][fromX] == 6 || board.arr[fromY][fromX] == 12)
        {
            if (toX - fromX == 2 || toX - fromX == -2)
            {
                return true;
            }
        }

        return false;
    }

    void updateCastlingCondition(int toX, int toY)
    {
        if (board.arr[fromY][fromX] == 6)
        {
            // white king
            moveGeneration.kingRookMoved[0][0] = true;
        }
        else if (board.arr[fromY][fromX] == 12)
        {
            // black king
            moveGeneration.kingRookMoved[1][0] = true;
        }

        if (board.arr[fromY][fromX] == 2)
        {
            if (fromX == 0)
            {
                moveGeneration.kingRookMoved[0][2] = true;
            }
            else if(fromX == 7)
            {
                moveGeneration.kingRookMoved[0][1] = true;
            }
        }
        else if (board.arr[fromY][fromX] == 8)
        {
            if (fromX == 0)
            {
                moveGeneration.kingRookMoved[1][2] = true;
            }
            else if(fromX == 7)
            {
                moveGeneration.kingRookMoved[1][1] = true;
            }
        }
    }

    bool validate(int toX, int toY)
    {
        int i;

        for (i = 0; i < legalMoves.size(); i++)
        {
            if (legalMoves[i][0] == toX && legalMoves[i][1] == toY)
            {

                updateCastlingCondition(toX, toY);
                detectPromotion(toX, toY);
                board.castling = detectCastling(toX, toY);
                return true;
            }
        }

        return false;
    }

    bool isCurrentKingInCheck()
    {
        return kingSafety.simulatedKingInCheck(0, 0, 0, 0, true);
    }

    void getPieces()
    {
        piecesPosition.clear();
        int i, j;

        int target = 6;
        if (board.currentTurn == 1)
        {
            target = 0;
        }

        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                if (board.arr[i][j] > 6 - target && board.arr[i][j] <= 12 - target)
                {
                    piecesPosition.push_back({j, i});
                    std::cout << j << " " << i;
                }
            }
        }
    }

    bool isMoveAvaliable()
    {
        int i;
        getPieces();
        for (i = 0; i < piecesPosition.size(); i++)
        {
            checking.clear();
            checking = getLegalMoves(piecesPosition[i][0], piecesPosition[i][1]);
            if (!checking.empty())
            {
                return true;
            }
        }
        return false;
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

class GameManager
{
private:
    Cursor cursor;
    LimitedCursor limitedCursor;
    Board board;
    InputHandling inputHandling;
    MoveGeneration moveGeneration;
    KingSafety kingsafety;
    MoveValidation moveValidation;
    sounds sound;

    char ch;

public:
    bool gameStatus;

    GameManager() : cursor(), limitedCursor(), board(cursor), inputHandling(cursor, board), moveGeneration(board), kingsafety(board, moveGeneration), moveValidation(board, moveGeneration, kingsafety), sound(), gameStatus(true) {}

    void kingIncheck()
    {
        board.isKinginCheck = moveValidation.isCurrentKingInCheck();
        if (board.isKinginCheck)
        {
            moveValidation.kingSafety.kingCordinate(board.kingX, board.kingY);
        }
    }

    void pawnPromotionDisplay()
    {
        int theme = board.theme;
        std::cout << "\033c"; // Clear screen

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {

                int val = board.arr[i][j];
                bool isCursor = (limitedCursor.xCord == j && limitedCursor.yCord == i);
                bool isLightSquare = ((i + j) % 2 == 0);
                std::string bgColor;
                std::string textColor;
                std::string pieces[4] = {"♛", "♜", "♞", "♝"};

                int colorIndex;

                if (isLightSquare)
                {
                    colorIndex = 0;
                }
                else
                {
                    colorIndex = 1;
                }

                // if i in promotion menu
                if (j == limitedCursor.xCord && i >= limitedCursor.minY && i <= limitedCursor.maxY)
                {

                    if (isCursor)
                    {
                        colorIndex = 2;
                    }
                    else
                    {
                        colorIndex = 6;
                    }

                    bgColor = board.utility.bgColors[theme][colorIndex];

                    // since current trun is changed hence this reverse check
                    if (board.currentTurn == 0)
                    {
                        textColor = board.utility.blackPieceTextColor[theme];
                    }
                    else
                    {
                        textColor = board.utility.whitePieceTextColor[theme];
                    }

                    std::cout << bgColor << textColor << " " << pieces[i - limitedCursor.minY] << " " << RESET;
                }
                else
                {

                    bgColor = board.utility.bgColors[theme][colorIndex];

                    if (val == 0)
                        textColor = board.utility.emptyTextColor[theme];
                    else if (val <= 6)
                        textColor = board.utility.whitePieceTextColor[theme];
                    else
                        textColor = board.utility.blackPieceTextColor[theme];

                    std::cout << bgColor << textColor << " " << board.utility.p[val] << " " << RESET;
                }
            }
            std::cout << "\n";
        }

        // Player turn info

        std::cout << BLUE << board.utility.message[board.currentTurn] << RESET;

        // Captured pieces
        std::cout << "\n";

        std::cout << WHITE << " White Captures: " << RESET;
        for (int i = 0; i < board.whiteKilled.size(); i++)
            std::cout << WHITE << board.utility.p[board.whiteKilled.at(i)] << " ";
        std::cout << "\n";

        std::cout << BLACK << " Black Captures: " << RESET;
        for (int i = 0; i < board.blackKilled.size(); i++)
            std::cout << BLACK << board.utility.p[board.blackKilled.at(i)] << " ";
        std::cout << "\n"
                  << RESET << "\n";
    }

    void promotion(int toX, int toY)
    {
        if (board.pawnPromote)
        {
            int target = board.currentTurn ? 0 : 6;
            char ch;
            limitedCursor.setPosition(toX, toY);

            while (true)
            {
                pawnPromotionDisplay();
                ch = getch();
                limitedCursor.updateCursor(ch);
                if (ch == '\n')
                {
                    int diffrence = limitedCursor.yCord - limitedCursor.minY;
                    int piecesArr[] = {5, 2, 3, 4};
                    board.arr[toY][toX] = piecesArr[diffrence] + target;
                    board.pawnPromote = false;
                    break;
                }
            }
        }
    }

    void handleSelection()
    {

        if (inputHandling.pieceSelected)
        {
            board.legalMoves = moveValidation.getLegalMoves(inputHandling.currentX, inputHandling.currentY);

            if (board.legalMoves.empty())
            {
                cursor.enterCounter = 0;
            }
        }

        if (inputHandling.moveSelected)
        {

            if (moveValidation.validate(inputHandling.moveX, inputHandling.moveY))
            {
                board.isKinginCheck = false;
                board.moveFromTo(inputHandling.currentX, inputHandling.currentY, inputHandling.moveX, inputHandling.moveY);

                // cureent positon in updated from MoveFromTo function;

                promotion(inputHandling.moveX, inputHandling.moveY);

                kingIncheck();

                if (board.isKinginCheck)
                {
                    board.isCheckMate = !moveValidation.isMoveAvaliable();
                }
                else
                {
                    board.isDraw = !moveValidation.isMoveAvaliable();
                    if (!board.isDraw)
                    {
                        if (board.whiteKilled.size() == 15 && board.blackKilled.size() == 15)
                        {
                            board.isDraw = true;
                        }
                    }
                }
            }
        }
    }

    void updateGameStatus()
    {
        if (board.isCheckMate || board.isDraw || cursor.gamestatus)
        {
            gameStatus = false;
            sound.gameOver();
        }
    }

    void getTheme()
    {
        std::ifstream inFile("files/theme.txt");
        if (!inFile)
        {
            std::cerr << "Error opening file for reading";
        }
        inFile >> board.theme;
        inFile.close();
    }

    void run()
    {
        getTheme();
        board.initialize();

        sound.start();
        while (gameStatus)
        {
            board.display();
            ch = getch();
            cursor.updateCursor(ch);
            inputHandling.manageInput();
            handleSelection();
            sound.manageSound(board);
            updateGameStatus();
        }
        board.display();
        std::cout << "Game Ended" << std::endl;
        getch();
    }

    void displayInstructions()
    {
        system("clear");
        std::ifstream file("files/instructions.txt");
        if (!file)
        {
            std::cerr << "Error: Could not open instructions.txt\n";
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::cout << line << "\n";
        }

        file.close();
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

    void saveTheme()
    {
        std::ofstream outFile("files/theme.txt");
        if (!outFile)
        {
            std::cerr << "Error opening file for writing\n";
            return;
        }
        outFile << board.theme;
        outFile.close();
    }

    void customizeBoard()
    {
        board.initialize();
        char select;

        while (true)
        {
            select = getch();
            if (select == 'a')
            {
                if (board.theme > 0)
                {
                    board.theme--;
                }
            }
            else if (select == 'd')
            {
                if (board.theme < board.totalTheme - 1)
                {
                    board.theme++;
                }
            }
            else if (select == '\n')
            {
                saveTheme();
                std::cout << "Board selectd press any key to exit" << std::endl;
                getch();
                break;
            }
            board.display();
        }
    }
};

void startMenuSong() {
    system("nohup aplay -q sounds/menusong.wav >/dev/null 2>&1 & echo $! > /tmp/menusong.pid");
}

void endMenuSong() {
    system("pkill -f menusong.wav >/dev/null 2>&1");
}

int main() {
    GameManager g;
    char cha;

    startMenuSong(); // Start menu music once

    while (true) {
        system("clear");
        std::cout << "\n\t WELCOME TO CHESS" << std::endl;
        std::cout << "1: Start" << std::endl;
        std::cout << "2: Instructions" << std::endl;
        std::cout << "3: Customize Board" << std::endl;
        std::cout << "4: Exit" << std::endl;

        cha = getch();

        switch (cha) {
        case '1':
            endMenuSong();   
            {
                GameManager game;
                game.run();
            }
            startMenuSong(); 
            break;

        case '2':
            g.displayInstructions();
            break;

        case '3':
            g.customizeBoard();
            break;

        case '4':
            endMenuSong(); 
            return 0;

        default:
            break;
        }
    }

    endMenuSong();
    return 0;
}
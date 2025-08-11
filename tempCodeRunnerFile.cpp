#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

#define RESET "\033[0m"
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

public:
    Cursor() : xCord(4), yCord(5), enterCounter(0), counterTrigger(false) {}

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

    const std::string message[5] = {"\n       White's Turn\n ", "\n       Black's Turn\n ", "\n       White Wins\n ", "\n       Black Wins\n ", "\n       DRAW\n "};

    const std::string p[13] = {" ", "♟", "♜", "♞", "♝", "♛", "♚", "♟", "♜", "♞", "♝", "♛", "♚"};

    const std::string bgColors[2][5] = {
        {"\033[47m",
         "\033[42m",
         "\033[106m",
         "\033[103m",
         "\033[48;5;210m"},

        {"\033[48;2;240;217;181m",
         "\033[48;2;181;136;99m",
         "\033[48;2;150;200;180m",
         "\033[48;2;255;210;100m",
         "\033[48;5;210m"}};

    const std::string whitePieceTextColor[2] = {
        {

            //"\033[38;2;60;90;130m"
            "\033[38;2;180;100;0m"
            //"\033[38;2;242;242;242m"
        },
        {"\033[38;2;180;100;0m"}

    };

    const std::string blackPieceTextColor[2] = {
        {"\033[30m"}, {"\033[38;2;0;0;0m"}

    };

    const std::string emptyTextColor[2] = {
        {"\033[30m"}, {"\033[30m"}};
};

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

        sound.move();

              if(isKinginCheck){
                sound.check();
              }  
              else if(isCheckMate){
                sound.checkmate();
              }
              else if(isDraw)
              {
                sound.stalemate();
              }
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
        int theme = 0;
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
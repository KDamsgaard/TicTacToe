//TODO let class ComputerPlayer inherit from class Player.
//(Having trouble with converting from Player to ComputerPlayer in game:m_players[])

//Logic for controlling the computer player.
//Works by subdividing the gameboard into segments. Each segment corresponds to a potentially 
//winning combination (lines) or a trap (corners) - by, f.ex. setting tile 2, then tile 4 and 
//then tile 1 two lines are open to a win, which the computer would not be able to see without 
//checking the corners.
//
//Precondition: A pointer to the board array, contained in the game class.
class ComputerPlayer
{
    char *m_board_ptr;
    char m_board[10];
    
    char m_corner1[3];
    char m_corner2[3];
    char m_corner3[3];
    char m_corner4[3];
    char m_line1[3];
    char m_line2[3];
    char m_line3[3];
    char m_line4[3];
    char m_line5[3];
    char m_line6[3];
    char m_line7[3];
    char m_line8[3];

    int m_bestMove;
    // bool m_decided = false;

    // Lines diagram:
    // 1, 2 and 3 are the horizontal lines, 4, 5 and 6 are the vertical lines, 
    // 7 and 8 are the diagonals.
    //
    // 7     8
    //  _|_|_ 1
    //  _|_|_ 2
    //   | |  3
    //  4 5 6

public:
    ComputerPlayer(char *board_ptr)
    : m_board_ptr(board_ptr)
    {
        //Extract array
        for (int i = 0; i < 10; i++)
        {
            m_board[i] = *m_board_ptr;
            m_board_ptr++;            
        }
                
        //Set corners
        m_corner1[0] = m_board[1];
        m_corner1[1] = m_board[2];
        m_corner1[2] = m_board[4];

        m_corner2[0] = m_board[3];
        m_corner2[1] = m_board[2];
        m_corner2[2] = m_board[6];

        m_corner3[0] = m_board[7];
        m_corner3[1] = m_board[4];
        m_corner3[2] = m_board[8];

        m_corner4[0] = m_board[9];
        m_corner4[1] = m_board[6];
        m_corner4[2] = m_board[8];

        //Set lines
        m_line1[0] = m_board[1];
        m_line1[1] = m_board[2];
        m_line1[2] = m_board[3];

        m_line2[0] = m_board[4];
        m_line2[1] = m_board[5];
        m_line2[2] = m_board[6];

        m_line3[0] = m_board[7];
        m_line3[1] = m_board[8];
        m_line3[2] = m_board[9];

        m_line4[0] = m_board[1];
        m_line4[1] = m_board[4];
        m_line4[2] = m_board[7];

        m_line5[0] = m_board[2];
        m_line5[1] = m_board[5];
        m_line5[2] = m_board[8];

        m_line6[0] = m_board[3];
        m_line6[1] = m_board[6];
        m_line6[2] = m_board[9];

        m_line7[0] = m_board[1];
        m_line7[1] = m_board[5];
        m_line7[2] = m_board[9];

        m_line8[0] = m_board[3];
        m_line8[1] = m_board[5];
        m_line8[2] = m_board[7];

        //The order of calling defaultAction(). checkCorner() and checkLine() is strict - reordering these 
        //calls will result in logic errors

        //Call default action to set a move
        defaultAction();

        //Calculate if set move needs to be reset
        checkCorner(m_corner1);
        checkCorner(m_corner2);
        checkCorner(m_corner3);
        checkCorner(m_corner4);

        checkLine(m_line1);
        checkLine(m_line2);
        checkLine(m_line3);
        checkLine(m_line4);
        checkLine(m_line5);
        checkLine(m_line6);
        checkLine(m_line7);
        checkLine(m_line8);
    }

    //Sets m_bestMove to a default value (which is overwritten if checkLine() or chechCorner() finds 
    //a better move).
    //This works because trapping by corner takes more moves.
    void defaultAction()
    {     
        //pick random tile
        while (true)
        {
            int random = rand() %9 + 1;
            if(m_board[random] == (random + 48) )
            {
                m_bestMove = random;
                break;
            }
            //TESTING
            cout << "randomizer sets: " << random << endl;
        }

        //Try to get middle tile
        if (m_board[5] == '5')
        {
            m_bestMove = 5;
        }

        //if computer has middle tile, go for win by starting a straight line
        if (m_board[5] == 'C')
        {
            if(m_board[2] == '2')
            {
                m_bestMove = 2;
            }
            else if(m_board[4] == '4')
            {
                m_bestMove = 4;
            }
            else if(m_board[6] == '6')
            {
                m_bestMove = 6;
            }
            else if(m_board[8] == '8')
            {
                m_bestMove = 8;
            }
        }
        //if player has middle tile, block by setting a corner
        if (m_board[5] == 'X')
        {
            if(m_board[1] == '1')
            {
                m_bestMove = 1;
            }
            else if(m_board[3] == '3')
            {
                m_bestMove = 3;
            }
            else if(m_board[7] == '7')
            {
                m_bestMove = 7;
            }
            else if(m_board[9] == '9')
            {
                m_bestMove = 9;
            }
        }
    }

    //Sets m_bestMove if other player has set the surrounding tiles of a corner and surrounded tile is 
    //not set (f.ex. if other player has set tiles 2 and 4 and tile 1 is not set by any player)
    void checkCorner(char corner[])
    {
        if (corner[1] == 'X' && corner[2] == 'X' && corner[0] != 'X' && corner[0] != 'C')
        {
            m_bestMove = (corner[0] -48); //Subtracting 48 converts from ASCII value to integer
        }
    }

    //Checks a line for 'X' and 'O' values and decides if computer can win or is threatened (IE: can lose)
    //and sets m_bestMove as third tile of line, assuming tile is not set.
    void checkLine(char line[])
    {
        //Set a counter
        int counter = 0;

        //For tiles in line, count up for O and down for X
        for (int i = 0; i < 3; i++)
        {
            if (line[i] == 'X')
            {
                counter--;
            }
            if (line[i] == 'C')
            {
                counter++;
            }
        }

        //If counter is higher than 1 it means that the line holds two O's and no X's.
        //If counter is lower than -1 it means that the line holds two X's and no O's
        if (counter > 1 || counter < -1)
        {
            //find empty tile
            for (int i = 0; i < 3; i++)
            {
                if (line[i] != 'C' && line[i] != 'X')
                {
                    //set best move as empty tile in line
                    m_bestMove = (line[i] - 48); //Subtracting 48 converts from ASCII value to integer
                }
            }
        }
    }


/////   COMPUTERPLAYER GETTERS     ///////////////////////////////////////////////////////////////////////

    //Simple getter.
    //Return: integer, best move as calculated by computer.
    int getBestMove()
    {
        return m_bestMove;
    }
};//END CLASS -------------------------------------------------------------------------------------------

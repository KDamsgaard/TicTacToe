//Tic Tac Toe
//Kristian K. Damsgaard, 2019

//With quite a lot more classes than are needed!

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define PLATFORM "windows"
#else
    #define PLATFORM "other"
#endif

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <map>

using namespace std;

//Simple class that holds a letter used to identify tiles chosen by said player and wether or not the 
//player is controlled by the computer.
class Player
{
    friend class Game; //Not optimal. Refrain from using friend classes in future.

    char m_letter;
    bool m_isComputer;

    //Needed for reasons I do not quite understand. If missing Game() reports "no default constructor 
    //for Player".
    Player()
    {
    }

    Player(char letter, bool isComputer)
    : m_letter(letter), m_isComputer(isComputer)
    {
    }

public:

////    PLAYER GETTERS      /////////////////////////////////////////////////////////////////////////////
    
    char getLetter()
    {
        return m_letter;
    }

    bool isComputer()
    {
        return m_isComputer;
    }
};//END CLASS -------------------------------------------------------------------------------------------


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
    int m_turn;
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


    int m_bestMoveFrom = 0;
    int m_bestMoveTo = 0;
    
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
    ComputerPlayer(int turn, char *board_ptr)
    : m_turn(turn), m_board_ptr(board_ptr)
    {
        //Extract array
        for (int i = 0; i < 10; i++)
        {
            m_board[i] = *m_board_ptr;
            m_board_ptr++;            
        }

        //TESTING
        for (int i = 1; i < 10; i++)
        {
            cout << m_board[i] << " ";
        }
        cout << endl;

        //Set lines
        //Due to the mistake of not storing position AND value in the game board, the lines hold their line 
        //number at index 3.
        m_line1[0] = m_board[1];
        m_line1[1] = m_board[2];
        m_line1[2] = m_board[3];
        m_line1[3] = '1';

        m_line2[0] = m_board[4];
        m_line2[1] = m_board[5];
        m_line2[2] = m_board[6];
        m_line1[3] = '2';

        m_line3[0] = m_board[7];
        m_line3[1] = m_board[8];
        m_line3[2] = m_board[9];
        m_line1[3] = '3';

        m_line4[0] = m_board[1];
        m_line4[1] = m_board[4];
        m_line4[2] = m_board[7];
        m_line1[3] = '4';

        m_line5[0] = m_board[2];
        m_line5[1] = m_board[5];
        m_line5[2] = m_board[8];
        m_line1[3] = '5';

        m_line6[0] = m_board[3];
        m_line6[1] = m_board[6];
        m_line6[2] = m_board[9];
        m_line1[3] = '6';

        m_line7[0] = m_board[1];
        m_line7[1] = m_board[5];
        m_line7[2] = m_board[9];
        m_line1[3] = '7';

        m_line8[0] = m_board[3];
        m_line8[1] = m_board[5];
        m_line8[2] = m_board[7];
        m_line1[3] = '8';

        //The order of calling defaultAction(). checkThreatened() and CheckCanWin() is strict - reordering 
        //these calls will result in logic errors

        //Call default action to set a move
        defaultAction();

        //Calculate if set move needs to be reset

        calculateBestMoveFrom(m_line1);
        calculateBestMoveFrom(m_line2);
        calculateBestMoveFrom(m_line3);
        calculateBestMoveFrom(m_line4);
        calculateBestMoveFrom(m_line5);
        calculateBestMoveFrom(m_line6);
        calculateBestMoveFrom(m_line7);
        calculateBestMoveFrom(m_line8);

        checkThreatened(m_line1);
        checkThreatened(m_line2);
        checkThreatened(m_line3);
        checkThreatened(m_line4);
        checkThreatened(m_line5);
        checkThreatened(m_line6);
        checkThreatened(m_line7);
        checkThreatened(m_line8);

        checkCanWin(m_line1);
        checkCanWin(m_line2);
        checkCanWin(m_line3);
        checkCanWin(m_line4);
        checkCanWin(m_line5);
        checkCanWin(m_line6);
        checkCanWin(m_line7);
        checkCanWin(m_line8);
    }

    void defaultAction()
    {
        if (m_board[5] == '5')
        {
            m_bestMoveTo = 5;
        }
        else
        {
            int possible[4] = {2, 4, 6, 8};

            while(true){
                
                int random = rand()%4;
                int check = possible[random];

                if(m_board[check] == (check + 48) ) //Adding 48 converts from integer to ASCII value
                {
                    m_bestMoveTo = check;
                    break;
                }
            }
        }
    }

    //Attempt to find the best tile to move a piece from by running through
    //the eight lines and finding the lines that contain precisely O and 
    //less than two X's. Then get the tile number from the line array and
    //set a suitable bestMoveFrom.
    void calculateBestMoveFrom(char line[])
    {
        int countX = 0;
        int countC = 0;
        for (int i = 0; i < 3; i++)
        {
            if (line[i] == 'X')
            {
                countX++;
            }
            if (line[i] == 'C')
            {
                countC++;
            }
        }
        //TESTING
        cout << "improve countX: " << countX << endl;
        cout << "improve countC: " << countC << endl;
        // if (countC == 1 && countX == 0)
        // {
        //     for (int i = 0; i < 3; i++)
        //     {
        //         if (line[i] != 'C')
        //         {
        //             m_bestMoveTo = line[i] - 48;//Subtracting 48 converts from ASCII to integer
        //         }
        //     }
        // //TESTING
        // // cout << "improve moveTo: " << m_bestMoveTo << endl;
        // }

        if (countC == 1 && countX < 2)
        {
            //TESTING
            // cout << "Entering overly complicated if statement" << endl;
            int lineNumber = line[4];

            if (line[3] == 1+48)//Adding 48 converts from integer to ASCII value
            {
                //TESTING
                cout << "if line = 1" << endl;
                if (m_board[1] == 'C')
                {
                    m_bestMoveFrom = 1;
                }
                if (m_board[2] == 'C')
                {
                    m_bestMoveFrom = 2;
                }
                if (m_board[3] == 'C')
                {
                    m_bestMoveFrom = 3;
                }
            }
            if (line[3] == 2+48)
            {
                //TESTING
                cout << "if line = 2" << endl;
                if (m_board[4] == 'C')
                {
                    m_bestMoveFrom = 4;
                }
                if (m_board[5] == 'C')
                {
                    m_bestMoveFrom = 5;
                }
                if (m_board[6] == 'C')
                {
                    m_bestMoveFrom = 6;
                }
            }
            if (line[3] == 3+48)//Adding 48 converts from integer to ASCII value
            {
                //TESTING
                cout << "if line = 3" << endl;
                if (m_board[7] == 'C')
                {
                    m_bestMoveFrom = 7;
                }
                if (m_board[8] == 'C')
                {
                    m_bestMoveFrom = 8;
                }
                if (m_board[9] == 'C')
                {
                    m_bestMoveFrom = 9;
                }
            }
            if (line[3] == 4+48)//Adding 48 converts from integer to ASCII value
            {
                //TESTING
                cout << "if line = 4" << endl;
                if (m_board[1] == 'C')
                {
                    m_bestMoveFrom = 1;
                }
                if (m_board[4] == 'C')
                {
                    m_bestMoveFrom = 4;
                }
                if (m_board[7] == 'C')
                {
                    m_bestMoveFrom = 7;
                }
            }
            if (line[3] == 5+48)//Adding 48 converts from integer to ASCII value
            {
                //TESTING
                cout << "if line = 5" << endl;
                if (m_board[2] == 'C')
                {
                    m_bestMoveFrom = 2;
                }
                if (m_board[5] == 'C')
                {
                    m_bestMoveFrom = 5;
                }
                if (m_board[8] == 'C')
                {
                    m_bestMoveFrom = 8;
                }
            }
            if (line[3] == 6+48)//Adding 48 converts from integer to ASCII value
            {
                //TESTING
                cout << "if line = 6" << endl;
                if (m_board[3] == 'C')
                {
                    m_bestMoveFrom = 3;
                }
                if (m_board[6] == 'C')
                {
                    m_bestMoveFrom = 6;
                }
                if (m_board[9] == 'C')
                {
                    m_bestMoveFrom = 9;
                }
            }
            if (line[3] == 7+48)//Adding 48 converts from integer to ASCII value
            {
                //TESTING
                cout << "if line = 7" << endl;
                if (m_board[1] == 'C')
                {
                    m_bestMoveFrom = 1;
                }
                if (m_board[5] == 'C')
                {
                    m_bestMoveFrom = 5;
                }
                if (m_board[9] == 'C')
                {
                    m_bestMoveFrom = 9;
                }
            }
            if (line[3] == 8+48)//Adding 48 converts from integer to ASCII value
            {//TESTING
                cout << "if line = 8" << endl;
                if (m_board[3] == 'C')
                {
                    m_bestMoveFrom = 3;
                }
                if (m_board[5] == 'C')
                {
                    m_bestMoveFrom = 5;
                }
                if (m_board[7] == 'C')
                {
                    m_bestMoveFrom = 7;
                }
            }
        }
        //TESTING
        cout << "improve moveFrom: " << m_bestMoveFrom << endl;
    }

    //Checks a line for threats (IE: where player has 2 tiles)
    void checkThreatened(char line[])
    {
        int counter = 0;
        bool isThreatened = false;
        //Count X's on line
        for (int i = 0; i < 3; i++)
        {
            if (line[i] == 'X')
            {
                counter++;
            }
            if (line[i] == 'C')
            {
                counter--;
            }

        }
        //TESTING
        // cout << "isThreatened counter: " << counter << endl;
        //If two X's are found find out if last tile is empty and set empty tile as bestMoveTo
        if (counter > 1)
        {
            isThreatened = true;
            for (int i = 0; i < 3; i++)
            {
                if (line[i] != 'C' && line[i] != 'X')
                {
                    m_bestMoveTo = line[i] - 48;//Subtracting 48 converts from ASCII to integer
                }
            }
            //TESTING
            // cout << "isThreatened moveTo: " << m_bestMoveTo << endl;
        }
        //Find a line with fewer than 2 X's and set any C found on line as bestMoveFrom
        // if (isThreatened)
        // {
        //     for (int i = 0; i < 3; i++)
        //     {
        //         if (line[i] == 'C')
        //         {
        //             m_bestMoveFrom = line[i] - 48; //Subtracting 48 converts from ASCII to integer
        //         }
        //     }
            //TESTING
            // cout << "isThreatened moveFrom: " << m_bestMoveFrom << endl;
        // }
    }

    void checkCanWin(char line[])
    {
        int counter = 0;
        //Count C's on line
        for (int i = 0; i < 3; i++)
        {
            if (line[i] == 'C')
            {
                counter++;
            }
            if (line[i] == 'X')
            {
                counter--;
            }
        }
        //TESTING
        // cout << "canWin counter: " << counter << endl;
        if (counter > 1)
        {
            for (int i = 0; i < 3; i++)
            {
                if (line[i] != 'C')
                {
                    m_bestMoveTo = line[i] - 48; //Subtracting 48 converts from ASCII to integer
                }
            }
            //TESTING
            // cout << "canWin moveTo: " << m_bestMoveTo << endl;
        }
        // if (counter == 1)
        // {
        //     for (int i = 0; i < 3; i++)
        //     {
        //         if (line[i] == 'C')
        //         {
        //             m_bestMoveFrom = line[i] - 48; //Subtracting 48 converts from ASCII to integer
        //         }
        //     }
        //     //TESTING
        //     // cout << "canWin moveFrom: " << m_bestMoveFrom << endl;
        // }
    }

    int getBestFrom()
    {
        return m_bestMoveFrom;
    }

    int getBestTo()
    {
        return m_bestMoveTo;
    }

};//END CLASS -------------------------------------------------------------------------------------------


//Class holding the main logic required to start and play Tic tac toe.
//
//Note that m_players[] and m_board[] are both one index longer than needed; this has been done so that 
//indexes correspond to names (IE: player 2 is on index 2 and so on).
class Game
{
    friend class Player; //Not optimal. Refrain from using friend classes in future.

    Player m_players[3];
    char m_board[10] = {'0','1','2','3','4','5','6','7','8','9'};
    char m_state = 'R';
    int m_turn = 1;
    int m_currentPlayer = 1;

public:
    Game(char playComputer)
    {
        //Set player 1 as human player
        Player player1 = Player('X', false);
        m_players[1] = player1;
   
        //Player chooses to play against a human
        if (playComputer == 'n' || playComputer == 'N')
        {
            //Set player 2 as human player
            Player player2 = Player('O', false);
            m_players[2] = player2;
        }
        //If player is smart-ass
        else
        {
            //Set player 2 as computer player
            Player player2 = Player('C', true);
            m_players[2] = player2;
        }
    }

    void Turn()
    {
    int input;

        //Check if turn is more than 2 (IE: both players have three pieces 
        //in play).
        if (m_turn > 3)
        {
            if(getPlayer(m_currentPlayer).isComputer())
            {
                ComputerPlayer computer = ComputerPlayer(getTurn(), getBoardPointer());
                //TESTING
                cout << "computer calling moveTile: From: " << computer.getBestFrom() << " To: " << computer.getBestTo() << endl;
                moveTile(computer.getBestFrom(), computer.getBestTo(), m_currentPlayer);
            }
            else
            {
                int from;
                int to;
                from = getInputWhenAskingFrom();
                to = getUserInput();
                moveTile(from, to, m_currentPlayer);
            } 
        }
        else
        {
            //Check if currentPlayer is computer
            if (getPlayer(m_currentPlayer).isComputer())
            {
                ComputerPlayer computer = ComputerPlayer(getTurn(),getBoardPointer());
                setTile(computer.getBestTo(), m_currentPlayer);
            }
            else
            {
                // cout << "   Turn " << m_turn << ". Player " << 
                //         getPlayer(m_currentPlayer).getLetter() << " enter tile:" << endl;
                int input = getUserInput();
                setTile(input, m_currentPlayer);
            } 
        }
    
        //Change player
        if (m_currentPlayer == 1)
        {
            m_currentPlayer = 2;
        }
        else
        {
            //If player is 2, increment turn
            m_turn++;
            m_currentPlayer = 1;
        }
        //Check game state
        checkGameState();

        //Print board and redo or print result of game
        printBoard();
    }

    int getUserInput()
    {
        int input;
            
        while(true)
        {
            cin >> input;

            if(cin.fail() ||
                m_board[input] != (input + 48) ||//Adding 48 converts from integer to ASCII value
                input < 1 ||
                input > 9)
            {
                //TESTING
                // cout << "Ascii input: " << input+48 << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << printError() << endl;
                cin >> input;
            }
            if (!cin.fail() &&
                m_board[input] == (input + 48) &&//Adding 48 converts from integer to ASCII value
                input > 0 &&
                input < 10)
            {
                break;
            }   
        }
        return input;
    }

    int getInputWhenAskingFrom()
    {
        int input;
            
        while(true)
        {
            cin >> input;

            if(cin.fail() ||
                m_board[input] != getPlayer(m_currentPlayer).getLetter())//Adding 48 converts from integer to ASCII value
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << printError() << endl;
                cin >> input;
            }
            if (!cin.fail() &&
                m_board[input] == getPlayer(m_currentPlayer).getLetter())//Adding 48 converts from integer to ASCII value
            {
                break;
            }   
        }
        return input;
    }

    //Check game state (player has won, board is full with no winner, still running) by running through the 
    //board searching for winning combinations. On any straight line on the board, if one player has all 
    //tiles then the winner can be determined by returning the value of any tile in the row.
    //
    //@ return char (R = game still running, N = game over with no winner, X = X wins, O = O wins)
    void checkGameState(){
        //Check winning combinations:
        if (m_board[1] == m_board[2] && m_board[2] == m_board[3])
        {
            m_state = m_board[1];
        }
        else if (m_board[4] == m_board[5] && m_board[5] == m_board[6])
        {
            m_state =  m_board[4];
        }
        else if (m_board[7] == m_board[8] && m_board[8] == m_board[9])
        {
            m_state =  m_board[7];
        }
        else if (m_board[1] == m_board[4] && m_board[4] == m_board[7])
        {
            m_state =  m_board[1];            
        }
        else if (m_board[2] == m_board[5] && m_board[5] == m_board[8])
        {
            m_state =  m_board[2];    
        }
        else if (m_board[3] == m_board[6] && m_board[6] == m_board[9])
        {
            m_state =  m_board[3];            
        }
        else if (m_board[1] == m_board[5] && m_board[5] == m_board[9])
        {
            m_state =  m_board[1];            
        }
        else if (m_board[3] == m_board[5] && m_board[5] == m_board[7])
        {
            m_state =  m_board[3];            
        }
        //If no winning combinations
        else
        {
            //Set counter
            int counter = 0;
            //Count set tiles
            for (int i = 1; i < 10; i++)
            {
                if (m_board[i] == 'X' || m_board[i] == 'O' || m_board[i] == 'C')
                {
                    counter++;
                }
            }
            //If eight tiles are set: Game ends in draw
            if (counter == 8)
            {
                m_state = 'N';
            }
            //Else game continues
            else
            {
                m_state = 'R';
            }   
        }
    }


/////   GAME SETTERS     //////////////////////////////////////////////////////////////////////

    void setTile(int number, int playerNumber)
    {
        m_board[number] = m_players[playerNumber].m_letter;
    }

    void moveTile(int from, int to, int playerNumber)
    {
        m_board[from] = (from + 48);//Adding 48 converts from integer to ASCII value
        m_board[to] = m_players[playerNumber].m_letter;
    }
    

/////   GAME GETTERS    //////////////////////////////////////////////////////////////////////

    Player getPlayer(int number)
    {
        return m_players[number];
    }

    char* getBoardPointer()
    {
        char *ptr;
        ptr = &m_board[0];
        
        return ptr;
    }
    
    char getTileByNumber(int number)
    {
        return m_board[number];
    }

    char getGameState()
    {
        return m_state;
    }

    int getTurn()
    {
        return m_turn;
    }


/////   GAME PRINTERS    //////////////////////////////////////////////////////////////////////

    void printBoard()
    {
        //Check OS and call correct clear-screen function
        if (PLATFORM == "windows")
        {
            system("cls");
        }
        else if (PLATFORM == "other")
        {
            system("clear");
        }
        else
        {
            //Do nothing
        }

        cout << endl;
        if (getPlayer(2).isComputer())
        {
            cout << printMessageFromComputer() << endl;
        }
        else{
            cout << "Player 1 is X, player 2 is O" << endl;
        }
        cout << endl;
        cout << "        |     |     " << endl;
        cout << "     " << m_board[1] << "  |  " << m_board[2] << "  |  " << m_board[3] << endl; 
        cout << "   _____|_____|_____" << endl;
        cout << "        |     |     " << endl;
        cout << "     " << m_board[4] << "  |  " << m_board[5] << "  |  " << m_board[6] << endl; 
        cout << "   _____|_____|_____" << endl;
        cout << "        |     |     " << endl;
        cout << "     " << m_board[7] << "  |  " << m_board[8] << "  |  " << m_board[9] << endl; 
        cout << "        |     |     " << endl;
        cout << endl;
        cout << printInstruction();
        cout << endl;
    }

    string printInstruction()
    {
        string result;

        result = "  Turn "; 
        result += m_turn + 48;
        result += ". ";
        
        if (getGameState() == 'N')
        {
            result += "Nobody wins!";    
        }
        else if (getGameState() != 'N' && getGameState() != 'R')
        {
            result += "Player ";
            result += getGameState();
            result += " wins!";            
        }
        else if (m_turn <= 3)
        {
            result += "Player ";
            result += getPlayer(m_currentPlayer).getLetter();
            result += " enter tile: ";
        }
        else
        {
            result += "Player ";
            result += getPlayer(m_currentPlayer).getLetter();
            result += " enter tiles to move from and to: ";
        }
        return result;
    }

    string printError()
    {
        return "Please input correct tile: ";
    }

    string printMessageFromComputer()
    {
        string result;

        result += "   ";
        
        //Seed is set in main() as it should only be set once
        int random = rand()%15;

        string messages[15] = 
        {
        "Hah!",                                      //0
        "Noone expects the Spanish inquisition!", 
        "Think you can beat me?!",
        "Try harder.",
        "Oh, the humanity!",                         //4
        "Hello... World..?",
        "Are we playing for pink slips?",
        "if (player == good){playCorrectTile(true)}",
        "A difference engine, a Turing machine and an Anthekythera device walk into a bar...",
        "Are we there yet?",                         //9
        "Error: expected a \";\"",
        "I wish I was a Threadripper...",
        "Your mother was hamster and your father smelled of elderberries!",
        "Need more coffee?",
        "00|>5"                                      //14
        };
        result += messages[random];

        int counter = 0;

        for (int i = 1; i < 10; i++)
        {
            if (m_board[i] == 'C')
            {
                counter++;
            }
        }
        if(counter > 3)
        {
            result = "Cheating? I would NEVER!";
        }
    
        return result;
    }

};//END CLASS -------------------------------------------------------------------------------------------


int main()
{
    srand(time(NULL)); //Set random seed

    //Check OS and call correct clear-screen function
    if (PLATFORM == "windows")
    {
        system("cls");
    }
    else if (PLATFORM == "linux")
    {
        system("clear");
    }
    else
    {
        //Do nothing
    }

    //Print welcome message
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "        |     |     " << endl;
    cout << "     T  |  i  |  c  " << endl; 
    cout << "   _____|_____|_____" << endl;
    cout << "        |     |     " << endl;
    cout << "     T  |  a  |  c  " << endl; 
    cout << "   _____|_____|_____" << endl;
    cout << "        |     |     " << endl;
    cout << "     T  |  o  |  e  " << endl; 
    cout << "        |     |     " << endl;
    cout << endl;
    cout << "   Play against computer? (y/n)" << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    char playComputer;
    cin >> playComputer;

    Game game = Game(playComputer);

    game.printBoard();

    while (game.getGameState() == 'R')
    {
        game.Turn();
    }

    cin.clear();
    char endApp;
    cin >> endApp;

    return 0;
}//END PROGRAM -------------------------------------------------------------------------------------------
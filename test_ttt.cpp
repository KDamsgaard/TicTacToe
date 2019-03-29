




#include <iostream>

using namespace std;

class Player
{
public: //TESTING

    char m_letter;
    bool m_isComputer = false; //TESTING

public:
    Player(){} //DEFAULT
    
    Player(char letter)
    : m_letter(letter)
    {
    }

    char getLetter()
    {
        return m_letter;
    }

    virtual bool isComputer()
    {
        return false;
    }

};

class Computer : public Player
{
    char m_letter;
    bool m_isComputer = true; //TESTING

public:
    Computer(){} //DEFAULT

    Computer(char letter)
    : Player(letter), m_letter(letter)
    {
    }

    char getLetter()
    {
        return m_letter;
    }

    bool isComputer()
    {
        return true;
    }
};

class Game
{
public: //TESTING
    Player m_players[3];
    Player* m_player_ptrs[3];

    //What we want to avoid needing
    // char m_playComputer;
    // Player m_player1;
    // Player m_player2;
    // Computer m_computer;

public:
    Game(char playComputer)
    {
        m_players[1] = Player('X');
        
        if (playComputer == 'n')
        {
            m_players[2] = Player('O');
        }
        else{
            //Set a pointer to a Player
            Player *player_ptr;

            //Create a Computer
            Computer computer = Computer('C');

            //Set value of player pointer as address of Computer object
            player_ptr = &computer;

            // Player *player_ptr = new Computer('C');
            
            //Print by pointer
            cout << "From Game(): " << player_ptr->getLetter() << " is computer: " 
                    << player_ptr->isComputer() << endl;

            //Player 2 in pointer array is set as pointer to Computer object
            m_player_ptrs[3] = player_ptr;

            //Print by pointer from array
            cout << "From Game(): " << m_player_ptrs[3]->getLetter() << " is computer: " 
                    << m_player_ptrs[3]->isComputer() << endl;
        }
    }

    Player getPlayer(int number)
    {
        return m_players[number];            
    }

    Player* getPlayerByPointer(int number)
    {
        return m_player_ptrs[number];
    }

    //What we want to avoid needing
    // Computer returnComputer()
    // {
    //     return m_computer;
    // }

    //What we want to avoid needing
    // Player returnPlayer(int number)
    // {
    //     return m_players[number];
    // }

};

int main()
{
    Game game1 = Game('y');

    cout << game1.getPlayer(1).getLetter() << " is computer: " << game1.getPlayer(1).isComputer() << endl;
    
    // // Does not work!
    // cout << game1.m_player_ptrs[3]->getLetter() << " is computer: " 
    //      << game1.m_player_ptrs[3]->isComputer() << endl;

    // // Does not work!
    // cout << game1.getPlayerByPointer(2)->getLetter() << " is computer: " 
    //      << game1.getPlayerByPointer(2)->isComputer() << endl;

    // // Does not work!
    // cout << game1.m_player_ptrs[3]->m_letter << " is Computer: " << game1.m_player_ptrs[3]->m_isComputer 
    //      << endl;

    return 0;
}
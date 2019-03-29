




#include <iostream>

using namespace std;

class Player
{
    char m_letter;

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
    Player m_players[3];
    char m_playComputer;
    Player m_player1;
    Player m_player2;
    Computer m_computer;

    public:
        Game(char playComputer)
        : m_playComputer(playComputer)
        {
            m_players[1] = Player('X');
            
            if (m_playComputer == 'n')
            {
                m_players[2] = Computer('O');
            }
            else{
                m_computer = Computer('C');
            }
        }

        Player getPlayer(int number)
        {
            if (m_playComputer != 'n' && number != 1)
            {
                returnComputer();
            }
            else if (number == 1)
            {
                returnPlayer(1);
            }
            else if (number == 2)
            {
                returnPlayer(2);
            }
            else
            {
                throw("getPlayer() in Game given bad player number.");
            }
            
            
        }

        Computer returnComputer()
        {
            return m_computer;
        }

        Player returnPlayer(int number)
        {
            return m_players[number];
        }

};






int main()
{
    Game game1 = Game('n');

    cout << game1.getPlayer(1).getLetter() << " is computer: " << game1.getPlayer(1).isComputer() << endl;
    cout << game1.getPlayer(2).getLetter() << " is computer: " << game1.getPlayer(2).isComputer() << endl;

    


    return 0;
}
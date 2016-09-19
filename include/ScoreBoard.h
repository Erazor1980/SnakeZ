#pragma once
#include <string>
#include <vector>

#define MAX_NUMBER_SCORES   (10)    /* maximal number of entries in the high score list */

class Node
{
public:
    Node( const std::string name, const int points )
    {
        m_name      = name;
        m_points    = points;
        mp_nextNode = NULL;
    }

    void setNext( Node* next )
    {
        mp_nextNode = next;
    }

    std::string m_name;
    int m_points;
    Node* mp_nextNode;
};

class ScoreBoard
{
public:
    ScoreBoard();
    ~ScoreBoard();

    void writeToFile( const std::string fileName = "highscore.bin" );
    void loadFromFile( const std::string fileName = "highscore.bin" );
    bool isNotEmpty()
    {
        if( mp_firstNode == NULL )
            return false;
        else
            return true;
    }

    void testPrint()
    {
        Node* currNode = mp_firstNode;
        printf( "HIGHSCORES:\n" );
        int i = 1;
        while( currNode )
        {
            printf( "%d:\t%s\t%d\n", i, currNode->m_name.c_str(), currNode->m_points );
            currNode = currNode->mp_nextNode;
            i++;
        }
    }

    std::vector< Node > getScoreBoardList() const;
    
    // returns position in the highscore list (0-9), -1 -> out of list
    int addScore( const std::string name, const int points );

private:
    void reset();

    Node* mp_firstNode = NULL;
};
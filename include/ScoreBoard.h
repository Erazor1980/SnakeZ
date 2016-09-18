#pragma once
#include <string>
#include <vector>

#define MAX_NUMBER_SCORES   (10)    /* maximal number of entries in the high score list */

struct Node
{
    Node( const std::string name, const int points )
    {
        m_name      = name;
        m_points    = points;
        mp_nextNode = NULL;
    }
    ~Node(){}

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

    void loadScoreBoard();
    void saveScoreBoard();

    void addScore( const std::string name, const int points );

private:
    Node* mp_firstNode = NULL;
};
#include "ScoreBoard.h"
#include <fstream>

ScoreBoard::ScoreBoard()
{
}

ScoreBoard::~ScoreBoard()
{
    reset();
}

void ScoreBoard::reset()
{
    Node* current = mp_firstNode;
    while( current != NULL )
    {
        Node* next = current->mp_nextNode;
        delete current;
        current = NULL;
        current = next;
    }
    mp_firstNode = NULL;
}

struct nameAndScore
{
    char name[ 50 ];
    int score;
};

void ScoreBoard::writeToFile( const std::string fileName )
{
    std::ofstream out( fileName.c_str(), std::ios::binary );

    Node* currNode = mp_firstNode;
    while( currNode )
    {
        nameAndScore nas;
        sprintf_s( nas.name, "%s", currNode->m_name.c_str() );
        nas.score = currNode->m_points;

        out.write( ( char* )&nas, sizeof( nameAndScore ) );
        currNode = currNode->mp_nextNode;
    }
    out.close();
}

void ScoreBoard::loadFromFile( const std::string fileName )
{
    reset();

    std::ifstream in( fileName.c_str(), std::ios::binary );
    if( !in.is_open() )
    {
        printf( "Could not open %s.\n", fileName.c_str() );
        return;
    }

    while( !in.eof() )
    {
        nameAndScore nas;
        in.read( ( char* )&nas, sizeof( nameAndScore ) );

        // check if the size of read data equals the desired size
        if( (int)in.gcount() == sizeof( nameAndScore ) )
            addScore( nas.name, nas.score );
    }
    in.close();
}

std::vector<Node> ScoreBoard::getScoreBoardList() const
{
    std::vector< Node > vNodes;
    Node* currNode = mp_firstNode;
    while( currNode )
    {
        vNodes.push_back( Node( currNode->m_name, currNode->m_points ) );
        currNode = currNode->mp_nextNode;
    }
    return vNodes;
}

int ScoreBoard::addScore( const std::string name, const int points )
{
    int posInList = 0;
    if( mp_firstNode == NULL )
    {
        mp_firstNode = new Node( name, points );
        posInList = 0;
    }
    else
    {
        // special case for new high score (mp_firstNode has to be replaced)
        if( points > mp_firstNode->m_points )
        {
            Node* next = mp_firstNode;
            mp_firstNode = new Node( name, points );
            mp_firstNode->setNext( next );
            posInList = 0;
        }
        else
        {
            // at least one entry in the list
            Node* currNode = mp_firstNode;
            while( true )
            {
                posInList++;

                // end of list
                if( currNode->mp_nextNode == NULL )
                {
                    currNode->mp_nextNode = new Node( name, points );
                    break;
                }

                // mid of the list
                if( points > currNode->mp_nextNode->m_points )
                {
                    Node* newNode = new Node( name, points );
                    newNode->setNext( currNode->mp_nextNode );
                    currNode->setNext( newNode );
                    break;
                }
                else
                {
                    currNode = currNode->mp_nextNode;
                }
            }
        }

        // delete last element, if list > MAX_NUMBER_SCORES
        int i = 1;
        Node* currNode = mp_firstNode;
        while( true )
        {
            if( currNode->mp_nextNode )
            {
                i++;
                // list too big, delete last element
                if( i > MAX_NUMBER_SCORES )
                {
                    delete currNode->mp_nextNode;
                    currNode->mp_nextNode = NULL;
                    break;
                }
                currNode = currNode->mp_nextNode;
            }
            else
            {
                // everything is fine!
                if( i <= MAX_NUMBER_SCORES )
                {
                    break;
                }
            }
        }
    }
    if( posInList >= MAX_NUMBER_SCORES )
    {
        posInList = -1;
    }
    return posInList;
}



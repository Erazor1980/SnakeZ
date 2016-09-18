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

void ScoreBoard::writeToFile( const std::string fileName )
{
    std::ofstream out( fileName.c_str() );
    
    if( mp_firstNode )
    {
        out << mp_firstNode->m_name << std::endl;
        out << mp_firstNode->m_points;
    }
    if( mp_firstNode->mp_nextNode )
    {
        Node* currNode = mp_firstNode->mp_nextNode;
        while( currNode )
        {
            out << std::endl << currNode->m_name << std::endl;
            out << currNode->m_points;
            currNode = currNode->mp_nextNode;
        }
    }    
    out.close();
}

void ScoreBoard::loadFromFile( const std::string fileName )
{
    reset();
    std::string name;
    int points;
    std::ifstream in( fileName.c_str() );
    if( !in.is_open() )
    {
        printf( "Could not open %s.\n", fileName.c_str() );
        return;
    }
    while( !in.eof() )
    {
        in >> name;
        in >> points;
        addScore( name, points );
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

bool ScoreBoard::addScore( const std::string name, const int points )
{
    if( mp_firstNode == NULL )
    {
        mp_firstNode = new Node( name, points );
    }
    else
    {
        // special case for new high score (mp_firstNode has to be replaced)
        if( points > mp_firstNode->m_points )
        {
            Node* next = mp_firstNode;
            mp_firstNode = new Node( name, points );
            mp_firstNode->setNext( next );
            return true;
        }

        // at least one entry in the list
        Node* currNode = mp_firstNode;
        while( true )
        {
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

        // delete last element, if list > MAX_NUMBER_SCORES
        int i = 1;
        currNode = mp_firstNode;
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
    return false;
}



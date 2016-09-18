#include "ScoreBoard.h"

ScoreBoard::ScoreBoard()
{
}

ScoreBoard::~ScoreBoard()
{
    Node* current = mp_firstNode;
    while( current != NULL )
    {
        Node* next = current->mp_nextNode;
        delete current;
        current = next;
    }
    mp_firstNode = NULL;
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

void ScoreBoard::loadScoreBoard()
{
}

void ScoreBoard::saveScoreBoard()
{
}

void ScoreBoard::addScore( const std::string name, const int points )
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
            return;
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
}

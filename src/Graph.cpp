//2-9-2026

#include "..\include\Graph.h"
#include <queue>
#include <iostream>

using namespace std;

//Constructor
GraphNode::GraphNode(const Vector& vector, std::size_t id): vector(vector), id(id){
}

void Graph::addEdge(GraphNode* node1, GraphNode* node2)
{
    node1->addNeighbor(node2);
}

void GraphNode::addNeighbor(GraphNode* node)
{
    neighbors.push_back(node);
}

const vector<GraphNode*>&GraphNode::getNeighbors() const
{
    return neighbors;
}

std::size_t GraphNode::getId() const
{
    return id;
}


GraphNode* Graph::addNode(const Vector& vector)
{
    //Maak een nieuwe GraphNode aan in het geheugen en verwijs hiernaar met een pointer newNode
    {
        GraphNode* newNode = new GraphNode(vector, nextNodeId);

        nextNodeId++;

        //2-9-2026Kanweg newNode->vector = vector;

        nodes.push_back(newNode);

        return newNode;
    }
}

void Graph::bfs(GraphNode* startNode)
{
    queue<GraphNode*> queue;

    //2-9-26: Dit kan later sneller met een set
    vector<GraphNode*> visited;

    queue.push(startNode);
    visited.push_back(startNode);

    while(!queue.empty())
    {
        //queue is zelf het object
        GraphNode* currentNode = queue.front();
        queue.pop();

        cout<<"Visited Node: "<<currentNode->getId() <<endl;

        //doorloop de neighbours
        for(GraphNode* neighbor : currentNode->getNeighbors())
        {
            bool alreadyVisited = false;
            
            for( GraphNode*visitedNode:visited)
            {
                if(visitedNode == neighbor)
                {      
                    alreadyVisited = true;
                    break;
                }
            }
            if(!alreadyVisited)
            {
                queue.push(neighbor);
                visited.push_back(neighbor);
            }
        }
    }
}

void Graph::dfs(GraphNode* startNode)
{
    vector<GraphNode*>visited;

    dfsRecursive(startNode, visited);
}

void Graph::dfsRecursive(GraphNode* currentNode, vector<GraphNode*>&visited)
{
    visited.push_back(currentNode);
    
    cout<<"Visited Node: "<<currentNode->getId()<<endl;

    for(GraphNode*neighbor:currentNode->getNeighbors())
    {
        bool alreadyVisited = false;

        for(GraphNode* visitedNode:visited)
        {
            if(visitedNode == neighbor)
            {
                alreadyVisited = true;
                break;
            }
        }

        if(!alreadyVisited)
        {
            dfsRecursive(neighbor, visited);
        }
    }
}

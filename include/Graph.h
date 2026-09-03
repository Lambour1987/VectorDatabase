//1-9-2026

#pragma once

#include "Vector.h"
#include <vector>

class GraphNode
{
    //Iedere GraphNode kan dus 1 object Vector hebben. Als we er meer hadden gewild, Hadden we bijv. Vector vector1, Vector vector2, Vector vector3 etc.
    // Hier dus geen pointers. De vector zit gewoon in de node. En de Graphnode heeft dus ook toegang tot alle public functies van Vector zoals
    // Distance.h en at etc.
    public:
        //Parameterized constructor
        GraphNode(const Vector& vector, std::size_t id);

        //Functie die straks bij het private gedeelte kan om de neighbours aan te wijzen
        void addNeighbor(GraphNode* node);

        const std::vector<GraphNode*>& getNeighbors() const;

        std::size_t getId() const;

        Vector vector;

    private:
        std::size_t id;
        std::vector<GraphNode*> neighbors;

};

class Graph
{
    private:
        std::vector<GraphNode*> nodes;
        std::size_t nextNodeId = 0;

    public:
        GraphNode* addNode(const Vector& vector);
        void addEdge(GraphNode* node1, GraphNode* node2);
        void bfs(GraphNode* startNode);
        void dfs(GraphNode* startNode);
        void dfsRecursive(GraphNode* currentNode, std::vector<GraphNode*>&visited);
};
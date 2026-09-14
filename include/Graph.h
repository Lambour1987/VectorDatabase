//1-9-2026

#pragma once

#include "Vector.h"
#include <vector>

//7-9-26:
#include <cstddef>

//14-9-26
#include <unordered_map>

//We vertellen alvast dat GraphNode bestaat
class GraphNode;


//3-9-26: Hier dus een struct ipv Class. Vanwege semantiek: gewoon een 'pakketje' gegevens dat verbinding beschrijft.
// Class is eerder een object met eigen gedrag
struct Edge
{
    GraphNode* destination;
    double weight;
};
class GraphNode
{
    //Iedere GraphNode kan dus 1 object Vector hebben. Als we er meer hadden gewild, Hadden we bijv. Vector vector1, Vector vector2, Vector vector3 etc.
    // Hier dus geen pointers. De vector zit gewoon in de node. En de Graphnode heeft dus ook toegang tot alle public functies van Vector zoals
    // Distance.h en at etc.
    public:
        //Parameterized constructor
        GraphNode(const Vector& vector, std::size_t id);

        //Functie die straks bij het private gedeelte kan om de neighbours aan te wijzen
        void addEdge(GraphNode* node, double weight);

        const std::vector<Edge>& getEdges() const;

        std::size_t getId() const;

        Vector vector;

    private:
        std::size_t id;
        std::vector<Edge> edges;

};

//7-9-2026: Struct om resultaat op te slaan van Dijkstra

struct DijkstraResult
{
    //14-9-2026: dit wordne unordered maps
    // std::vector<double>distances;
    // std::vector<GraphNode*>previous;
    std::unordered_map<GraphNode*, double> distances;
    std::unordered_map<GraphNode*,GraphNode*> previous;
};    


// 7-9-2026: Struct A-star result: Uitbreiding van Dijkstra om op basis van schattingen (afgeleid van Eucleudische afstand)
// het SSSP te berekenen ipv alleen de werkelijke afstanden. We berekenen per positie de afstand tot de doelpositie. 

struct AStarResult
{
    //HIer dus ipv distances (Dijkstra) gscores gebruiken.
    std::vector<double>gScores;
    std::vector<GraphNode*> previous;
};

class Graph
{
    public:
        GraphNode* addNode(const Vector& vector);
        void addEdge(GraphNode* node1, GraphNode* node2, double weight);
        void bfs(GraphNode* startNode);
        void dfs(GraphNode* startNode);
        void dfsRecursive(GraphNode* currentNode, std::vector<GraphNode*>&visited);
        //void dijkstra(GraphNode* startNode);
        DijkstraResult dijkstra(GraphNode* startNode);
        AStarResult aStar(GraphNode* startNode, GraphNode* targetNode);
        //14-9-26: Deze ook aangepast naar een unordered_map: std::vector<GraphNode*>reconstructPath(GraphNode* startNode, GraphNode* targetNode, const std::vector<GraphNode*>& previous);
        std::vector<GraphNode*> reconstructPath(GraphNode* startNode,GraphNode* targetNode,const std::unordered_map<GraphNode*, GraphNode*>& previous);

        ~Graph();

    private:
        std::vector<GraphNode*> nodes;
        std::size_t nextNodeId = 0;

};


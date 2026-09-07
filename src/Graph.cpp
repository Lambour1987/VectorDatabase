//2-9-2026

#include "..\include\Graph.h"
#include "..\include\MinHeap.h"

#include <queue>
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

//Constructor
GraphNode::GraphNode(const Vector& vector, std::size_t id): vector(vector), id(id){
}

void Graph::addEdge(GraphNode* node1, GraphNode* node2, double weight)
{
    node1->addEdge(node2, weight);
}

void GraphNode::addEdge(GraphNode* node, double weight)
{
    edges.push_back({node, weight});
}

const vector<Edge>&GraphNode::getEdges() const
{
    return edges;
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
        for(const Edge& edge : currentNode->getEdges())
        {
            GraphNode* neighbor = edge.destination;

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

    for(const Edge& edge:currentNode->getEdges())
    {
        GraphNode* neighbor = edge.destination;

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

//4-9-26: We zitten al in de class Graph dus we hoeven niet los de adjacency list te importeren. We kunnen dara al bij
DijkstraResult Graph::dijkstra(GraphNode* startNode)
{
    //3-9-26: Initialiseer een vector distances die de omvang heeft van het aantal nodes en en geef de waarden infinity
    // Dus initialiseer alles op infinity
    std::vector<double> distances(nodes.size(), std::numeric_limits<double>::infinity());

    //7-9-26: Declareer en initaliseer previous
    std::vector<GraphNode*> previous(nodes.size(), nullptr);

    //Hier zetten we de eerste node op 0 (want vanuit daar berekenen)
    distances[startNode->getId()] = 0;

    //Om de laagste waarde te bepalen gebruiken we een MinHeap
    MinHeap heap;

    
    heap.push({0, startNode});

    //zolang de heap niet leeg is
    while(!heap.empty())
    {
        //stop de top van heap in current
        auto current = heap.top();
        //haal deze uit de heap
        heap.pop();
                
        //stop eerste gedeelte van current in een double currentDistance
        double currentDistance = current->first;

        //maak een pointer naar GraphNode genaamd currentNode en laat die wijzen naar het tweede deel van current.
        GraphNode* currentNode = current->second;

        // Als de afstand die uit deze node uit de heap komt slechter is dan de afstand die we al kennen, dan slaan
        // we die over
        if (currentDistance > distances[currentNode->getId()])
        {
            continue;
        }

        //Vanuit de startNode bekijken welke nodes er verbonden zijn met de startNode: hieruit krijgen
        // wij destination en gewicht
        // dit kan weg omdat we dit verwerkt hebben de while loop  startNode->getEdges();
        //Directe buren van de startNode in distance zetten door met een forloop
        // StartNode is een pointer naar een GraphNode: Dus ga naar het GraphNode-object waar startNode naar
        // verwijst en roep getEdges() op. We krijgen dan terug een verzameling Edges. En die gaan we doorlopen.
        // Dus voor iedere Edge in de verzameling Edges van de startnode, geef een read-only referentie naar die Edge en noem die edge.
        for(const Edge& edge:currentNode->getEdges())
        {
            
            //maak een variabele newDistance aan van het type double die de afstand berekend van
            // de huidige distance naar de andere edges
            double newDistance = currentDistance + edge.weight;
            //Als de nieuwe afstand kleiner is dan de afstand die er al staat
            if(newDistance<distances[edge.destination->getId()])
            {
                //Neem dan de nieuwe afstand
                distances[edge.destination->getId()]=newDistance;

                //7-9-26 Van welke node kwamen wij toen we de beste route vonden naar huidige node
                previous[edge.destination->getId()] = currentNode;

                // Direct tijdens het doorlopen stoppen we de node in de heap. Dit doen we tijdens het doorlopen
                // direct. Als we het buiten de loop in de heap zouden stoppen dan heeft het geen zin omdat we dan
                // alsnog die distance array steeds moeten doorlopen.
                heap.push({newDistance, edge.destination});
            }
        }
    }
    //7-9-26: Vanaf hier de testen: Kan weg
    // std::cout << "\nAfstanden:" << std::endl;

    // for (std::size_t i = 0; i < distances.size(); i++)
    // {
    //     std::cout << "Node " << i
    //             << " | distance: " << distances[i]
    //             << std::endl;
    // }

    // std::cout << "\nPrevious:" << std::endl;

    // for (std::size_t i = 0; i < previous.size(); i++)
    // {
    //     std::cout << "Node " << i << " | previous: ";

    //     if (previous[i] == nullptr)
    //     {
    //         std::cout << "nullptr";
    //     }
    //     else
    //     {
    //         std::cout << previous[i]->getId();
    //     }

    //     std::cout << std::endl;
    // }
    return {distances, previous};
}

//7-9-2026: Pad Reconstruction: Functie reconstructPath die als input een pointer StartNode en een TargetNode heeft
// en de vector van pointers previous
vector<GraphNode*> Graph::reconstructPath(GraphNode* startNode, GraphNode* targetNode, const vector<GraphNode*>& previous)
{
    //Maak leeg path
    vector<GraphNode*> path;
    //Target is currentNode
    GraphNode* currentNode = targetNode;
    //Zolang current geen nullptr is
    while(currentNode !=nullptr)
    {
        //Voeg current toe aan path
        path.push_back(currentNode);
        //Current wordt vorige
        if(currentNode == startNode)
        {
            break;
        }

        currentNode = previous[currentNode->getId()];
    }

    //Target was niet bereikbaar vanaf start
    if(path.empty() || path.back() !=startNode)
    {
        path.clear();
        return path;
    }

    std::reverse(path.begin(), path.end());
    
    //Retourneer path
    return path;
}

//
AStarResult Graph::aStar(GraphNode* startNode, GraphNode* targetNode)
{
    //Initialisatie met alle waarden op infinity
    vector<double>gScores(nodes.size(), std::numeric_limits<double>::infinity());
    vector<GraphNode*> previous(nodes.size(),nullptr);

    //7-9-26:
    gScores[startNode->getId()] = 0;

    MinHeap heap;

    double g = 0;
    double h = startNode->vector.distanceTo(targetNode->vector);
    double f = g +h;
    heap.push({f, startNode});

    while(!heap.empty())
    {
        auto current = heap.top();
        heap.pop();

        double currentF = current->first;
        GraphNode* currentNode = current->second;

        double currentG = gScores[currentNode->getId()];

        double currentH = currentNode->vector.distanceTo(targetNode->vector);

        double expectedF = currentG + currentH;

            // 👇 HIER zetten
        cout << "A* visits node: "
            << currentNode->getId()
            << " | g = " << currentG
            << " | h = " << currentH
            << " | f = " << currentF
            << endl;

        if(currentF>expectedF)
        {
            continue;
        }

        if(currentNode == targetNode)
        {
            break;
        }

        //7-9-26: Hier de buren gaan bekijken
        for(const Edge& edge:currentNode->getEdges())
        {
            GraphNode* neighbor = edge.destination;

            double tentativeG = currentG + edge.weight;

            if(tentativeG < gScores[neighbor->getId()])
            {
                gScores[neighbor->getId()]=tentativeG;
                previous[neighbor->getId()] = currentNode;

                double h = neighbor->vector.distanceTo(targetNode->vector);

                double f = tentativeG + h;

                heap.push({f, neighbor});
            }

        }
    }

    return {gScores, previous};

}



//7-9-26: Destructor
Graph::~Graph()
{
    for (GraphNode* node : nodes)
    {
        delete node;
    }
}
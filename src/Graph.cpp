//2-9-2026

#include "..\include\Graph.h"
#include "..\include\MinHeap.h"
#include "..\include\KDTree.h"

#include <queue>
#include <iostream>
#include <limits>
#include <algorithm>

//12-9-2026: Unordered set toegevoegd
#include <unordered_set>

//14-9-2026: Unordered map toegevoegd
#include <unordered_map>

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
    //12-9-26: set vector<GraphNode*> visited;
    unordered_set<GraphNode*> visited;

    //14-9-26: als test tbv DFS neergezet
    unordered_map<GraphNode*, double> distances;
    distances[startNode]=0.0;
    cout << "Distance of startNode: "
     << distances[startNode]
     << endl;

    queue.push(startNode);
    //12-9 vervangen omdat we set gebruiken. visited.push_back(startNode); wordt
    visited.insert(startNode);

    while(!queue.empty())
    {
        //queue is zelf het object
        GraphNode* currentNode = queue.front();
        queue.pop();

        cout<<"Visited Node: "<<currentNode->getId() <<endl;

        //doorloop de neighbours:
        //10-9-26: Dus: currentNode wijst naar een GraphNode object die een memberfunctie getEdges heeft waarvan de output een vector van Edges is.
        // Door die output lopen we heen (dat zijn dus Edge objecten binnen de vector) en daarvan pakken we steeds eentje eruit (en noemen we edge)
        for(const Edge& edge : currentNode->getEdges())
        {
            GraphNode* neighbor = edge.destination;

            bool alreadyVisited = false;
            
            // 12-9-26: 
            // for( GraphNode*visitedNode:visited)
            // {
            //     if(visitedNode == neighbor)
            //     {      
            //         alreadyVisited = true;
            //         break;
            //     }
            // }
            // if(!alreadyVisited)
            // {
            //     queue.push(neighbor);
            //     visited.push_back(neighbor);
            // }

            if(visited.find(neighbor)== visited.end())
            {
                queue.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
}

void Graph::dfs(GraphNode* startNode)
{
    //14-9-2026 Naar unordered set vector<GraphNode*>visited;
    //vector<GraphNode*> visited;
    unordered_set<GraphNode*> visited; 

    dfsRecursive(startNode, visited);
}

void Graph::dfsRecursive(GraphNode* currentNode, unordered_set<GraphNode*>&visited)
{
    //1d-9-26: ipv visited.push_back(startNode) wordt dit
    visited.insert(currentNode);
    
    cout<<"Visited Node: "<<currentNode->getId()<<endl;

    //12-9-26: Dit hele stuk (geneste forloop en if statement wordt vervangen door 1 if statement)
    for(const Edge& edge:currentNode->getEdges())
    {
         GraphNode* neighbor = edge.destination;
         //14-9-2026: eruit vanwege unordered mapbool alreadyVisited = false;
         //for(GraphNode* visitedNode:visited)
        //  {
        //      if(visitedNode == neighbor)
        //      {
        //          alreadyVisited = true;
        //          break;
        //      }
        //  }
        //  if(!alreadyVisited)
        //  {
        //      dfsRecursive(neighbor, visited);
        //  }
        //14-9-2026: nieuw
        if(visited.find(neighbor) == visited.end())
        {
            dfsRecursive(neighbor, visited);
        }

    //12-9-26: omdat we een set gebruiken 
    //if(visited.find(neighbor)==visited.end())
    //{
    //   queue.push(neighbor);
    //  visited.insert(neighbor);

    }
}

//4-9-26: We zitten al in de class Graph dus we hoeven niet los de adjacency list te importeren. We kunnen dara al bij
DijkstraResult Graph::dijkstra(GraphNode* startNode)
{
    //3-9-26: Initialiseer een vector distances die de omvang heeft van het aantal nodes en en geef de waarden infinity
    // Dus initialiseer alles op infinity
    //14-9-2026: Dit gewijzigd naar een unordered_map: std::vector<double> distances(nodes.size(), std::numeric_limits<double>::infinity());
    unordered_map<GraphNode*, double> distances;

    //14-9-26: Schijnbaar dit er bij zetten
    for(GraphNode* node : nodes)
    {
        distances[node] = std::numeric_limits<double>::infinity();
    }



    //7-9-26: Declareer en initaliseer previous
    // std::vector<GraphNode*> previous(nodes.size(), nullptr);
    //14-9-26: w
    distances[startNode] = 0.0;

    //Hier zetten we de eerste node op 0 (want vanuit daar berekenen)
    // wordt anders distances[startNode->getId()] = 0;
    //14-9-26: moet weg en wordt distances[currentNode];
    // vector<GraphNode*> previous(nodes.size(), nullptr);
    unordered_map<GraphNode*, GraphNode*>previous;

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
        //14-9-26: Wijzigt ook van if (currentDistance > distances[currentNode->getId()])
        if(currentDistance>distances[currentNode])
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
            // if(newDistance<distances[edge.destination->getId()])
            if(newDistance<distances[edge.destination])
            {
                //Neem dan de nieuwe afstand
                // distances[edge.destination->getId()] = newDistance;
                distances[edge.destination] = newDistance;

                //7-9-26 Van welke node kwamen wij toen we de beste route vonden naar huidige node
                //14-9-26 previous[edge.destination->getId()] = currentNode; wordt
                previous[edge.destination]=currentNode;

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
//14-9-26: Dit wordt dus een unordered map vector<GraphNode*> Graph::reconstructPath(GraphNode* startNode, GraphNode* targetNode, const vector<GraphNode*>& previous)
vector<GraphNode*> Graph::reconstructPath(GraphNode* startNode, GraphNode* targetNode, const unordered_map<GraphNode*,GraphNode*>& previous)
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

        //14-9-26: Gewijzigd: currentNode = previous[currentNode->getId()];
        // Gebruik at currentNode = previous.at(currentNode);

        auto it = previous.find(currentNode);

        if(it==previous.end())
        {
            path.clear();
            return path;
        }
        currentNode = it->second;

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

//15-9-2026: maake een functie connectKNearestNeighbors en geef als input een referentie genaamd tree naar de
// KD tree, die we niet mogen aanpassen en een variabele k van size_t
void Graph::connectKNearestNeighbors(const KDTree& tree, std::size_t k)
{
    //Doorloop de graphnodes met een pointer genaamd node
    for(GraphNode* node:nodes)
    // Zoek de k dichtstbijzijnde neighbors van de vector van node
    // via de KD-tree en sla deze op in een vector van const Vector-pointers
    // genaamd neighbors.
    {
        //16-9-2026: kleine wijziging: k+1 ipv vector<const Vector*> neighbors = tree.kNearestNeighbors(node->vector,k);
        vector<const Vector*> neighbors = tree.kNearestNeighbors(node->vector,k+1);
        // Doorloop alle gevonden neighbors met een pointer
        // genaamd neighbor naar een const Vector.
        for(const Vector* neighbor:neighbors)
        {
            // Doorloop alle GraphNodes met een pointer
            // genaamd neighborNode naar een GraphNode.
            for(GraphNode* neighborNode:nodes)
            {
            // Controleer of het adres van de Vector in neighborNode
            // hetzelfde is als de Vector-pointer neighbor.
                //16-9-2026: Vervang dit if(&neighborNode->vector==neighbor) door
                if(neighborNode->vector == *neighbor)
                // Bereken de afstand tussen node->vector en neighbor.
                {
                    if(neighborNode->vector == node->vector)
                    {
                        continue;
                    }
                    double distance = node->vector.distanceTo(*neighbor);
                    // Voeg een edge toe van node naar neighborNode
                    // met distance als gewicht.
                    addEdge(node,neighborNode,distance);

                    //Stop met het zoeken naar deze neighbor
                    break;
                }
            }
        }
    }
}

//16-9-2026: print nearestNeighbor
void Graph::printEdges() const
{
    cout << "\nKNN Graph edges:\n";

    for(const GraphNode* node : nodes)
    {
        for(const Edge& edge : node->getEdges())
        {
            cout << node->getId()
                 << " -> "
                 << edge.destination->getId()
                 << " | distance = "
                 << edge.weight
                 << endl;
        }
    }
}



//7-9-26: Destructor
Graph::~Graph()
{
    for (GraphNode* node : nodes)
    {
        delete node;
    }
}
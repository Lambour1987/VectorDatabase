//18-8-2026

#include "../include/Vector.h"
#include "../include/VectorDatabase.h"
#include "../include/Input.h"
#include "../include/DatabaseInput.h"
#include "../include/DatabaseQuery.h"
#include "../include/MaxHeap.h"
#include "../include/KDTree.h"
#include "../include/Graph.h"
#include "../include/MinHeap.h"

//Voor throw exception, try en catch()
#include <stdexcept>
#include <iostream>
#include <string>

//Hiermee kunnen we informatie opvragen over een datatype. Bijv. wat is de grootste integer.
#include <limits>

using namespace std;


int main()
{
    // //1 Database voor het hele programma
    // VectorDatabase database; 

    // //Vraag hoeveel vectors de gebruiker wil invoeren
    // int aantalVectors = vraagInt("Hoeveel vectors wil je invoeren? ");

    // //20-8-26: controle
    // if(aantalVectors <= 0)
    // {
    //     cout << "Aantal vectors moet groter zijn dan 0." << endl;
    //     return 1;
    // }

    // vulDatabase(database, aantalVectors);

    // cout <<"\nAantal vectors in database: "<<database.size()<<endl;

    // KDTree tree(database.getVectors());

    // tree.printTree();
    // voerQueryUit(database);

    // MaxHeap heap;

    // Vector v1({1, 2, 3});
    // Vector v2({4, 5, 6});
    // Vector v3({7, 8, 9});

    // heap.push({10.0, &v1});
    // heap.push({20.0, &v2});
    // heap.push({50.0, &v3});

    // while(!heap.empty())
    // {
    //     auto result = heap.top();

    //     if(result.has_value())
    //     {
    //         cout << "Top distance: " << result.value().first << endl;
    //     }

    //     heap.pop();
    // }

    //Maak 3 vectoren aan
    Vector v1({1,2,3});
    Vector v2({4,5,6});
    Vector v3({7,8,9});
    Vector v4({10,11,12});
    Vector v5({13,14,15});
    Vector v6({16,17,18});


    Graph graph;

    GraphNode* A = graph.addNode(v1);
    GraphNode* B = graph.addNode(v2);
    GraphNode* C = graph.addNode(v3);
    GraphNode* D = graph.addNode(v4);
    GraphNode* E = graph.addNode(v5);
    GraphNode* F = graph.addNode(v6);
    GraphNode* G = graph.addNode(Vector({19,20,21}));

    graph.addEdge(A, B, 10.0);
    graph.addEdge(A, C, 3.5);
    graph.addEdge(C, B, 2.0);
    graph.addEdge(B, D, 4.0);
    graph.addEdge(B, E, 5.0);
    graph.addEdge(C, F, 1.0);

//    for (const Edge& edge : A->getEdges())
//     {
//         GraphNode* neighbor = edge.destination;
//         cout << "A -> Node " << neighbor->getId()
//              << " | weight: " << edge.weight << endl;
//     }

//     for (const Edge& edge : B->getEdges())
//     {
//         GraphNode* neighbor = edge.destination;
//         cout << "B -> Node " << neighbor->getId()
//              << " | weight: " << edge.weight << endl;

//     }

//     cout << "ID A: " << A->getId() << endl;
//     cout << "ID B: " << B->getId() << endl;
//     cout << "ID C: " << C->getId() << endl;

//     //3-9-26: BFS en DFS test
//     cout << "\nBFS:" << endl;
//     graph.bfs(A);

//     cout << "\nDFS:" << endl;
//     graph.dfs(A);

    // MinHeap heap;

    // heap.push({5, nullptr});
    // heap.push({2, nullptr});
    // heap.push({8, nullptr});
    // heap.push({1, nullptr});
    // heap.push({4, nullptr});

    // while (!heap.empty())
    // {
    //     auto value = heap.top();

    //     if (value)
    //     {
    //         std::cout << value->first << std::endl;
    //     }

    //     heap.pop();
    // }

    //7-9-26: Dijkstra oproepen
    cout << "\nA*:" << endl;
    //graph.dijkstra(A);

    DijkstraResult result = graph.dijkstra(A);

    AStarResult aStarResult = graph.aStar(A,F);

    vector<GraphNode*> aStarPath = graph.reconstructPath(A,B,result.previous);

    cout << "\nPath A -> B: ";

    for (GraphNode* node : aStarPath)
    {
        cout << node->getId() << " ";
    }

    cout << endl;

    vector<GraphNode*> pathF =
    graph.reconstructPath(A, F, result.previous);

    cout << "Path A -> F: ";

    for (GraphNode* node : pathF)
    {
        cout << node->getId() << " ";
    }

    cout << endl;

    vector<GraphNode*> pathG =
    graph.reconstructPath(A, G, result.previous);

    cout << "Path A -> G: ";

    for (GraphNode* node : pathG)
    {
        cout << node->getId() << " ";
    }

    cout << endl;
    return 0;
}



    

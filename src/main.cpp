//18-8-2026

#include "../include/Vector.h"
#include "../include/VectorDatabase.h"
#include "../include/Input.h"
#include "../include/DatabaseInput.h"
#include "../include/DatabaseQuery.h"
#include "../include/MaxHeap.h"
#include "../include/KDTree.h"
#include "../include/Graph.h"


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
    
    graph.addEdge(A, B);
    graph.addEdge(A, C);
    graph.addEdge(B, D);
    graph.addEdge(B, E);
    graph.addEdge(C, F);

   for (GraphNode* neighbor : A->getNeighbors())
    {
        cout << "A -> Node " << neighbor->getId() << endl;
    }

    for (GraphNode* neighbor : B->getNeighbors())
    {
        cout << "B -> Node " << neighbor->getId() << endl;
    }

    cout << "ID A: " << A->getId() << endl;
    cout << "ID B: " << B->getId() << endl;
    cout << "ID C: " << C->getId() << endl;

    //3-9-26: BFS en DFS test
    cout << "\nBFS:" << endl;
    graph.bfs(A);

    cout << "\nDFS:" << endl;
    graph.dfs(A);


    return 0;
}



    

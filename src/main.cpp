//18-8-2026

#include "../include/Vector.h"
#include "../include/VectorDatabase.h"
#include "../include/Input.h"
#include "../include/DatabaseInput.h"
#include "../include/DatabaseQuery.h"
#include "../include/MaxHeap.h"


//Voor throw exception, try en catch()
#include <stdexcept>
#include <iostream>
#include <string>

//Hiermee kunnen we informatie opvragen over een datatype. Bijv. wat is de grootste integer.
#include <limits>

using namespace std;


int main()
{
    //1 Database voor het hele programma
    VectorDatabase database; 

    //Vraag hoeveel vectors de gebruiker wil invoeren
    int aantalVectors = vraagInt("Hoeveel vectors wil je invoeren? ");

    //20-8-26: controle
    if(aantalVectors <= 0)
    {
        cout << "Aantal vectors moet groter zijn dan 0." << endl;
        return 1;
    }

    vulDatabase(database, aantalVectors);

    cout <<"\nAantal vectors in database: "<<database.size()<<endl;

    voerQueryUit(database);

    MaxHeap heap;

    Vector v1({1, 2, 3});
    Vector v2({4, 5, 6});
    Vector v3({7, 8, 9});

    heap.push({10.0, &v1});
    heap.push({20.0, &v2});
    heap.push({50.0, &v3});

    while(!heap.empty())
    {
        auto result = heap.top();

        if(result.has_value())
        {
            cout << "Top distance: " << result.value().first << endl;
        }

        heap.pop();
    }


    return 0;
}



    

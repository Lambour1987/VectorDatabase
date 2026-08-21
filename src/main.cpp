//18-8-2026

#include "../include/Vector.h"
#include "../include/VectorDatabase.h"
#include "../include/Input.h"
#include "../include/DatabaseInput.h"
#include "../include/DatabaseQuery.h"

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

    return 0;
}



    

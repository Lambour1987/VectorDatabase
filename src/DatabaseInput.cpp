//21-8-2026


#include "../include/DatabaseInput.h"
#include "../include/Input.h"

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

void vulDatabase(VectorDatabase& database, int aantalVectors)
{
    // Bepaalt hoevel vectors in de database komen
    for(int i = 0; i < aantalVectors; i++)
    {
        cout<< "\nVector "<<i+1<<endl;

        // Deze Vector opnieuw proberen totdat hij succesvol is aangemaakt
        bool vectorGemaakt = false;

        while(!vectorGemaakt)
        {
            vector<double> inputValues;

            //Vraag dimensies van deze vector
            for(size_t j=0; j<Vector::MAX_DIMENSIONS;j++)
            {
                // Voeg aan queryValues de double functie toe die een vraagDouble() teruggeeft, waarbij de gebruiker wordt
                // gevraagd om een waarde nummer i+1
                inputValues.push_back(vraagDouble("Geef waarde "+ to_string(j+1)+": "));
            };

            try
            { 
                // Gewijzigd van Vector v({x, y, z}); naar
                Vector v(inputValues);
                database.add(v);
                cout<<"Vector toegevoegd! "<<endl;
                cout<<"Aantal vectors: "<<database.size()<<endl;
                //Vector Succesvol aangemaakt, dus true
                vectorGemaakt = true;

            }
            catch(const invalid_argument& e)
            {
                cout<<e.what()<<endl;
                cout<<"Probeer deze Vector opnieuw. "<<endl;
            }
        }
    }
}
//18-8-2026

#include "../include/Vector.h"
#include "../include/VectorDatabase.h"
#include "../include/Input.h"

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
    cout <<"\nAantal vectors in database: "<<database.size()<<endl;

    //19-8-2026: 
    //maak een vector van doubles en noem deze queryValues
    vector<double> queryValues;
    //Doorloop alle vectoren. Voeg aan de vector queryValues toe de uitkomst van de vraagDouble functie...
    for(size_t i=0; i<Vector::MAX_DIMENSIONS; i++)
    {
        queryValues.push_back(vraagDouble("Geef waarde " + to_string(i+1)+ ": "));
    }
    //try (probeer iets)
    try
    {
        //Maak een nieuw Vector object met de naam query en gebruik queryValues als invoer voor de constructor
        Vector query(queryValues);
        // Zoek in de VectorDatabase de dichtstbijzijnde Vector op basis van de query
        // en maak een variabele 'nearest' die een niet te wijzigen referentie
        // naar die Vector bevat.
        //19-8-26: aangepast const Vector& nearest = database.findNearest(query); naar
        NearestResult result = database.findNearest(query);

        //Bericht: Dichtstbijzijnde vector:
        cout<<"Dichtstbijzijnde vector: "<<endl;
        //Loop door de dimensies heen van de vector die het dichtst bij staat.
        //19-8-26: ook aangepast: for(size_t i=0;i<nearest.dimension();i++)
        //20-8-26: aangepast naar pointers van for(size_t i =0; i<result.vector.dimension();i++) naar
        for(size_t i =0; i<result.vector->dimension();i++)
        {
            //Print de waarden van deze vector
            cout<<result.vector->at(i)<<endl;
        }

        //20-8-2026
        // Roep de functie vraagInt op en geef de tekst "Hoeveel dichtstbijzijnde vectors wil je? " mee als argument en sla
        //20-8-2026 wijzig van size_t k = vraagInt("Hoeveel dichtsbijzijnde vectors wil je? "); naar
        int kInput = vraagInt("Hoeveel dichtstbijzijnde vectors wil je? ");

        if(kInput<=0)
        {
            cout<<"Aantal moet groter zijn dan 0. "<<endl;
            return 1;
        }

        size_t k = static_cast<size_t>(kInput);

        // Roep op het object database de memberfunctie findNearestK aan en geef query en k mee als argumenten. 
        // Sla het resultaat op in de variabele results, die een std::vector<NearestResult> is.
        vector<NearestResult> results = database.findNearestK(query, k);

        // Weergeef bericht: alle gevonden resultaten
        cout<<"\n Alle gevonden resultaten:\n";

        // Loop door alle resultaten van results heen (refereer zonder te wijzigen)
        for(const NearestResult& result : results)
        {
            //Bericht: "Afstand" en geef de afstand door de memberfunctie distance op de result op te roepen
            cout<<"Afstand: "<<result.distance<<endl;
            //Loop door alle dimensies van de Vector die in result.vector zit.
            for(size_t i =0; i<result.vector->dimension();i++)
            {
                //Bericht: geef het resultaat van de vector op index i
                cout<< result.vector->at(i)<<" ";
            }
            cout<<endl;
        }
    }    
        
    //catch: Als er een runtime_error wordt gegooid, vang die exception op via een const reference met de naam e.
    catch(const runtime_error&e)
    {
        //print de foutmelding
        cout<<e.what()<<endl;
    }


    //19-8-26: Haal eerste element op en sla deze op in 'eersteVector'. 
    //Pak de eerste Vector uit de database en geef mij daar een constante referentie naar en noem die eersteVector.
    //BELANGRIJK: Of kopie maken (duur), of referentie muteren of lezen (dan const), of pointer ('link' naar object), tevens te muteren of alleen lezen (const)
    const Vector& eersteVector = database.get(0);

    return 0;
    
}



    

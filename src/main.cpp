//18-8-2026

#include "../include/Vector.h"
#include "../include/VectorDatabase.h"
#include "../include/Knowledgeitem.h"
#include "../include/VectorLoader.h"
#include "../include/TextLoader.h"
#include "../include/DatabaseStorage.h"
#include "../include/BTreeTest.h"
#include "GraphTest.h"

//Voor throw exception, try en catch()
#include <stdexcept>
#include <iostream>
#include <vector>

using namespace std;


int main()
{
    // //1 Database voor het hele programma
    VectorDatabase database; 

    auto loadedVectors = loadVectorsFromFile("embedding/vectors.txt");
    auto loadedTexts = loadTextsFromFile("embedding/texts.txt");

    // Controle: hebben we evenveel vectors als teksten?
    if(loadedVectors.size() != loadedTexts.size())
    {
        throw std::runtime_error("Aantal vectors en teksten komt niet overeen.");
    }

    // // Maak KnowledgeItems
    // std::vector<KnowledgeItem> knowledgeItems;

    // for(std::size_t i = 0; i < loadedVectors.size(); ++i)
    // {
    //     //weg?KnowledgeItem item(Vector(loadedVectors[i]),loadedTexts[i]);
    //     knowledgeItems.push_back(item);
    // }

    // // Voeg alle KnowledgeItems toe aan de VectorDatabase
    // for(const auto& item : knowledgeItems)
    // {
    //    database.add(Vector(loadedVectors[i]), loadedTexts[i]);
    // }

    for(std::size_t i = 0; i < loadedVectors.size(); ++i)
    {
    database.add(Vector(loadedVectors[i]), loadedTexts[i]);
    }

    cout << "Items in VectorDatabase: " << database.size() << endl;

    //5-10-2026 Tijdelijk toevoegen:
    DatabaseStorage storage;

    storage.save(database, "database.dat");

    VectorDatabase loadedDatabase = storage.load("database.dat");

    cout << "Originele database: " << database.size() << " items" << endl;
    cout << "Geladen database:   " << loadedDatabase.size() << " items" << endl;

    cout << "\nOriginele tekst: "
        << database.get(0).text << endl;

    cout << "Geladen tekst:   "
        << loadedDatabase.get(0).text << endl;

    cout << "Originele dimensie: "
        << database.get(0).vector.dimension() << endl;

    cout << "Geladen dimensie:   "
        << loadedDatabase.get(0).vector.dimension() << endl;

    if(database.get(0).vector == loadedDatabase.get(0).vector)
    {
        cout << "Vector komt overeen!" << endl;
    }
    else
    {
        cout << "Vector wijkt af!" << endl;
    }

    //1-10-2026: TIjdelijk toevoegen
    //7-10-2026: Aangepast
    for(std::size_t i = 0; i < database.size(); ++i)
    {
        const auto& item = database.get(i);

        cout << "Item " << i << ":" << endl;
        cout << "ID: " << item.id << endl;
        cout << "Tekst: " << item.text << endl;
        cout << "Dimensies: " << item.vector.dimension() << endl;
    }

    //1-10-2026: tijdelijk
    auto queryVectors = loadVectorsFromFile("embedding/query_vector.txt");

    Vector externalQuery(queryVectors[0]);

    //1-10-26: Threshold: resultaten die volledig buiten query vallen hoeven niet genoemd te zijn.
    double threshold = 0.8;

    vector<NearestResult> queryResults =
        database.findNearestK(externalQuery, 3);

    if(queryResults[0].distance <= threshold)
    {
        cout << "Relevante kennis gevonden:" << endl;
        cout << queryResults[0].item->text << endl;
    }
    else
    {
    cout << "Geen relevante kennis gevonden." << endl;
    }

    cout << "\nResultaten externe query:" << endl;

    for(size_t i = 0; i < queryResults.size(); ++i)
    {
        cout << i + 1 << ". "
            << queryResults[i].item->text
            << " | Afstand: "
            << queryResults[i].distance
            << endl;
    }

    runGraphTests();
    runBTreeTests();

    return 0;
}



    

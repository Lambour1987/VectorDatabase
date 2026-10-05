//5-10-2026: Serialization en Deserialization: 
//5-10-2026: Voetnoot: size_t en double worden nu in hun native binaire representatie opgeslagen.

#include "../include/DatabaseStorage.h"
#include <fstream>
#include <stdexcept>

using namespace std;

// Save functie
void DatabaseStorage::save(const VectorDatabase& database, const string& filename) const
{
    //Output naar een bestand, behandel het bestand als binaire data.
    ofstream file(filename, ios::binary);

    //Check of het gelukt is
    if(!file)
    {
        throw runtime_error("Could not open database file for writing");
    }

    //Aantal Items dat we gaan opslaan
    size_t itemCount = database.size();

    // Neem het geheugenadres van itemCount, bekijk de bytes die daar staan als een
    // reeks van char-bytes en schrijf precies sizeof(itemCount) bytes naar het bestand
    // Vergelijkbaar met Multiplexing/ Demultiplexing
    // Dus: Schrijf het aantal items naar het bestand
    file.write(reinterpret_cast<const char*>(&itemCount),
           sizeof(itemCount));


    // Loop door alle KnowLedgeItems in de database
    for(size_t i = 0; i < database.size(); i++)
    {
        const KnowledgeItem& item = database.get(i);

        //Bepaal hoeveel bytes de tekst inneemt
        size_t textLength = item.text.size();

        // Schrijf de lengte van de tekst naar het bestand
        file.write(reinterpret_cast<const char*>(&textLength),
               sizeof(textLength));

        //Schrijf de daadwerkelijke tekst als bytes naar het bestand
        file.write(item.text.data(), textLength);

        // Bepaal hoeveel dimensies de vector heeft
        // Wel slaan dit op zodat de loader later weet hoeveel vectorwaarden hij moet lezen
        size_t dimension = item.vector.dimension();

        // Schrijf de vectordimensie naar het bestand.
        file.write(reinterpret_cast<const char*>(&dimension),
        sizeof(dimension));

         // Loop door alle dimensies van de vector.
        for(size_t j = 0; j < dimension; j++)
        {
            double value = item.vector.at(j);

            // Schrijf de binaire bytes van deze double naar het bestand.
            file.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
    }
}

VectorDatabase DatabaseStorage::load(const string& filename) const
{
    // Open het binaire databasebestand om te lezen.
    ifstream file(filename, ios::binary);

    //Controleer of het bestand geopend kan worden
    if(!file)
    {
        throw runtime_error("Could not open database file for reading");
    }

    // Hierin wordt het aantal opgeslagen items gelezen.
    size_t itemCount;

    // Lees de bytes van itemCount uit het bestand.
    file.read(reinterpret_cast<char*>(&itemCount),sizeof(itemCount));

    VectorDatabase database;

    // Loop door alle opgeslagen KnowledgeItems.
    for(size_t i = 0; i < itemCount; i++)
    {
        // Hierin wordt de lengte van de tekst gelezen.
        size_t textLength;

        // Lees de tekstlengte uit het bestand.
        file.read(reinterpret_cast<char*>(&textLength),sizeof(textLength));

        // Maak een string met precies genoeg ruimte voor de tekst.
        string text(textLength, '\0');

        // Lees de daadwerkelijke tekst uit het bestand.
        file.read(text.data(), textLength);

        size_t dimension;

        file.read(reinterpret_cast<char*>(&dimension),sizeof(dimension));

        vector<double> values(dimension);

        for(size_t j = 0; j < dimension; j++)
        {
            file.read(reinterpret_cast<char*>(&values[j]),sizeof(values[j]));
        }

       Vector vector(values);

        KnowledgeItem item(vector, text);

        database.add(item);

    }
    return database;
}
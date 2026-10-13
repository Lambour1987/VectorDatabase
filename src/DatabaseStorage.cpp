//5-10-2026: Serialization en Deserialization: 
//5-10-2026: Voetnoot: size_t en double worden nu in hun native binaire representatie opgeslagen.

#include "../include/DatabaseStorage.h"
#include "../include/DatabaseStorageInternal.h"
#include <fstream>
#include <stdexcept>
#include <limits>
//Speciaal voor een test save omdat we niet een echte fout kunnen maken in het geheugen simuleren we er een
#include <functional>
#include <ostream>

using namespace std;

//13-10-2026: Helemaal vervangen. Kan we wel als de rest werkt
// Save functie
// void DatabaseStorage::save(const VectorDatabase& database, const string& filename) const
// {
//     //Output naar een bestand, behandel het bestand als binaire data.
//     ofstream file(filename, ios::binary);

//     //Check of het gelukt is
//     if(!file)
//     {
//         throw runtime_error("Could not open database file for writing");
//     }

//     //Aantal Items dat we gaan opslaan
//     size_t itemCount = database.size();

//     // Neem het geheugenadres van itemCount, bekijk de bytes die daar staan als een
//     // reeks van char-bytes en schrijf precies sizeof(itemCount) bytes naar het bestand
//     // Vergelijkbaar met Multiplexing/ Demultiplexing
//     // Dus: Schrijf het aantal items naar het bestand
//     file.write(reinterpret_cast<const char*>(&itemCount),
//            sizeof(itemCount));


//     // Loop door alle KnowLedgeItems in de database
//     for(size_t i = 0; i < database.size(); i++)
//     {
//         const KnowledgeItem& item = database.get(i);

//         //Bepaal hoeveel bytes de tekst inneemt
//         size_t textLength = item.text.size();

//         // Schrijf de lengte van de tekst naar het bestand
//         file.write(reinterpret_cast<const char*>(&textLength),
//                sizeof(textLength));

//         //Schrijf de daadwerkelijke tekst als bytes naar het bestand
//         file.write(item.text.data(), textLength);

//         // Bepaal hoeveel dimensies de vector heeft
//         // Wel slaan dit op zodat de loader later weet hoeveel vectorwaarden hij moet lezen
//         size_t dimension = item.vector.dimension();

//         // Schrijf de vectordimensie naar het bestand.
//         file.write(reinterpret_cast<const char*>(&dimension),
//         sizeof(dimension));

//          // Loop door alle dimensies van de vector.
//         for(size_t j = 0; j < dimension; j++)
//         {
//             double value = item.vector.at(j);

//             // Schrijf de binaire bytes van deze double naar het bestand.
//             file.write(reinterpret_cast<const char*>(&value), sizeof(value));
//         }
//     }
// }

//13-10-2026: OOK DEZE EVEN LATEN STAAN ALS VOLGENDE NIET WERKT
// void DatabaseStorage::save(const VectorDatabase& database, const string& filename) const
// {
//     ofstream file(filename, ios::binary);

//     if (!file)
//     {
//         throw runtime_error("Could not open database file for writing");
//     }

//     const size_t itemCount = database.size();

//     file.write(reinterpret_cast<const char*>(&itemCount), sizeof(itemCount));

//     for (size_t i = 0; i < itemCount; i++)
//     {
//         const KnowledgeItem& item = database.get(i);

//         const size_t textLength = item.text.size();
//         file.write(reinterpret_cast<const char*>(&textLength), sizeof(textLength));

//         if (textLength > 0)
//         {
//             file.write(item.text.data(), static_cast<streamsize>(textLength));
//         }

//         const size_t dimension = item.vector.dimension();
//         file.write(reinterpret_cast<const char*>(&dimension), sizeof(dimension));

//         for (size_t j = 0; j < dimension; j++)
//         {
//             const double value = item.vector.at(j);
//             file.write(reinterpret_cast<const char*>(&value), sizeof(value));
//         }

//         if (!file)
//         {
//             throw runtime_error("Failed while writing database data");
//         }
//     }

//     file.flush();

//     if (!file)
//     {
//         throw runtime_error("Failed to flush database file");
//     }
// }

namespace DatabaseStorageInternal
{
    //using WriteFunction = function<void(ostream&)>;

    void writeDatabase(const VectorDatabase& database, ostream& output)
    {
        const size_t itemCount = database.size();

        output.write(reinterpret_cast<const char*>(&itemCount), sizeof(itemCount));

        for (size_t i = 0; i < itemCount; i++)
        {
            const KnowledgeItem& item = database.get(i);

            const size_t textLength = item.text.size();
            output.write(reinterpret_cast<const char*>(&textLength), sizeof(textLength));

            if (textLength > 0)
            {
                output.write(item.text.data(), static_cast<streamsize>(textLength));
            }

            const size_t dimension = item.vector.dimension();
            output.write(reinterpret_cast<const char*>(&dimension), sizeof(dimension));

            for (size_t j = 0; j < dimension; j++)
            {
                const double value = item.vector.at(j);
                output.write(reinterpret_cast<const char*>(&value), sizeof(value));
            }

            if (!output)
            {
                throw runtime_error("Failed while writing database data");
            }
        }

        output.flush();

        if (!output)
        {
            throw runtime_error("Failed to flush database file");
        }
    }
}

void DatabaseStorage::save(const VectorDatabase& database, const string& filename) const
{
    ofstream file(filename, ios::binary);

    if (!file)
    {
        throw runtime_error("Could not open database file for writing");
    }

    //writeDatabase(database, file);
    DatabaseStorageInternal::writeDatabase(database, file);
}


// 13-10-2026: Helemaal vervangen: kan weg als de rest blijft werken
// VectorDatabase DatabaseStorage::load(const string& filename) const
// {
//     // Open het binaire databasebestand om te lezen.
//     ifstream file(filename, ios::binary);

//     //Controleer of het bestand geopend kan worden
//     if(!file)
//     {
//         throw runtime_error("Could not open database file for reading");
//     }
//     // Hierin wordt het aantal opgeslagen items gelezen.
//     //13-10-2026: gewijzigd size_t itemCount;
//     // Lees de bytes van itemCount uit het bestand.
//     //13-10-26: gewijzigdfile.read(reinterpret_cast<char*>(&itemCount),sizeof(itemCount));

//     size_t itemCount = 0;

//     if (!file.read(reinterpret_cast<char*>(&itemCount), sizeof(itemCount)))
//     {
//         throw runtime_error("Invalid database file: could not read item count");
//     }

//     VectorDatabase database;

//     // Loop door alle opgeslagen KnowledgeItems.
//     for(size_t i = 0; i < itemCount; i++)
//     {
//         // Hierin wordt de lengte van de tekst gelezen.
//         size_t textLength;

//         // Lees de tekstlengte uit het bestand.
//         file.read(reinterpret_cast<char*>(&textLength),sizeof(textLength));

//         // Maak een string met precies genoeg ruimte voor de tekst.
//         string text(textLength, '\0');

//         // Lees de daadwerkelijke tekst uit het bestand.
//         file.read(text.data(), textLength);

//         size_t dimension;

//         file.read(reinterpret_cast<char*>(&dimension),sizeof(dimension));

//         vector<double> values(dimension);

//         for(size_t j = 0; j < dimension; j++)
//         {
//             file.read(reinterpret_cast<char*>(&values[j]),sizeof(values[j]));
//         }

//        Vector vector(values);

//         //Kanweg KnowledgeItem item(vector, text);

//         database.add(vector, text);

//     }
//     return database;
//}


VectorDatabase DatabaseStorage::load(const string& filename) const
{
    ifstream file(filename, ios::binary);

    if (!file)
    {
        throw runtime_error("Could not open database file for reading");
    }

    // Veiligheidslimieten voor onbetrouwbare bestandsgegevens.
    constexpr size_t MAX_ITEMS = 1'000'000;
    constexpr size_t MAX_TEXT_LENGTH = 10 * 1024 * 1024; // 10 MiB
    constexpr size_t MAX_DIMENSION = 1'000'000;

    size_t itemCount = 0;

    if (!file.read(reinterpret_cast<char*>(&itemCount), sizeof(itemCount)))
    {
        throw runtime_error("Invalid database file: could not read item count");
    }

    if (itemCount > MAX_ITEMS)
    {
        throw runtime_error("Invalid database file: too many items");
    }

    VectorDatabase database;

    for (size_t i = 0; i < itemCount; i++)
    {
        size_t textLength = 0;

        if (!file.read(reinterpret_cast<char*>(&textLength), sizeof(textLength)))
        {
            throw runtime_error("Invalid database file: could not read text length");
        }

        if (textLength > MAX_TEXT_LENGTH)
        {
            throw runtime_error("Invalid database file: text length exceeds limit");
        }

        string text(textLength, '\0');

        if (textLength > 0 &&
            !file.read(text.data(), static_cast<streamsize>(textLength)))
        {
            throw runtime_error("Invalid database file: incomplete text data");
        }

        size_t dimension = 0;

        if (!file.read(reinterpret_cast<char*>(&dimension), sizeof(dimension)))
        {
            throw runtime_error("Invalid database file: could not read vector dimension");
        }

        if (dimension > MAX_DIMENSION)
        {
            throw runtime_error("Invalid database file: vector dimension exceeds limit");
        }

        // Controleer ook of de benodigde vectorbytes binnen streamsize passen.
        if (dimension > static_cast<size_t>(
                numeric_limits<streamsize>::max() / sizeof(double)))
        {
            throw runtime_error("Invalid database file: vector data is too large");
        }

        vector<double> values(dimension);

        for (size_t j = 0; j < dimension; j++)
        {
            if (!file.read(reinterpret_cast<char*>(&values[j]), sizeof(values[j])))
            {
                throw runtime_error("Invalid database file: incomplete vector data");
            }
        }

        Vector vector(values);
        database.add(vector, text);
    }

    return database;
}

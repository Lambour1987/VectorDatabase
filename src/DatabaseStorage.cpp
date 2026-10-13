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

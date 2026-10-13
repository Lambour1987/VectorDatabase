//13-10-2026: nu pas heir aangemaakt. Staat ook in main?

#include "../include/DatabaseStorage.h"
#include "../include/DatabaseStorageInternal.h"
#include <algorithm>
#include <ostream>
#include <streambuf>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;


//13-10-2026 Hele class nog toegevoegd:

class FailingStreambuf : public streambuf
{
public:
    explicit FailingStreambuf(streamsize bytesBeforeFailure)
        : remaining_(bytesBeforeFailure)
    {
    }

protected:
    streamsize xsputn(const char*, streamsize count) override
    {
        if (remaining_ <= 0)
        {
            return 0;
        }

        const streamsize accepted = min(count, remaining_);
        remaining_ -= accepted;
        return accepted;
    }

    int_type overflow(int_type ch = traits_type::eof()) override
    {
        if (traits_type::eq_int_type(ch, traits_type::eof()))
        {
            return traits_type::not_eof(ch);
        }

        if (remaining_ <= 0)
        {
            return traits_type::eof();
        }

        --remaining_;
        return ch;
    }

private:
    streamsize remaining_;
};

int main()
{
    DatabaseStorage storage;
    const string testFile = "DatabaseStorageTest_temp.dat";
    const string missingFile = "DatabaseStorageTest_missing.dat";
    const string emptyFile = "DatabaseStorageTest_empty.dat";

    // 1. Normaal opslaan en laden
    VectorDatabase original;
    original.add(Vector{1.0, 2.0, 3.0}, "eerste tekst");
    original.add(Vector{4.5, 5.5}, "tweede tekst");

    storage.save(original, testFile);

    VectorDatabase loaded = storage.load(testFile);

    assert(loaded.size() == 2);
    assert(loaded.get(0).text == "eerste tekst");
    assert(loaded.get(1).text == "tweede tekst");
    assert(loaded.get(0).vector == original.get(0).vector);
    assert(loaded.get(1).vector == original.get(1).vector);
    assert(loaded.get(0).id == 1);
    assert(loaded.get(1).id == 2);

    cout << "PASS: normal save/load roundtrip\n";

    //13-10-2026: weer wat tussengevoegd
    // 2. Een bestand met extra bytes achter de database moet worden afgewezen
    const string trailingDataFile = "DatabaseStorageTest_trailing.dat";
    storage.save(original, trailingDataFile);

    {
        FILE* file = std::fopen(trailingDataFile.c_str(), "ab");
        assert(file != nullptr);

        const char extraData[] = {0x12, 0x34, 0x56};
        std::fwrite(extraData, sizeof(extraData), 1, file);
        std::fclose(file);
    }

    bool trailingDataRejected = false;

    try
    {
        storage.load(trailingDataFile);
    }
    catch (const runtime_error&)
    {
        trailingDataRejected = true;
    }

    assert(trailingDataRejected);
    cout << "PASS: trailing data rejected\n";
    std::remove(trailingDataFile.c_str());



    // 2. Een niet-bestaand bestand moet een fout opleveren
    std::remove(missingFile.c_str());

    bool missingFileRejected = false;

    try
    {
        storage.load(missingFile);
    }
    catch (const runtime_error&)
    {
        missingFileRejected = true;
    }

    assert(missingFileRejected);
    cout << "PASS: missing file rejected\n";

    // 3. Een leeg bestand moet veilig worden afgewezen
    {
        FILE* file = std::fopen(emptyFile.c_str(), "wb");
        assert(file != nullptr);
        std::fclose(file);
    }

    bool emptyFileRejected = false;

    try
    {
        storage.load(emptyFile);
    }
    catch (const runtime_error&)
    {
        emptyFileRejected = true;
    }

    assert(emptyFileRejected);
    cout << "PASS: empty file rejected\n";

    //13-10-2026: nieuw tekstblok voor test

    // 4. Een bestand met een onvolledige itemtelling moet worden afgewezen
    const string truncatedFile = "DatabaseStorageTest_truncated.dat";

    {
        FILE* file = std::fopen(truncatedFile.c_str(), "wb");
        assert(file != nullptr);

        // Schrijf slechts 2 bytes, minder dan een volledige size_t.
        const char partialData[2] = {0, 0};
        std::fwrite(partialData, 1, sizeof(partialData), file);
        std::fclose(file);
    }

    bool truncatedFileRejected = false;

    try
    {
        storage.load(truncatedFile);
    }
    catch (const runtime_error&)
    {
        truncatedFileRejected = true;
    }

    assert(truncatedFileRejected);
    cout << "PASS: truncated item count rejected\n";

    //13-10-2026: Toegevoegd test
    // 5. Een bestand met een geldig itemaantal maar ontbrekende itemdata
    const string incompleteItemFile = "DatabaseStorageTest_incomplete.dat";

    {
        FILE* file = std::fopen(incompleteItemFile.c_str(), "wb");
        assert(file != nullptr);

        // Schrijf een itemaantal van 1, maar geen verdere itemgegevens.
        const size_t itemCount = 1;
        std::fwrite(&itemCount, sizeof(itemCount), 1, file);
        std::fclose(file);
    }

    bool incompleteItemRejected = false;

    try
    {
        storage.load(incompleteItemFile);
    }
    catch (const runtime_error&)
    {
        incompleteItemRejected = true;
    }

    assert(incompleteItemRejected);
    cout << "PASS: incomplete item rejected\n";

    //13-10-2026 Toegevoegd
    // 6. Een onrealistische vectordimensie moet worden afgewezen
    const string oversizedDimensionFile = "DatabaseStorageTest_oversized.dat";

    {
        FILE* file = std::fopen(oversizedDimensionFile.c_str(), "wb");
        assert(file != nullptr);

        const size_t itemCount = 1;
        const size_t textLength = 0;
        const size_t oversizedDimension = 1'000'001;

        std::fwrite(&itemCount, sizeof(itemCount), 1, file);
        std::fwrite(&textLength, sizeof(textLength), 1, file);
        std::fwrite(&oversizedDimension, sizeof(oversizedDimension), 1, file);

        std::fclose(file);
    }

    bool oversizedDimensionRejected = false;

    try
    {
        storage.load(oversizedDimensionFile);
    }
    catch (const runtime_error&)
    {
        oversizedDimensionRejected = true;
    }

    assert(oversizedDimensionRejected);
    cout << "PASS: oversized vector dimension rejected\n";

    std::remove(oversizedDimensionFile.c_str());

    std::remove(incompleteItemFile.c_str());

    std::remove(truncatedFile.c_str());

    // Tijdelijke testbestanden opruimen
    std::remove(testFile.c_str());
    std::remove(emptyFile.c_str());
    std::remove(missingFile.c_str());

    //13-10-2026: toegevoegd
    // 7. Een onrealistisch lange tekst moet worden afgewezen
    const string oversizedTextFile = "DatabaseStorageTest_oversized_text.dat";
    {
        FILE* file = std::fopen(oversizedTextFile.c_str(), "wb");
        assert(file != nullptr);

        const size_t itemCount = 1;
        const size_t oversizedTextLength = 10 * 1024 * 1024 + 1;

        std::fwrite(&itemCount, sizeof(itemCount), 1, file);
        std::fwrite(&oversizedTextLength, sizeof(oversizedTextLength), 1, file);

        std::fclose(file);
    }

    bool oversizedTextRejected = false;

    try
    {
        storage.load(oversizedTextFile);
    }
    catch (const runtime_error&)
    {
        oversizedTextRejected = true;
    }

    assert(oversizedTextRejected);
    cout << "PASS: oversized text length rejected\n";

    std::remove(oversizedTextFile.c_str());

    // 8. Opslaan naar een ongeldige locatie moet een fout opleveren
    const string invalidOutputFile =
        "DatabaseStorageTest_missing_directory\\output.dat";

    bool saveFailureDetected = false;

    try
    {
        storage.save(original, invalidOutputFile);
    }
    catch (const runtime_error&)
    {
        saveFailureDetected = true;
    }

    assert(saveFailureDetected);
    cout << "PASS: save to invalid path rejected\n";

    //13-10-2026: Nog een nieuwe test
    // Test: schrijffout tijdens het serialiseren wordt afgehandeld
    const streamsize bytesBeforeFailure =
        static_cast<streamsize>(sizeof(size_t) * 2 + 3);

    FailingStreambuf failingBuffer(bytesBeforeFailure);
    ostream failingOutput(&failingBuffer);

    bool writeFailureDetected = false;

    try
    {
        DatabaseStorageInternal::writeDatabase(original, failingOutput);
    }
    catch (const runtime_error&)
    {
        writeFailureDetected = true;
    }

    assert(writeFailureDetected);
    cout << "PASS: write failure during serialization rejected\n";

    cout << "All DatabaseStorage tests passed.\n";

    return 0;
}
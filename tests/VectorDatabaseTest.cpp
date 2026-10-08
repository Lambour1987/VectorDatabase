#include "../include/VectorDatabase.h"
#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
    VectorDatabase database;

    Vector vector1{1.0, 2.0, 3.0};
    Vector vector2{4.0, 5.0, 6.0};

    database.add(vector1, "eerste tekst");
    database.add(vector2, "tweede tekst");

    assert(database.size() == 2);

    assert(database.get(0).id == 1);
    assert(database.get(1).id == 2);

    const KnowledgeItem& item = database.getById(2);

    assert(item.id == 2);

    cout << "Found ID 2: "
         << item.text
         << "\n";

    try
    {
        database.getById(999);
        assert(false);
    }
    catch(const runtime_error&)
    {
        cout << "Missing ID correctly detected!\n";
    }
    assert(database.containsId(1));
    assert(database.containsId(2));
    assert(!database.containsId(999));

    cout << "containsId tests passed!" << endl;
    cout << "VectorDatabase tests passed!" << endl;

    return 0;
}
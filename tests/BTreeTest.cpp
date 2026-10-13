//6-10-2026

//6-10-2026

#include "../include/BTree.h"

#include <iostream>
#include <stdexcept>
#include "../include/KnowledgeItem.h"
#include "../include/Vector.h"

using namespace std;

void runBTreeTests()
{
    cout << "\n=== BTree Tests ===" << endl;

    BTree tree;

    // Deze waarden zorgen voor meerdere splitsingen.
    tree.insert(50, nullptr);
    tree.insert(30, nullptr);
    tree.insert(70, nullptr);
    tree.insert(10, nullptr);
    tree.insert(40, nullptr);
    tree.insert(60, nullptr);
    tree.insert(80, nullptr);
    tree.insert(20, nullptr);
    tree.insert(35, nullptr);
    tree.insert(45, nullptr);

    // Print boom
    cout << "\nBTree structure:" << endl;
    tree.print();

    // Waarden die aanwezig moeten zijn.
    if(!tree.contains(50))
    {
        throw runtime_error("BTree test failed: 50 not found");
    }

    if(!tree.contains(10))
    {
        throw runtime_error("BTree test failed: 10 not found");
    }

    if(!tree.contains(45))
    {
        throw runtime_error("BTree test failed: 45 not found");
    }

    if(!tree.contains(80))
    {
        throw runtime_error("BTree test failed: 80 not found");
    }

    // Waarden die NIET aanwezig zijn.
    if(tree.contains(999))
    {
        throw runtime_error("BTree test failed: 999 should not exist");
    }

    if(tree.contains(25))
    {
        throw runtime_error("BTree test failed: 25 should not exist");
    }

    // Test verwijderen uit een leaf.
    tree.remove(20);

    if(tree.contains(20))
    {
        throw runtime_error("BTree test failed: 20 should have been removed");
    }

    cout << "\nBTree after deleting 20:" << endl;
    tree.print();

    BTree mergeTree;

    mergeTree.insert(50, nullptr);
    mergeTree.insert(30, nullptr);
    mergeTree.insert(70, nullptr);
    mergeTree.insert(10, nullptr);
    mergeTree.insert(20, nullptr);
    mergeTree.insert(60, nullptr);
    mergeTree.insert(80, nullptr);
    mergeTree.insert(35, nullptr);
    mergeTree.insert(40, nullptr);
    mergeTree.insert(45, nullptr);

    cout << "\nMerge test - initial tree:" << endl;
    mergeTree.print();

    mergeTree.remove(20);
    mergeTree.remove(40);
    mergeTree.remove(45);

    cout << "\nMerge test - before deleting 30:" << endl;
    mergeTree.print();

    mergeTree.remove(30);

    if(mergeTree.contains(30))
    {
        throw runtime_error(
            "BTree test failed: 30 should have been removed after merge"
        );
    }

    cout << "\nMerge test - after deleting 30:" << endl;
    mergeTree.print();

    BTree nextTree;

    nextTree.insert(50, nullptr);
    nextTree.insert(30, nullptr);
    nextTree.insert(70, nullptr);
    nextTree.insert(10, nullptr);
    nextTree.insert(20, nullptr);
    nextTree.insert(60, nullptr);
    nextTree.insert(80, nullptr);
    nextTree.insert(35, nullptr);
    nextTree.insert(40, nullptr);
    nextTree.insert(45, nullptr);

    cout << "\nBorrow from next - initial tree:" << endl;
    nextTree.print();

    nextTree.remove(20);

    cout << "\nBorrow from next - before deleting 10:" << endl;
    nextTree.print();

    nextTree.remove(10);

    if(nextTree.contains(10))
    {
        throw runtime_error(
            "BTree test failed: 10 should have been removed"
        );
    }

    cout << "\nBorrow from next - after deleting 10:" << endl;
    nextTree.print();

    BTree previousTree;

    previousTree.insert(50, nullptr);
    previousTree.insert(30, nullptr);
    previousTree.insert(70, nullptr);
    previousTree.insert(10, nullptr);
    previousTree.insert(20, nullptr);
    previousTree.insert(60, nullptr);
    previousTree.insert(80, nullptr);
    previousTree.insert(35, nullptr);
    previousTree.insert(40, nullptr);
    previousTree.insert(45, nullptr);

    cout << "\nBorrow from previous - initial tree:" << endl;
    previousTree.print();

    previousTree.remove(40);
    previousTree.remove(45);
    previousTree.remove(60);
    previousTree.remove(70);

    cout << "\nBorrow from previous - before deleting 35:" << endl;
    previousTree.print();

    previousTree.remove(35);

    if(previousTree.contains(35))
    {
        throw runtime_error(
            "BTree test failed: 35 should have been removed"
        );
    }

    cout << "\nBorrow from previous - after deleting 35:" << endl;
    previousTree.print();

    cout << "BTree tests passed!" << endl;

    BTree rootTree;

    rootTree.insert(10, nullptr);
    rootTree.insert(20, nullptr);
    rootTree.insert(30, nullptr);
    rootTree.insert(40, nullptr);

    cout << "\nRoot shrink - initial tree:" << endl;
    rootTree.print();

    rootTree.remove(10);
    rootTree.remove(20);

    cout << "\nRoot shrink - after deleting 10 and 20:" << endl;
    rootTree.print();

    if(rootTree.contains(10) || rootTree.contains(20))
    {
        throw runtime_error(
            "BTree test failed: deleted root-shrink keys still exist"
        );
    }

    cout << "\nRoot shrink - final tree:" << endl;
    rootTree.print();

    BTree internalTree;

    internalTree.insert(50, nullptr);
    internalTree.insert(30, nullptr);
    internalTree.insert(70, nullptr);
    internalTree.insert(10, nullptr);
    internalTree.insert(20, nullptr);
    internalTree.insert(60, nullptr);
    internalTree.insert(80, nullptr);
    internalTree.insert(35, nullptr);
    internalTree.insert(40, nullptr);
    internalTree.insert(45, nullptr);

    cout << "\nInternal deletion - initial tree:" << endl;
    internalTree.print();

    internalTree.remove(30);

    if(internalTree.contains(30))
    {
        throw runtime_error(
            "BTree test failed: internal key 30 should have been removed"
        );
    }

    cout << "\nInternal deletion - after deleting 30:" << endl;
    internalTree.print();

    //12-10-2026: Test:
        // Range query tests
    BTree rangeTree;

    rangeTree.insert(10, nullptr);
    rangeTree.insert(20, nullptr);
    rangeTree.insert(30, nullptr);
    rangeTree.insert(40, nullptr);
    rangeTree.insert(50, nullptr);

    // Gedeeltelijk bereik: 20 en 30.
    if(rangeTree.findRange(15, 35).size() != 2)
    {
        throw runtime_error(
            "BTree range test failed: expected 2 results for range 15-35"
        );
    }

    // Inclusieve grenzen: 20, 30 en 40.
    if(rangeTree.findRange(20, 40).size() != 3)
    {
        throw runtime_error(
            "BTree range test failed: expected 3 results for range 20-40"
        );
    }

    // Exact één ID.
    if(rangeTree.findRange(30, 30).size() != 1)
    {
        throw runtime_error(
            "BTree range test failed: expected 1 result for range 30-30"
        );
    }

    // Geen resultaten.
    if(!rangeTree.findRange(60, 70).empty())
    {
        throw runtime_error(
            "BTree range test failed: expected no results for range 60-70"
        );
    }

    // Ongeldig bereik: minimum groter dan maximum.
    if(!rangeTree.findRange(40, 20).empty())
    {
        throw runtime_error(
            "BTree range test failed: invalid range should return no results"
        );
    }

    cout << "BTree range tests passed!" << endl;

        // Range query test met echte KnowledgeItems
    Vector vectorA({1.0, 2.0});
    Vector vectorB({3.0, 4.0});
    Vector vectorC({5.0, 6.0});

    KnowledgeItem item20(20, vectorA, "Record twintig");
    KnowledgeItem item30(30, vectorB, "Record dertig");
    KnowledgeItem item40(40, vectorC, "Record veertig");

    BTree recordTree;

    recordTree.insert(item40.id, &item40);
    recordTree.insert(item20.id, &item20);
    recordTree.insert(item30.id, &item30);

    const vector<const KnowledgeItem*> results =
        recordTree.findRange(20, 40);

    if(results.size() != 3)
    {
        throw runtime_error(
            "BTree range test failed: expected 3 KnowledgeItems"
        );
    }

    if(results[0]->id != 20 ||
       results[1]->id != 30 ||
       results[2]->id != 40)
    {
        throw runtime_error(
            "BTree range test failed: incorrect IDs or order"
        );
    }

    if(results[0]->text != "Record twintig" ||
       results[1]->text != "Record dertig" ||
       results[2]->text != "Record veertig")
    {
        throw runtime_error(
            "BTree range test failed: incorrect KnowledgeItems returned"
        );
    }

    cout << "BTree KnowledgeItem range tests passed!" << endl;

    //13-10-2026: Nog een test:
        // Extra controle: rangequery met meerdere B-tree-splitsingen.
    BTree splitRangeTree;

    Vector testVector({1.0, 2.0});

    std::vector<KnowledgeItem> testItems;
    testItems.reserve(10);

    // Bewust in een ongesorteerde volgorde invoegen.
    const int testIds[] = {50, 30, 70, 10, 40, 60, 80, 20, 35, 45};

    for (int id : testIds)
    {
        testItems.emplace_back(id, testVector, "Record " + std::to_string(id));
        splitRangeTree.insert(id, &testItems.back());
    }

    const auto splitResults = splitRangeTree.findRange(20, 70);

    const int expectedIds[] = {20, 30, 35, 40, 45, 50, 60, 70};

    if (splitResults.size() != 8)
    {
        throw runtime_error(
            "BTree split range test failed: expected 8 results"
        );
    }

    for (std::size_t i = 0; i < splitResults.size(); ++i)
    {
        if (splitResults[i] == nullptr ||
            splitResults[i]->id != expectedIds[i] ||
            splitResults[i]->text != "Record " + std::to_string(expectedIds[i]))
        {
            throw runtime_error(
                "BTree split range test failed: incorrect item or order"
            );
        }
    }

    cout << "BTree split range test passed!" << endl;

}
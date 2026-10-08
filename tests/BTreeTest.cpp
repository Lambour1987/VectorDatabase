//6-10-2026

//6-10-2026

#include "../include/BTree.h"

#include <iostream>
#include <stdexcept>

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
}
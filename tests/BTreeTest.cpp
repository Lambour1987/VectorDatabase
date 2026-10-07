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
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(10);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);
    tree.insert(20);
    tree.insert(35);
    tree.insert(45);

    //Print boom
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

    mergeTree.insert(50);
    mergeTree.insert(30);
    mergeTree.insert(70);
    mergeTree.insert(10);
    mergeTree.insert(20);
    mergeTree.insert(60);
    mergeTree.insert(80);
    mergeTree.insert(35);
    mergeTree.insert(40);
    mergeTree.insert(45);

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

    nextTree.insert(50);
    nextTree.insert(30);
    nextTree.insert(70);
    nextTree.insert(10);
    nextTree.insert(20);
    nextTree.insert(60);
    nextTree.insert(80);
    nextTree.insert(35);
    nextTree.insert(40);
    nextTree.insert(45);

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

    previousTree.insert(50);
    previousTree.insert(30);
    previousTree.insert(70);
    previousTree.insert(10);
    previousTree.insert(20);
    previousTree.insert(60);
    previousTree.insert(80);
    previousTree.insert(35);
    previousTree.insert(40);
    previousTree.insert(45);

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

    rootTree.insert(10);
    rootTree.insert(20);
    rootTree.insert(30);
    rootTree.insert(40);

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

    internalTree.insert(50);
    internalTree.insert(30);
    internalTree.insert(70);
    internalTree.insert(10);
    internalTree.insert(20);
    internalTree.insert(60);
    internalTree.insert(80);
    internalTree.insert(35);
    internalTree.insert(40);
    internalTree.insert(45);

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

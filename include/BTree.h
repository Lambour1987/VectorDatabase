//6-10-2026: B Tree

#pragma once

#include <vector>
#include <cstddef>

//8-10-2026: class KnowledgeItem, niet als #include "Knowledge item" omdat we alleen een pointer nodig hebben
class KnowledgeItem;

class BTree
{
private:
    //8-10-2026: Nieuwe struct binnen de class
    struct Entry
    {
        int key;
        KnowledgeItem* item;
    };

    //Struct binnen class
    struct Node
    {
        // Een node kan meerdere keys bevatten
        // 8-10-2026: Gewijzigd van std::vector<int> keys;
        std::vector<Entry> entries;
        // Meerdere kinderen
        std::vector<Node*> children;
        bool leaf;

        Node(bool isLeaf);
    };

    Node* root;

    void splitChild(Node* parent, std::size_t childIndex);
    //8-10-26: gewijzigd van void insertNonFull(Node* node, int key); naar
    //void BTree::insertNonFull(Node* node, int key, KnowledgeItem* item);

public:

    BTree();
    ~BTree();

    //8-10-26: Gewijzigd van void insert(int key); naar:

    void insert(int key, KnowledgeItem* item);
    bool contains(int key) const;
    //8-10-2026: Toegevoegd:
    const KnowledgeItem* find(int key) const;
    void print() const;
    //7-10-2026: Fucntie om keyNodes te verwijderen
    void remove(int key);
    

private:
    bool search(Node* node, int key) const;
    void destroy(Node* node);
    void printNode(Node* node, int depth) const;
    //7-10-26: Dus een functie om de LeafNode te verwijderen is een andere dan de keyNode
    void removeFromLeaf(Node* node, std::size_t keyIndex);
    // Lenen van volgende
    void borrowFromNext(Node* parent, std::size_t childIndex);
    // Lenen van vorige
    void borrowFromPrevious(Node* parent, std::size_t childIndex);
    // Alternatief: Samenvoegen
    void mergeChildren(Node* parent, std::size_t childIndex);

    //8-10-26: int getPredecessor(Node* node) const; en int getSuccessor(Node* node) const; naar
    Entry getPredecessor(Node* node) const;
    Entry getSuccessor(Node* node) const;
    void removeFromInternal(Node* node, std::size_t keyIndex);
    void removeFromNode(Node* node, int key);

    //8-10-2026: Nieuwe functie
    void insertNonFull(Node* node, int key, KnowledgeItem* item);

};

//6-10-2026: B Tree

#pragma once

#include <vector>
#include <cstddef>

class BTree
{
private:

    struct Node
    {
        // Een node kan meerdere keys bevatten
        std::vector<int> keys;
        // Meerdere kinderen
        std::vector<Node*> children;
        bool leaf;

        // 
        Node(bool isLeaf);
    };

    Node* root;

    void splitChild(Node* parent, std::size_t childIndex);
    void insertNonFull(Node* node, int key);

public:

    BTree();
    ~BTree();

    void insert(int key);
    bool contains(int key) const;
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

    int getPredecessor(Node* node) const;
    int getSuccessor(Node* node) const;
    void removeFromInternal(Node* node, std::size_t keyIndex);
    void removeFromNode(Node* node, int key);

};

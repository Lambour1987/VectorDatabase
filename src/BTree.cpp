//6-10-2026: Nog een keer goed leren

#include "../include/BTree.h"
#include "../include/BTreeTest.h"
#include <iostream>

using namespace std;

BTree::Node::Node(bool isLeaf)
    : leaf(isLeaf)
{
}

BTree::BTree()
    : root(new Node(true))
{
}

//Nodes staan dynamisch in het geheugen
BTree::~BTree()
{
    destroy(root);
}

void BTree::destroy(Node* node)
{
    if(node == nullptr)
    {
        return;
    }

    for(Node* child : node->children)
    {
        destroy(child);
    }

    delete node;
}

//Splitsen kinderen
void BTree::splitChild(Node* parent, size_t childIndex)
{
    Node* fullChild = parent->children[childIndex];

    Node* rightChild = new Node(fullChild->leaf);

    // De middelste key gaat naar de parent.
    int middleKey = fullChild->keys[1];

    // De rechter key gaat naar de nieuwe rechter node.
    rightChild->keys.push_back(fullChild->keys[2]);

    // Als de node kinderen heeft, verplaats de rechter helft
    // van de kinderen naar de nieuwe rechter node.
    if(!fullChild->leaf)
    {
        rightChild->children.push_back(fullChild->children[2]);
        rightChild->children.push_back(fullChild->children[3]);

        fullChild->children.resize(2);
    }

    // De linker node houdt alleen de linker key.
    fullChild->keys.resize(1);

    // Maak ruimte voor het nieuwe child.
    parent->children.insert(
        parent->children.begin() + childIndex + 1,
        rightChild
    );

    // Voeg de middelste key toe aan de parent.
    parent->keys.insert(
        parent->keys.begin() + childIndex,
        middleKey
    );
}


void BTree::insertNonFull(Node* node, int key)
{
    int index = static_cast<int>(node->keys.size()) - 1;

    // Als dit een leaf is, voegen we de key
    // direct op de juiste positie toe.
    if(node->leaf)
    {
        node->keys.push_back(0);

        while(index >= 0 && key < node->keys[index])
        {
            node->keys[index + 1] = node->keys[index];
            index--;
        }

        node->keys[index + 1] = key;
        return;
    }

    // Zoek het child waarin de key thuishoort.
    while(index >= 0 && key < node->keys[index])
    {
        index--;
    }

    index++;

    // Als het child vol is, moeten we het eerst splitsen.
    if(node->children[index]->keys.size() == 3)
    {
        splitChild(node, index);

        // Na de split staat er een nieuwe key in node.
        // Bepaal opnieuw of we links of rechts moeten gaan.
        if(key > node->keys[index])
        {
            index++;
        }
    }

    insertNonFull(node->children[index], key);
}


void BTree::insert(int key)
{
    // Als de root vol is, moet de boom eerst groeien.
    if(root->keys.size() == 3)
    {
        Node* newRoot = new Node(false);

        // De oude root wordt een child van de nieuwe root.
        newRoot->children.push_back(root);

        // Splits de oude root.
        splitChild(newRoot, 0);

        // De nieuwe root wordt nu de daadwerkelijke root.
        root = newRoot;
    }

    // De root is nu gegarandeerd niet vol.
    insertNonFull(root, key);
}

bool BTree::search(Node* node, int key) const
{
    std::size_t index = 0;

    // Zoek de eerste key die groter of gelijk is aan key.
    while(index < node->keys.size() && key > node->keys[index])
    {
        index++;
    }

    // Hebben we de key gevonden?
    if(index < node->keys.size() && key == node->keys[index])
    {
        return true;
    }

    // Als dit een leaf is, kunnen we niet verder zoeken.
    if(node->leaf)
    {
        return false;
    }

    // Zoek verder in het juiste child.
    return search(node->children[index], key);
}

bool BTree::contains(int key) const
{
    return search(root, key);
}



void BTree::printNode(Node* node, int depth) const
{
    if(node == nullptr)
    {
        return;
    }

    for(int i = 0; i < depth; i++)
    {
        cout << "    ";
    }

    cout << "[";

    for(std::size_t i = 0; i < node->keys.size(); i++)
    {
        cout << node->keys[i];

        if(i + 1 < node->keys.size())
        {
            cout << " | ";
        }
    }

    cout << "]" << endl;

    for(Node* child : node->children)
    {
        printNode(child, depth + 1);
    }
}

void BTree::print() const
{
    printNode(root, 0);
}

void BTree::removeFromLeaf(Node* node, std::size_t keyIndex)
{
    node->keys.erase(node->keys.begin() + keyIndex);
}

//7-10-2026: Kan waarschijnlijk weg
// void BTree::remove(int key)
// {
//     Node* node = root;

//     while(!node->leaf)
//     {
//         std::size_t index = 0;

//         while(index < node->keys.size() && key > node->keys[index])
//         {
//             index++;
//         }

//         if(index < node->keys.size() && key == node->keys[index])
//         {
//             cout << "Key " << key << " zit in een interne node." << endl;
//             return;
//         }

//         Node* child = node->children[index];

//         // Het child heeft maar één key.
//         // Eerst zorgen we dat het veilig een key kan verliezen.
//         if(child->keys.size() == 1)
//         {
//             //7-10-2026: Tijdelijke test:
//             cout << "Child heeft 1 key: " << child->keys[0] << endl;

//             if(index + 1 < node->children.size() &&
//             node->children[index + 1]->keys.size() > 1)
//             {
//                 borrowFromNext(node, index);
//             }
//             else if(index > 0 &&
//                     node->children[index - 1]->keys.size() > 1)
//             {
//                 borrowFromPrevious(node, index);
//             }
//             else if(index + 1 < node->children.size())
//             {
//                 mergeChildren(node, index);
//             }
//             else
//             {
//                 mergeChildren(node, index - 1);
//                 index--;
//             }
//         }

//         node = node->children[index];
//     }

//     std::size_t index = 0;

//     while(index < node->keys.size() && key > node->keys[index])
//     {
//         index++;
//     }

//     if(index < node->keys.size() && node->keys[index] == key)
//     {
//         removeFromLeaf(node, index);
//         return;
//     }

//     cout << "Key " << key << " niet gevonden." << endl;
// }

void BTree::remove(int key)
{
    removeFromNode(root, key);

    // Als de root geen keys meer heeft,
    // maar wel een child, wordt dat child de nieuwe root.
    if(!root->leaf && root->keys.empty())
    {
        Node* oldRoot = root;
        root = root->children[0];

        delete oldRoot;
    }
}

//7-10-2026: Lenen
void BTree::borrowFromNext(Node* parent, size_t childIndex)
{
    Node* child = parent->children[childIndex];
    Node* sibling = parent->children[childIndex + 1];

    // De separator uit de parent gaat naar het child.
    child->keys.push_back(parent->keys[childIndex]);

    // De kleinste key van de sibling gaat naar de parent.
    parent->keys[childIndex] = sibling->keys.front();

    // Die key is nu uit de sibling gehaald.
    sibling->keys.erase(sibling->keys.begin());
}

void BTree::borrowFromPrevious(Node* parent, size_t childIndex)
{
    Node* child = parent->children[childIndex];
    Node* sibling = parent->children[childIndex - 1];

    // De separator uit de parent gaat naar het child.
    child->keys.insert(
        child->keys.begin(),
        parent->keys[childIndex - 1]
    );

    // De grootste key van de sibling gaat naar de parent.
    parent->keys[childIndex - 1] = sibling->keys.back();

    // Die key is nu uit de sibling gehaald.
    sibling->keys.pop_back();
}

void BTree::mergeChildren(Node* parent, size_t childIndex)
{
    Node* leftChild = parent->children[childIndex];
    Node* rightChild = parent->children[childIndex + 1];

    // De separator uit de parent gaat naar het linker child.
    leftChild->keys.push_back(parent->keys[childIndex]);

    // Voeg alle keys van het rechter child toe.
    for(int key : rightChild->keys)
    {
        leftChild->keys.push_back(key);
    }

    // Als de children interne nodes zijn,
    // moeten ook hun child pointers worden samengevoegd.
    if(!rightChild->leaf)
    {
        for(Node* child : rightChild->children)
        {
            leftChild->children.push_back(child);
        }
    }

    // Verwijder de separator uit de parent.
    parent->keys.erase(parent->keys.begin() + childIndex);

    // Het rechter child bestaat niet meer als aparte node.
    parent->children.erase(parent->children.begin() + childIndex + 1);

    delete rightChild;
}

int BTree::getPredecessor(Node* node) const
{
    while(!node->leaf)
    {
        node = node->children.back();
    }

    return node->keys.back();
}

int BTree::getSuccessor(Node* node) const
{
    while(!node->leaf)
    {
        node = node->children.front();
    }

    return node->keys.front();
}

void BTree::removeFromInternal(Node* node, size_t keyIndex)
{
    int key = node->keys[keyIndex];

    Node* leftChild = node->children[keyIndex];
    Node* rightChild = node->children[keyIndex + 1];

    // Het linker child heeft genoeg keys.
    if(leftChild->keys.size() > 1)
    {
        int predecessorKey = getPredecessor(leftChild);

        node->keys[keyIndex] = predecessorKey;

        removeFromNode(leftChild, predecessorKey);
        return;
    }

    // Het rechter child heeft genoeg keys.
    if(rightChild->keys.size() > 1)
    {
        int successorKey = getSuccessor(rightChild);

        node->keys[keyIndex] = successorKey;

        removeFromNode(rightChild, successorKey);
        return;
    }

    // Beide children hebben maar één key.
    mergeChildren(node, keyIndex);

    // De oorspronkelijke key zit nu in het samengevoegde child.
    removeFromNode(node->children[keyIndex], key);
}

void BTree::removeFromNode(Node* node, int key)
{
    size_t index = 0;

    // Zoek de eerste key die groter of gelijk is aan key.
    while(index < node->keys.size() && key > node->keys[index])
    {
        index++;
    }

    // De key staat in deze node.
    if(index < node->keys.size() && node->keys[index] == key)
    {
        // Als dit een leaf is, kunnen we de key direct verwijderen.
        if(node->leaf)
        {
            removeFromLeaf(node, index);
            return;
        }

        // De key zit in een interne node.
        removeFromInternal(node, index);
        return;
    }

    // Als dit een leaf is, bestaat de key niet.
    if(node->leaf)
    {
        return;
    }

    // De key moet in child[index] zitten.
    Node* child = node->children[index];

    // Een child met maar één key mag niet zomaar
    // een key verliezen. Maak hem daarom eerst sterker.
    if(child->keys.size() == 1)
    {
        if(index + 1 < node->children.size() &&
           node->children[index + 1]->keys.size() > 1)
        {
            borrowFromNext(node, index);
        }
        else if(index > 0 &&
                node->children[index - 1]->keys.size() > 1)
        {
            borrowFromPrevious(node, index);
        }
        else if(index + 1 < node->children.size())
        {
            mergeChildren(node, index);
        }
        else
        {
            mergeChildren(node, index - 1);
            index--;
        }
    }

    // Ga verder in het child waar de key nu zit.
    removeFromNode(node->children[index], key);
}
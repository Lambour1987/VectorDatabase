//6-10-2026: Nog een keer goed leren

#include "../include/BTree.h"
#include "../include/BTreeTest.h"
#include "../include/KnowledgeItem.h"
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
    //8-10-2026: gewijzigd van int middleKey = fullChild->entries[1];
    Entry middleEntry = fullChild->entries[1];

    // De rechter key gaat naar de nieuwe rechter node.
    rightChild->entries.push_back(fullChild->entries[2]);

    // Als de node kinderen heeft, verplaats de rechter helft
    // van de kinderen naar de nieuwe rechter node.
    if(!fullChild->leaf)
    {
        rightChild->children.push_back(fullChild->children[2]);
        rightChild->children.push_back(fullChild->children[3]);

        fullChild->children.resize(2);
    }

    // De linker node houdt alleen de linker key.
    // 8-10-26: Gewijzigd van fullChild->keys.resize(1);
    fullChild->entries.resize(1);

    // Maak ruimte voor het nieuwe child.
    parent->children.insert(
        parent->children.begin() + childIndex + 1,
        rightChild
    );

    // Voeg de middelste key toe aan de parent.
    //8-10-26: parent->entries.insert(parent->entries.begin() + childIndex, middleKey);
    parent->entries.insert(parent->entries.begin() + childIndex,middleEntry);
}


//8-10-2026: Gewijzigd van void BTree::insertNonFull(Node* node, int key)
void BTree::insertNonFull(Node* node, int key, KnowledgeItem* item)
{
    //8-10-26: van int index = static_cast<int>(node->keys.size()) - 1;
    int index = static_cast<int>(node->entries.size()) - 1;

    // Als dit een leaf is, voegen we de key
    // direct op de juiste positie toe.
    if(node->leaf)
    {
        //8-10-26: node->keys.push_back(0);
        node->entries.push_back({0,nullptr});

        //8-10-26 van while(index >= 0 && key < node->keys[index])
        while(index >= 0 && key < node->entries[index].key)
        {
            // 8-10-26: van node->keys[index + 1] = node->keys[index];
            node->entries[index + 1] = node->entries[index];
            index--;
        }

        //8-10-26: node->keys[index + 1] = key;
        node->entries[index + 1] = {key,item};
        return;
    }

    // Zoek het child waarin de key thuishoort.
    while(index >= 0 && key < node->entries[index].key)
    {
        index--;
    }

    index++;

    // Als het child vol is, moeten we het eerst splitsen.
    if(node->children[index]->entries.size() == 3)
    {
        splitChild(node, index);

        // Na de split staat er een nieuwe key in node.
        // Bepaal opnieuw of we links of rechts moeten gaan.
        //8-10-26: if(key > node->keys[index])
        if(key > node->entries[index].key)
        {
            index++;
        }
    }

    insertNonFull(node->children[index], key, item);
}


//8-10-26: van void BTree::insert(int key) naar
void BTree::insert(int key, KnowledgeItem* item)
{
    // Als de root vol is, moet de boom eerst groeien.
    if(root->entries.size() == 3)
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
    //8-10-26: insertNonFull(root, key);
    insertNonFull(root, key, item);

}

bool BTree::search(Node* node, int key) const
{
    std::size_t index = 0;

    // Zoek de eerste key die groter of gelijk is aan key.
    //8-10-26: while(index < node->keys.size() && key > node->keys[index])
    while(index < node->entries.size() && key > node->entries[index].key)
    {
        index++;
    }

    // Hebben we de key gevonden?
    if(index < node->entries.size() && key == node->entries[index].key)
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

    for(std::size_t i = 0; i < node->entries.size(); i++)
    {
        //8-10-26: van cout << node->keys[i];
        cout << node->entries[i].key;

        if(i + 1 < node->entries.size())
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
    //8-10-26: gewijzigd van node->keys.erase(node->keys.begin() + keyIndex);
    node->entries.erase(node->entries.begin() + keyIndex);
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
    //8-10-26: if(!root->leaf && root->keys.empty())
    if(!root->leaf && root->entries.empty())
    {
        Node* oldRoot = root;
        root = root->children[0];

        delete oldRoot;
    }
}

//7-10-2026: Lenen
//8-10-26
void BTree::borrowFromNext(Node* parent, size_t childIndex)
{
    Node* child = parent->children[childIndex];
    Node* sibling = parent->children[childIndex + 1];

    // De separator uit de parent gaat naar het child.
    //8-10-26: child->keys.push_back(parent->keys[childIndex]);
    child->entries.push_back(parent->entries[childIndex]);

    // De kleinste key van de sibling gaat naar de parent.
    parent->entries[childIndex] = sibling->entries.front();

    // Die key is nu uit de sibling gehaald.
    sibling->entries.erase(sibling->entries.begin());
}

void BTree::borrowFromPrevious(Node* parent, size_t childIndex)
{
    Node* child = parent->children[childIndex];
    Node* sibling = parent->children[childIndex - 1];

    // De separator uit de parent gaat naar het child.
    // 8-10-26 gewijzigd de keys
    child->entries.insert(
        child->entries.begin(),
        parent->entries[childIndex - 1]
    );

    // De grootste key van de sibling gaat naar de parent.
    parent->entries[childIndex - 1] = sibling->entries.back();

    // Die key is nu uit de sibling gehaald.
    sibling->entries.pop_back();
}

void BTree::mergeChildren(Node* parent, size_t childIndex)
{
    Node* leftChild = parent->children[childIndex];
    Node* rightChild = parent->children[childIndex + 1];

    // De separator uit de parent gaat naar het linker child.
    leftChild->entries.push_back(parent->entries[childIndex]);

    // Voeg alle keys van het rechter child toe.
    //8-10-26: Vervang van for(int key : rightChild->keys)
    for(const Entry& entry : rightChild->entries)
    {
        leftChild->entries.push_back(entry);
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
    parent->entries.erase(parent->entries.begin() + childIndex);

    // Het rechter child bestaat niet meer als aparte node.
    parent->children.erase(parent->children.begin() + childIndex + 1);

    delete rightChild;
}

BTree::Entry BTree::getPredecessor(Node* node) const
{
    while(!node->leaf)
    {
        node = node->children.back();
    }

    //8-10-26: Van return node->keys.back();
    return node->entries.back();
}

BTree::Entry BTree::getSuccessor(Node* node) const
{
    while(!node->leaf)
    {
        node = node->children.front();
    }

    return node->entries.front();
}

void BTree::removeFromInternal(Node* node, size_t keyIndex)
{
    //8-10-26: Gewijzigd van int key = node->keys[keyIndex];
    Entry entry = node->entries[keyIndex];

    Node* leftChild = node->children[keyIndex];
    Node* rightChild = node->children[keyIndex + 1];

    // Het linker child heeft genoeg keys.
    if(leftChild->entries.size() > 1)
    {
        Entry predecessorKey = getPredecessor(leftChild);

        node->entries[keyIndex] = predecessorKey;

        removeFromNode(leftChild, predecessorKey.key);
        return;
    }

    // Het rechter child heeft genoeg keys.
    if(rightChild->entries.size() > 1)
    {
        Entry successorKey = getSuccessor(rightChild);

        node->entries[keyIndex] = successorKey;

        removeFromNode(rightChild, successorKey.key);
        return;
    }

    // Beide children hebben maar één key.
    mergeChildren(node, keyIndex);

    // De oorspronkelijke key zit nu in het samengevoegde child.
    removeFromNode(node->children[keyIndex], entry.key);
}

void BTree::removeFromNode(Node* node, int key)
{
    size_t index = 0;

    // Zoek de eerste key die groter of gelijk is aan key.
    while(index < node->entries.size() && key > node->entries[index].key)
    {
        index++;
    }

    // De key staat in deze node.
    if(index < node->entries.size() && node->entries[index].key == key)
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
    if(child->entries.size() == 1)
    {
        if(index + 1 < node->children.size() &&
           node->children[index + 1]->entries.size() > 1)
        {
            borrowFromNext(node, index);
        }
        else if(index > 0 &&
                node->children[index - 1]->entries.size() > 1)
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

//8-10-2026: nieuwe functie: Zoekt ID en kijkt in de huidige B-Tree-Node.
// Als die gevonden is geef dan het KnowledgeItem* terug. Als deze niet
// gevonden is: Bekijk of het een leaf is. Zo ja dan bestaat die niet. Zo nie
const KnowledgeItem* BTree::find(int key) const
{
    Node* node = root;

    while(node != nullptr)
    {
        size_t index = 0;

        while(index < node->entries.size() &&
              key > node->entries[index].key)
        {
            index++;
        }

        if(index < node->entries.size() &&
           key == node->entries[index].key)
        {
            return node->entries[index].item;
        }

        if(node->leaf)
        {
            return nullptr;
        }

        node = node->children[index];
    }

    return nullptr;
}
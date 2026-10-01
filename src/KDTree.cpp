//25-8-2026

#include "../include/KDTree.h"
#include "../include/MaxHeap.h"

#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;


// Constructor
// De KDTree krijgt nu een vector met KnowledgeItems.
// Ieder KnowledgeItem bevat zelf een Vector in .vector.
KDTree::KDTree(const vector<KnowledgeItem>& items)
    : root(nullptr)
{
    // Hier bewaren we pointers naar de KnowledgeItems.
    vector<const KnowledgeItem*> points;

    for(const KnowledgeItem& item : items)
    {
        points.push_back(&item);
    }

    // Start de recursieve opbouw bij dimensie 0.
    root = buildKDTree(points, 0);
}


// 28-8-2026: KDTree basecase
KDNode* KDTree::buildKDTree(
    vector<const KnowledgeItem*> points,
    size_t dimension)
{
    // Base case:
    // Als er geen punten meer zijn, hoeft er geen node gemaakt te worden.
    if(points.empty())
    {
        return nullptr;
    }

    // Sorteer de KnowledgeItems op de huidige dimensie.
    //
    // Belangrijk:
    // a en b zijn pointers naar KnowledgeItems.
    // De Vector die we willen vergelijken zit in:
    //
    // a->vector
    //
    // Daarom gebruiken we:
    //
    // a->vector.at(dimension)
    sort(
        points.begin(),
        points.end(),
        [dimension](const KnowledgeItem* a, const KnowledgeItem* b)
        {
            return a->vector.at(dimension)
                 < b->vector.at(dimension);
        }
    );

    // Pak het middelste element.
    size_t middle = points.size() / 2;

    // De KDNode bewaart nu een pointer naar het hele KnowledgeItem.
    KDNode* node = new KDNode{
        points[middle],
        nullptr,
        nullptr,
        dimension
    };

    // Alles links van middle gaat naar de linker subtree.
    vector<const KnowledgeItem*> leftPoints(
        points.begin(),
        points.begin() + middle
    );

    // Alles rechts van middle gaat naar de rechter subtree.
    vector<const KnowledgeItem*> rightPoints(
        points.begin() + middle + 1,
        points.end()
    );

    // Ga naar de volgende dimensie.
    //1-10-2026: aangepast size_t nextDimension = (dimension + 1) % Vector::MAX_DIMENSIONS;
    size_t nextDimension = (dimension + 1) % points[0]->vector.dimension();

    // Recursief linker- en rechterdeel bouwen.
    node->left = buildKDTree(leftPoints, nextDimension);
    node->right = buildKDTree(rightPoints, nextDimension);

    return node;
}


// Print de hele boom.
void KDTree::printTree() const
{
    printTree(root, 0);
}


// Recursieve print helper.
void KDTree::printTree(
    const KDNode* node,
    int depth) const
{
    if(node == nullptr)
    {
        return;
    }

    for(int i = 0; i < depth; ++i)
    {
        cout << "   ";
    }

    // node->item is een KnowledgeItem*.
    //
    // De Vector zit vervolgens in:
    //
    // node->item->vector
    //
    cout << "["
         << node->item->vector.at(0) << ", "
         << node->item->vector.at(1) << ", "
         << node->item->vector.at(2) << "]"
         << " [dim "
         << node->dimension
         << "]\n";

    printTree(node->left, depth + 1);
    printTree(node->right, depth + 1);
}


// Destructor.
KDTree::~KDTree()
{
    destroyTree(root);
}


// Recursief alle KDNodes verwijderen.
void KDTree::destroyTree(KDNode* node)
{
    if(node == nullptr)
    {
        return;
    }

    destroyTree(node->left);
    destroyTree(node->right);

    delete node;
}


// 15-9-2026: nearestNeighbor
//
// query is nog steeds gewoon een Vector.
//
// De KDTree zelf bevat KnowledgeItems.
KDNode* KDTree::nearestNeighbor(
    KDNode* node,
    const Vector& query,
    KDNode* best)
{
    KDNode* first;
    KDNode* second;

    if(node == nullptr)
    {
        return best;
    }

    // Kijk naar de Vector die in het KnowledgeItem zit.
    if(
        query.at(node->dimension)
        <
        node->item->vector.at(node->dimension)
    )
    {
        first = node->left;
        second = node->right;
    }
    else
    {
        first = node->right;
        second = node->left;
    }

    // Eerst zoeken in de kant waar de query volgens de
    // huidige dimensie waarschijnlijk thuishoort.
    best = nearestNeighbor(first, query, best);

    // Bereken afstand tussen query en het huidige KnowledgeItem.
    double distance =
        query.distanceTo(node->item->vector);

    // Als dit punt dichterbij is dan onze huidige beste,
    // wordt dit de nieuwe beste node.
    if(
        best == nullptr
        ||
        distance < query.distanceTo(best->item->vector)
    )
    {
        best = node;
    }

    // Afstand van de query tot het scheidingsvlak.
    double planeDistance =
        std::abs(
            query.at(node->dimension)
            -
            node->item->vector.at(node->dimension)
        );

    // Alleen de andere kant bekijken als die mogelijk
    // een dichter punt kan bevatten.
    if(
        best == nullptr
        ||
        planeDistance < query.distanceTo(best->item->vector)
    )
    {
        best = nearestNeighbor(second, query, best);
    }

    return best;
}


// 15-9-2026: kNearestNeighbors helper
void KDTree::kNearestNeighbors(
    KDNode* node,
    const Vector& query,
    size_t k,
    MaxHeap& heap) const
{
    KDNode* first;
    KDNode* second;

    if(node == nullptr)
    {
        return;
    }

    // Bepaal eerst welke kant we moeten bezoeken.
    if(
        query.at(node->dimension)
        <
        node->item->vector.at(node->dimension)
    )
    {
        first = node->left;
        second = node->right;
    }
    else
    {
        first = node->right;
        second = node->left;
    }

    // Eerst de waarschijnlijk interessante kant bezoeken.
    kNearestNeighbors(first, query, k, heap);

    // Afstand tussen query en huidig KnowledgeItem.
    double distance =
        query.distanceTo(node->item->vector);

    // Als de heap nog geen k elementen heeft,
    // voegen we dit item sowieso toe.
    if(heap.size() < k)
    {
        heap.push({
            distance,
            node->item
        });
    }
    else
    {
        // De MaxHeap bevat op top() het slechtste
        // resultaat van onze huidige top-k.
        auto top = heap.top();

        double worstDistance = top->first;

        // Als het huidige item beter is dan het slechtste
        // item uit de heap, vervangen we dat item.
        if(distance < worstDistance)
        {
            heap.pop();

            heap.push({
                distance,
                node->item
            });
        }
    }

    // Afstand tot het scheidingsvlak.
    double planeDistance =
        std::abs(
            query.at(node->dimension)
            -
            node->item->vector.at(node->dimension)
        );

    // Als de heap nog niet vol zit, moeten we sowieso
    // verder zoeken.
    //
    // Of:
    // als het andere gebied mogelijk nog een beter punt bevat,
    // zoeken we daar ook.
    if(
        heap.size() < k
        ||
        planeDistance < heap.top()->first
    )
    {
        kNearestNeighbors(second, query, k, heap);
    }
}


// Publieke functie voor k dichtstbijzijnde KnowledgeItems.
vector<const KnowledgeItem*> KDTree::kNearestNeighbors(
    const Vector& query,
    size_t k) const
{
    MaxHeap heap;

    // Gebruik de recursieve helper.
    kNearestNeighbors(root, query, k, heap);

    // De resultaten uit de heap halen.
    vector<const KnowledgeItem*> neighbors;

    while(!heap.empty())
    {
        auto top = heap.top();

        neighbors.push_back(top->second);

        heap.pop();
    }

    // Heap levert van slecht naar goed.
    // Daarom draaien we de resultaten om.
    reverse(
        neighbors.begin(),
        neighbors.end()
    );

    return neighbors;
}

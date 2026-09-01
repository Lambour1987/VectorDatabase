//25-8-2026

#include "../include/KDTree.h"
#include <algorithm>
#include <iostream>
//inlcude math voor de abs functie
#include <cmath>

using namespace std;

//Constructor
KDTree::KDTree(const vector<Vector>&vectors):root(nullptr)
{
    // Constructor loopt door de vector die als parameter heen die aan de constructor is meegegeven
    //Maak een vector van pointers naar de bestaande Vector objecten
    vector<const Vector*> points;

    //Loop door alle vectoren van vectors heen en sla hun adressen op
    for(const Vector& vector: vectors)
    {
        points.push_back(&vector);
    }
    root = buildKDTree(points, 0);
}

//28-8-2026: KDTree basecase
// BELANGRIJK: als ik deze in draw.io uit ga tekenen moet het duidelijk worden dat we DFS de punten (A*,B*,C*) vinden
// maar dat we pas bij het afbouwen van de callstack echt de verbindingen gaan maken
// Functie buildKDTree uit de class KDTree die een pointer naar een KNode teruggeeft en als input gebruikt een vector van 
// pointers naar Vectoren genaamd points en een size_t dimension.
KDNode*KDTree::buildKDTree(vector<const Vector*> points, size_t dimension)
{
    //Als er geen points meer zijn dan retourneer nullptr
    if(points.empty())
    {
        return nullptr;
    }

    // Sorteer de pointers in points op basis van de waarden waarnaar ze wijzen (op dimension).
    // a en b zijn de twee pointers die std::sort op dat moment met elkaar vergelijkt.
    sort(points.begin(), points.end(),[dimension](const Vector* a, const Vector* b)
    {
        return a->at(dimension)<b->at(dimension);
    });

    // Bereken mediaan van de gesorteerde vector. Gebruik integerdivision om naar beneden af te ronden
    size_t middle = points.size()/2;
    
    // Maak een nieuwe KDNode aan en sla het adres daarvan op in de pointer node. Geef aan de nieuwe
    // node mee: de pointer naar de middelste Vector, nullptr voor het linker en rechterkind en de huidige dimensie
    KDNode* node = new KDNode{points[middle], nullptr, nullptr, dimension};

    //Linker subset: functie leftPoints die teruggeeft een lijst met pointers naar vectoren en sorteert van
    //het begin tot het midden
    vector<const Vector*>leftPoints(points.begin(), points.begin()+middle);


    //Rechter subset: functie rightPoints die teruggeeft een lijst met pointers naar vectoren en sorteert van
    //het midden + 1 tot en met het eind.
    vector<const Vector*>rightPoints(points.begin()+middle+1, points.end());

    //Bepaal de volgende dimensie voor de volgende laag van de boom.
    size_t nextDimension = (dimension + 1) % Vector::MAX_DIMENSIONS;

    //Bouw recursief de linker subtree (dus denk hier aan de call stack: pas daar de connectie maken)
    node->left = buildKDTree(leftPoints, nextDimension);

    // Bouw recursief de rechter subtree.
    node->right = buildKDTree(rightPoints, nextDimension);

    return node;
}

void KDTree::printTree() const
{
    printTree(root,0);
}

//Helperfunctie
void KDTree::printTree(const KDNode* node, int depth) const
{
    if(node==nullptr)
    {
        return;
    }

    for(int i=0; i<depth;++i)
    {
        cout << "   ";
    }

    cout<<"["
        << node->vector->at(0)<< ", "
        << node->vector->at(1)<<", "
        << node->vector->at(2)<<"]"
        << " [dim "
        << node->dimension
        << "]\n";

    printTree(node->left, depth+1);
    printTree(node->right, depth+1);
}

KDTree::~KDTree()
{
    destroyTree(root);
}

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

KDNode* KDTree::nearestNeighbor(KDNode* node, const Vector& query, KDNode* best)
{

    // pointer first naar KDNode
    KDNode* first;

    // pointer second naar KDNOde
    KDNode* second;

    //Basecase: als we beneden aankomen, retourneer pointer
    if(node == nullptr)
    {
        return best;
    }
    //Als de query op dimensie x groter dan vector op die dimensie, dan naar rechts.
    if(query.at(node->dimension)<node->vector->at(node->dimension))
        // Maar omdat we later eventueel nog naar links kunnen gaan 
        {
            first = node->left;
            second = node->right;
        }
    else
        // Anders   
        {
            first = node->right;
            second = node->left;
        }

    //1-9-2: Onderzoek de kant die het dichts bij de query ligt
    best =  nearestNeighbor(first, query, best);
    
    //Maak een variabele distance aan van het type double. 
    // Dit is de afstand van de query naar de huidige node
    double distance = query.distanceTo(*node->vector);

    // Is de huidige node beter dan best?
    if(best == nullptr || distance < query.distanceTo(*best->vector))
    {
        best = node;
    }

    // Afstand van query tot het splitsingsvlak
    double planeDistance = std::abs(query.at(node->dimension)-node->vector->at(node->dimension));

    // Alleen de andere kant onderzoeken als daar mogelijk een betere vector kan zitten
    if(best == nullptr || planeDistance<query.distanceTo(*best->vector))
    {
        best = nearestNeighbor(second,query,best);
    }

    return best;

}
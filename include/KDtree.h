//25-8-26: KD-tree

#pragma once

#include "Vector.h"
//geeft standaard C++ types zoals std::size_t
#include <cstddef>
#include <vector>

struct KDNode
{
    const Vector* vector;
    KDNode* left;
    KDNode* right;
    std::size_t dimension;
};

class KDTree
{
    private:
        KDNode* root;

        // 28-8-2026: BELANGRIJK: Het gaat om recursief oproepen. De dimensie verhoogt iedere keer met oproepen met 1.
        // Wat we dan niet doen: Binnen de functie de dimensie declareren en initialiseren op 0, maar ook niet hier
        // in de header file de dimension initialiseren op 0. Wat we wel doen: We roepen de eerste keer gewoon de functie op
        // en handmatig voeren we 0 in.
        KDNode* buildKDTree(std::vector<const Vector*>points, std::size_t dimension);

        //TestFunctie private helperfunctie: Tweede print functie: voor het recursieve werkt
        void printTree(const KDNode* node, int depth) const;

        void destroyTree(KDNode* node);

    public:
        KDTree(const std::vector<Vector>& vectors);
        ~KDTree();
        
        //TestFunctie om te kijken of de KDTree werkt
        void printTree() const;
        

};
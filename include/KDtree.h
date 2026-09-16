//25-8-26: KD-tree

#pragma once

#include "Vector.h"
#include "MaxHeap.h"

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

        //28-8-26: Zoekfunctie voor de KDTree: Roep de functie nearestNeighbor op die een pointer heeft naar een KDnode genaamdnode, referentie naar een vector genaamd query
        // en een pointer genaamd best naar een KDNode object.
        KDNode* nearestNeighbor(KDNode* node, const Vector& query, KDNode* best);
        //15-9-2026 Helper functie 
        void kNearestNeighbors(KDNode* node, const Vector& query, std::size_t k, MaxHeap& heap) const;
        

    public:
        KDTree(const std::vector<Vector>& vectors);
        //15-9-2026: Toegevoegd t.b.v. NearestNeighbors uit Heap zoeken: let op: dezelfde naam als
        // die in de private functie: dit is overloading.
        std::vector<const Vector*> kNearestNeighbors(const Vector& query, std::size_t k) const;
       
        ~KDTree();

        //TestFunctie om te kijken of de KDTree werkt
        void printTree() const;
        

        

};
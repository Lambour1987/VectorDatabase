//3-9-2026: Gekopieerd van MaxHeap en gewijzigd naar MinHeap

//22-8-2026
// 3-9-2026: bedenk of D-Ary heap iets nuttigs is hier
// 4-9-2026: Kopie van MaxHeap en aangepast. Commentaar niet gewijzigd.

#include "../include/MinHeap.h"

#include <iostream>

using namespace std;

//Functie push van de Class Maxheap die als iput een waarde gebruikt
void MinHeap::push(std::pair<double, GraphNode*> value)
{
    //Push waarde op de vector (achteraan)
    values.push_back(value);
    {
        //Haal laatste index op en sla deze op als variabele ChildIndex
        int ChildIndex = values.size()-1;

        //Als ChildIndex 0 is, dan is het einde van het programma
        while(ChildIndex != 0)
        {
            int ParentIndex = (ChildIndex-1)/2;

            //Als waarde element op ChildIndex > waarde element ParentIndex, swap
            if(values[ChildIndex].first<values[ParentIndex].first)
            {
                //Wissel de waarden
                std::swap(values[ChildIndex], values[ParentIndex]);
                //De nieuwe Childindex is de huidige ParentIndex
                ChildIndex = ParentIndex;
            }
            else
            {
                // Stop het programma
                break;
            }

        }
    }
}


//Geeft de waarde van het grootste element op index 0 terug
//Gebruik optional<int> om onderscheid te maken indien een waarde in de heap 0 is en een lege heap.
//25-8-26: van std::optional<int> MaxHeap::top() const naar
optional<pair<double, GraphNode*>> MinHeap::top() const
{
    // De vector values komt uit de class en is een membervariabele (dus niet als parameter meenemen)
    // Als het element op index 0 niet leeg is, 
    if(values.empty())
    {
        cout<<"Values bevat geen waarden"<<endl;

        //Omdat we optional<int> gebruiken
        return nullopt;
    }
    return values[0];
}

//DUs poppen van heap. Niet teruggeven
void MinHeap::pop()
{
    if(empty())
    {
        cout<<"Values bevat geen waarden"<<endl;
        return;
    }

    int lastindex = values.size()-1;
    swap(values[0], values[lastindex]);
    values.pop_back();
    
    // Nu van boven naar beneden swappen als beneden>boven. Swap met kind met hoogste waarde
    int currentParent = 0;
    //Bereken de index van het rechterkind (sowieso berekenen, ook al bestaat deze index niet!)

    while(true)
    {
        //Straks kijken of die uberhaupt bestaat
        int leftChild = 2*currentParent+1;
        //Bereken de index van het linkerkind (idem)
        int rightChild = 2*currentParent+2;
    
        if(rightChild<values.size())
        {
            int minChildIndex;
            //Hier kijken of het linkerkind groter is dan het rechterkind of andersom
            if(values[leftChild].first<values[rightChild].first)
            {
                minChildIndex = leftChild;
            }
            else
            {
                minChildIndex = rightChild;
            }
            //Hier kijken of het grootste kind ook groter is dan element op current index
            if(values[minChildIndex].first<values[currentParent].first)
            {
                swap(values[currentParent],values[minChildIndex]);
                //CurrentParent staat op een index
                currentParent = minChildIndex;
            }
            else
            {
                break;
            }
        }
        else if(leftChild<values.size())
        {

            if(values[leftChild].first<values[currentParent].first)
            {
                swap(values[currentParent],values[leftChild]);
                currentParent = leftChild;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}

//Weer als extra abstractielaag: gebruiker hoeft niet te weten dat wij intern een vector gebruiken.
bool MinHeap::empty() const
{
    return values.empty();
}

// 25-8-26: Hier een functie size() van mijn eigen Class MaxHeap. Zodat gebruiker niet weet dat
// wij intern een vector gebruiken.
// const achter de functie: Functie belooft dat hij het MaxHeap object niet verandert.
size_t MinHeap::size() const
{
    return values.size();
}
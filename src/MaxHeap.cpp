//22-8-2026

#include "../include/MaxHeap.h"

#include <iostream>

using namespace std;

//Functie push van de Class Maxheap die als iput een waarde gebruikt
void MaxHeap::push(int value)
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
            if(values[ChildIndex]>values[ParentIndex])
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
std::optional<int> MaxHeap::top() const
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
void MaxHeap::pop()
{
    if(empty())
    {
        cout<<"Values bevat geen waarden"<<endl;
        return;
    }

    int lastindex = values.size()-1;
    std::swap(values[0], values[lastindex]);
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
            int maxChildIndex;
            //Hier kijken of het linkerkind groter is dan het rechterkind of andersom
            if(values[leftChild]>values[rightChild])
            {
                maxChildIndex = leftChild;
            }
            else
            {
                maxChildIndex = rightChild;
            }
            //Hier kijken of het grootste kind ook groter is dan element op current index
            if(values[maxChildIndex]>values[currentParent])
            {
                swap(values[currentParent],values[maxChildIndex]);
                //CurrentParent staat op een index
                currentParent = maxChildIndex;
            }
            else
            {
                break;
            }
        }
        else if(leftChild<values.size())
        {

            //Hier alleen berekenen of currentParent Groter is dan leftChild
            if(values[leftChild]>values[currentParent])
            {
                swap(values[currentParent],values[leftChild]);
                currentParent = leftChild;
            }
            else
            {
                //Als linkerkind niet groter is dan huidig element. Stop dan.
                break;
            }
        }
        else
        {
            //Dan geen kinderen, dus ga uit de loop
            break;
        }
    }
}

//Weer als extra abstractielaag: gebruiker hoeft niet te weten dat wij intern een vector gebruiken.
bool MaxHeap::empty() const
{
    return values.empty();
}

// 25-8-26: Hier een functie size() van mijn eigen Class MaxHeap. Zodat gebruiker niet weet dat
// wij intern een vector gebruiken.
// const achter de functie: Functie belooft dat hij het MaxHeap object niet verandert.
size_t MaxHeap::size() const
{
    return values.size();
}
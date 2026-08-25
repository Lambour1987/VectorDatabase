//22-8-2026

#pragma once
#include <vector>
#include <optional>
#include <utility>

#include "Vector.h"


//24-8-26: BELANGRIJK: Priority Queue is dus het gedrag, en de heap de implementatie. Je zou n.l. ook een
// PQ kunnen hebben door de max functie op een array te pakken, element te deleten en opnieuw max te pakken
// en te deleten totdat je de top grootste hebt. Kan natuurlijk ook met een gesorteerde vector! eerst sorteren,
// dan max pakken. Ook dan heb je een PQ.
class MaxHeap
{
    private:
        //25-8-2026: We gaan nu dus de heap maken zodat die pairs aankan. Kan weg: std::vector<int> values; wordt:
        std::vector<std::pair<double,const Vector*>> values;

    public:
        //25-8-26: ook omgezet 
        void push(std::pair<double,const Vector*> value);
        //optional int: Als de top '0' returned: kan dat betekenen dat er of geen waarde is of dat
        // de waarde 0 is. Wij kunnen dat onderscheid niet maken. Dus het kan zijn dat
        //25-8-26: Gewijzigd: De heap gaat gebruik maken van pairs <double, Vector*>
        std::optional<std::pair<double, const Vector*>>top() const;
        void pop();
        bool empty() const;
        std::size_t size() const;

};
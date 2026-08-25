//22-8-2026

#pragma once

#include <vector>
#include <optional>


//24-8-26: BELANGRIJK: Priority Queue is dus het gedrag, en de heap de implementatie. Je zou n.l. ook een
// PQ kunnen hebben door de max functie op een array te pakken, element te deleten en opnieuw max te pakken
// en te deleten totdat je de top grootste hebt. Kan natuurlijk ook met een gesorteerde vector! eerst sorteren,
// dan max pakken. Ook dan heb je een PQ.
class MaxHeap
{
    private:
        std::vector<int> values;

    public:
        void push(int value);
        //optional int: Als de top '0' returned: kan dat betekenen dat er of geen waarde is of dat
        // de waarde 0 is. Wij kunnen dat onderscheid niet maken. Dus het kan zijn dat
        std::optional<int> top() const;
        void pop();
        bool empty() const;

        //We 
        std::size_t size() const;
};
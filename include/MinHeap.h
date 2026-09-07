//3-9-2026: MinHeap t.b.v. Dijkstra

//Voorkom dat headers vaker geimporteerd worden
#pragma once

#include <vector>
#include <optional>
#include <utility>


//forward declartion
class GraphNode;

class MinHeap
{
    private:
        std::vector<std::pair<double, GraphNode*>> values;

    public:
        void push(std::pair<double, GraphNode*> value);
        std::optional<std::pair<double, GraphNode*>> top() const;
        void pop();
        bool empty() const;
        std::size_t size() const;
};

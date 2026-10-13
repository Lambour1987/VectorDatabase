//13-10-2026


#include "../include/KDTree.h"
#include "../include/Vector.h"
#include "../include/KnowledgeItem.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
#include <queue>

using Clock = std::chrono::steady_clock;

struct Result
{
    const KnowledgeItem* item;
    double distance;
};


static std::vector<Result> bruteForceHeap(
    const std::vector<std::unique_ptr<KnowledgeItem>>& items,
    const Vector& query,
    std::size_t k)
{
    // De grootste afstand staat bovenaan.
    auto compare = [](const Result& a, const Result& b)
    {
        return a.distance < b.distance;
    };

    std::priority_queue<
        Result,
        std::vector<Result>,
        decltype(compare)
    > heap(compare);

    for (const auto& item : items)
    {
        Result candidate{
            item.get(),
            query.distanceTo(item->vector)
        };

        if (heap.size() < k)
        {
            heap.push(candidate);
        }
        else if (candidate.distance < heap.top().distance)
        {
            heap.pop();
            heap.push(candidate);
        }
    }

    std::vector<Result> results;
    results.reserve(heap.size());

    while (!heap.empty())
    {
        results.push_back(heap.top());
        heap.pop();
    }

    std::sort(
        results.begin(),
        results.end(),
        [](const Result& a, const Result& b)
        {
            return a.distance < b.distance;
        });

    return results;
}


static double milliseconds(Clock::duration duration)
{
    return std::chrono::duration<double, std::milli>(duration).count();
}

int main()
{
    
    constexpr std::size_t dimensions = 384;
    constexpr std::size_t k = 5;
    constexpr int repetitions = 100;

    const std::vector<std::size_t> sizes = {
    1000, 5000, 10000, 20000

    };

    std::mt19937 generator(42);
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);

    std::cout << "KD-tree versus brute-force benchmark\n";
    std::cout << "Dimensies: " << dimensions
              << ", k: " << k
              << ", queryherhalingen: " << repetitions << "\n\n";

    std::cout << "Items | Build KD (ms) | KD/query (ms) "
             "| Heap brute/query (ms) | Correct\n";
    std::cout << "-------------------------------------------------------------\n";

    for (const std::size_t count : sizes)
    {
        std::vector<std::unique_ptr<KnowledgeItem>> items;
        items.reserve(count);

        for (std::size_t i = 0; i < count; ++i)
        {
            std::vector<double> values(dimensions);

            for (double& value : values)
            {
                value = distribution(generator);
            }

            items.push_back(std::make_unique<KnowledgeItem>(
                static_cast<int>(i + 1),
                Vector(values),
                "benchmark item"
            ));
        }

        std::vector<Vector> queries;
        queries.reserve(repetitions);

        for (int i = 0; i < repetitions; ++i)
        {
            std::vector<double> values(dimensions);

            for (double& value : values)
            {
                value = distribution(generator);
            }

            queries.emplace_back(values);
        }

        const auto buildStart = Clock::now();
        KDTree tree(items);
        const auto buildEnd = Clock::now();

        double kdTotalMs = 0.0;
        double bruteTotalMs = 0.0;
        bool correct = true;

        for (const Vector& query : queries)
        {
            const auto kdStart = Clock::now();
            const auto kdResults = tree.kNearestNeighbors(query, k);
            const auto kdEnd = Clock::now();

            const auto bruteStart = Clock::now();
            const auto bruteResults = bruteForceHeap(items, query, k);
            const auto bruteEnd = Clock::now();

            kdTotalMs += milliseconds(kdEnd - kdStart);
            bruteTotalMs += milliseconds(bruteEnd - bruteStart);

            if (kdResults.size() != bruteResults.size())
            {
                correct = false;
                continue;
            }

            for (std::size_t i = 0; i < kdResults.size(); ++i)
            {
                const double kdDistance =
                    query.distanceTo(kdResults[i]->vector);

                if (kdResults[i]->id != bruteResults[i].item->id ||
                    std::abs(kdDistance - bruteResults[i].distance) > 1e-9)
                {
                    correct = false;
                }
            }
        }

        const double kdAverage = kdTotalMs / repetitions;
        const double bruteAverage = bruteTotalMs / repetitions;

        std::cout << count << " | "
                  << milliseconds(buildEnd - buildStart) << " | "
                  << kdAverage << " | "
                  << bruteAverage << " | "
                  << (correct ? "JA" : "NEE") << '\n';

        if (!correct)
        {
            std::cerr << "FOUT: resultaten verschillen bij "
                      << count << " items.\n";
            return 1;
        }
    }

    std::cout << "\nAlle benchmarkresultaten gecontroleerd.\n";
    return 0;
}

#include "../include/VectorDatabase.h"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

int main()
{
    VectorDatabase database;

    Vector vector1{1.0, 2.0, 3.0};
    Vector vector2{4.0, 5.0, 6.0};

    database.add(vector1, "eerste tekst");
    database.add(vector2, "tweede tekst");

    assert(database.size() == 2);

    assert(database.get(0).id == 1);
    assert(database.get(1).id == 2);

    const KnowledgeItem& item = database.getById(2);

    assert(item.id == 2);

    cout << "Found ID 2: "
         << item.text
         << "\n";

    try
    {
        database.getById(999);
        assert(false);
    }
    catch(const runtime_error&)
    {
        cout << "Missing ID correctly detected!\n";
    }
    assert(database.containsId(1));
    assert(database.containsId(2));
    assert(!database.containsId(999));

    cout << "containsId tests passed!" << endl;

    database.add(Vector{7.0, 8.0, 9.0}, "derde tekst");
    database.add(Vector{10.0, 11.0, 12.0}, "vierde tekst");
    database.add(Vector{13.0, 14.0, 15.0}, "vijfde tekst");
    database.add(Vector{16.0, 17.0, 18.0}, "zesde tekst");

    cout << "VectorDatabase tests passed!" << endl;

    const auto results = database.findByIdRange(3, 6);

    if(results.size() != 4)
    {
        throw std::runtime_error(
            "VectorDatabase range test failed: expected 4 records"
        );
    }

    for(std::size_t i = 0; i < results.size(); i++)
    {
        if(results[i]->id != static_cast<int>(i) + 3)
        {
            throw std::runtime_error(
                "VectorDatabase range test failed: incorrect IDs or order"
            );
        }
    }

    std::cout << "VectorDatabase range tests passed!" << std::endl;

    // Edge case 1: bereik opvragen uit een lege database.
    VectorDatabase emptyDatabase;

    const auto emptyResults = emptyDatabase.findByIdRange(1, 10);

    assert(emptyResults.empty());

    cout << "Empty database range test passed!" << endl;


    // Edge case 2: bereik met precies één bestaand ID.
    const auto singleResult = database.findByIdRange(3, 3);

    assert(singleResult.size() == 1);
    assert(singleResult[0]->id == 3);

    cout << "Single ID range test passed!" << endl;


    // Edge case 3: bereik buiten alle bestaande IDs.
    const auto outsideResults = database.findByIdRange(100, 200);

    assert(outsideResults.empty());

    cout << "Outside range test passed!" << endl;


    // Edge case 4: bereik met ontbrekende IDs.
    const auto missingResults = database.findByIdRange(2, 4);

    assert(missingResults.size() == 3);
    assert(missingResults[0]->id == 2);
    assert(missingResults[1]->id == 3);
    assert(missingResults[2]->id == 4);

    cout << "Missing IDs range test passed!" << endl;

    //13-10-2026: toegevoegd
    
    // Test: gecombineerde ID-filter en vectorzoekfunctie.
    Vector query{4.0, 5.0, 6.0};

    const auto nearestResults =
        database.findNearestKInIdRange(query, 2, 1, 3);

    assert(nearestResults.size() == 2);

    // ID 2 heeft exact dezelfde vector als de query.
    assert(nearestResults[0].item->id == 2);
    assert(nearestResults[0].distance == 0.0);

    // ID 1 en ID 3 liggen even ver van de query.
    // Beide hebben afstand sqrt(27). We controleren daarom
    // dat het tweede resultaat een van deze twee items is.
    assert(
        nearestResults[1].item->id == 1 ||
        nearestResults[1].item->id == 3
    );

    cout << "Combined ID range and nearest-K test passed!" << endl;

    //13-10-2026: meer tests
    // Randgeval 1: k = 0 moet een invalid_argument opleveren.
    bool zeroKThrows = false;

    try {
        database.findNearestKInIdRange(query, 0, 1, 3);
    }
    catch (const std::invalid_argument&) {
        zeroKThrows = true;
    }

    assert(zeroKThrows);
    cout << "Zero-K edge case passed!" << endl;


    // Randgeval 2: minId > maxId moet een invalid_argument opleveren.
    bool invalidRangeThrows = false;

    try {
        database.findNearestKInIdRange(query, 2, 5, 2);
    }
    catch (const std::invalid_argument&) {
        invalidRangeThrows = true;
    }

    assert(invalidRangeThrows);
    cout << "Invalid ID range edge case passed!" << endl;


    // Randgeval 3: een geldig bereik zonder matches geeft lege resultaten.
    const auto noMatches =
        database.findNearestKInIdRange(query, 2, 100, 200);

    assert(noMatches.empty());
    cout << "Empty candidate range edge case passed!" << endl;

    //13-10-2026: Toegevoegd
    // Test: maxDistance = 0 accepteert alleen een exacte match.
    const auto exactResults =
        database.findNearestKInIdRange(query, 3, 1, 6, 0.0);

    assert(exactResults.size() == 1);
    assert(exactResults[0].item->id == 2);
    assert(exactResults[0].distance == 0.0);

    cout << "Max-distance exact-match test passed!" << endl;


    // Test: resultaten boven de afstandsgrens worden uitgesloten.
    const auto limitedResults =
        database.findNearestKInIdRange(query, 3, 1, 6, 5.5);

    assert(limitedResults.size() == 3);
    assert(limitedResults[0].item->id == 2);
    assert(limitedResults[0].distance == 0.0);
    assert(limitedResults[1].distance <= 5.5);
    assert(limitedResults[2].distance <= 5.5);

    // ID 1 en ID 3 liggen beide op afstand sqrt(27), ongeveer 5.196.
    assert(
        limitedResults[1].item->id == 1 ||
        limitedResults[1].item->id == 3
    );
    assert(limitedResults[1].distance <= 5.5);

    cout << "Max-distance filtering test passed!" << endl;

    // Test: een negatieve maximale afstand is ongeldig.
    bool negativeDistanceThrows = false;

    try {
        database.findNearestKInIdRange(query, 3, 1, 6, -1.0);
    }
    catch (const std::invalid_argument&) {
        negativeDistanceThrows = true;
    }

    assert(negativeDistanceThrows);

    cout << "Negative max-distance test passed!" << endl;

    //13-10-2026: nog een test
        // Test: afstand tussen vectoren met dezelfde dimensie.
    Vector distanceVector1{1.0, 2.0};
    Vector distanceVector2{4.0, 6.0};

    const double distance =
        distanceVector1.distanceTo(distanceVector2);

    assert(distance == 5.0);

    cout << "Same-dimension distance test passed!" << endl;


    // Test: vectoren met verschillende dimensies moeten worden geweigerd.
    Vector differentDimensionVector{1.0, 2.0, 3.0};

    bool dimensionMismatchThrows = false;

    try
    {
        distanceVector1.distanceTo(differentDimensionVector);
    }
    catch (const std::invalid_argument&)
    {
        dimensionMismatchThrows = true;
    }

    assert(dimensionMismatchThrows);

    cout << "Different-dimension distance test passed!" << endl;

    //13-10-2026: nieuwe toegevoegd
    
    // 13-10-2026: KD-tree-integratietests

    // Test 1: vergelijk findNearestK met brute force.
    const Vector kdQuery{5.0, 6.0, 7.0};

    const auto kdResults = database.findNearestK(kdQuery, 3);

    // Onafhankelijke brute-forceberekening over alle items.
    std::vector<NearestResult> bruteForceResults;

    for (std::size_t i = 0; i < database.size(); ++i)
    {
        const KnowledgeItem& current = database.get(i);

        bruteForceResults.push_back({
            &current,
            kdQuery.distanceTo(current.vector)
        });
    }

    std::sort(
        bruteForceResults.begin(),
        bruteForceResults.end(),
        [](const NearestResult& a, const NearestResult& b)
        {
            return a.distance < b.distance;
        }
    );

    assert(kdResults.size() == 3);

    for (std::size_t i = 0; i < kdResults.size(); ++i)
    {
        assert(kdResults[i].item->id == bruteForceResults[i].item->id);
        assert(
            std::abs(
                kdResults[i].distance - bruteForceResults[i].distance
            ) < 1e-9
        );
    }

    cout << "KD-tree versus brute-force test passed!" << endl;


    // Test 2: na een eerdere zoekopdracht een nieuw item toevoegen.
    // De volgende zoekopdracht moet de verouderde KD-tree vernieuwen.
    const Vector newVector{5.0, 6.0, 7.0};

    database.add(newVector, "KD-tree nieuw item");

    const auto updatedResults = database.findNearestK(kdQuery, 1);

    assert(updatedResults.size() == 1);
    assert(updatedResults[0].item->text == "KD-tree nieuw item");
    assert(updatedResults[0].distance == 0.0);

    cout << "KD-tree rebuild after add test passed!" << endl;


    // Test 3: vraag meer resultaten op dan er items zijn.
    const auto allResults =
        database.findNearestK(kdQuery, database.size() + 10);

    assert(allResults.size() == database.size());

    for (std::size_t i = 1; i < allResults.size(); ++i)
    {
        assert(allResults[i - 1].distance <= allResults[i].distance);
    }

    cout << "KD-tree k larger than database size test passed!" << endl;
    
    return 0;
}
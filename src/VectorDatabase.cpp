//18-8-2026 Gemaakt
#include "../include/MaxHeap.h"
#include "../include/VectorDatabase.h"
#include "../include/KnowledgeItem.h"

//Om runtime errors op te vangen
#include <stdexcept>

#include <algorithm>
#include <queue>

using namespace std;

//20-8-26: hier een struct: niet ni de header. Omdat het alleen een hulpmiddel is voor de implementatie van findNearestK
struct CompareDistance
{
    bool operator()(const NearestResult& a, const NearestResult& b) const
    {
        return a.distance<b.distance;
    }
};



// //8-10-26: void VectorDatabase::add(const Vector& vector, const std::string& text)
// {
//     auto item = std::make_unique<KnowledgeItem>(nextId,vector,text);

//     items.push_back(std::move(item));

//     nextId++;
// }
void VectorDatabase::add(const Vector& vector, const string& text)
{
    auto item = make_unique<KnowledgeItem>(
        nextId,
        vector,
        text
    );

    KnowledgeItem* itemPointer = item.get();

    items.push_back(move(item));

    idIndex.insert(itemPointer->id, itemPointer);

    nextId++;
}

size_t VectorDatabase::size() const
{
    return items.size();
}

//30-9-2026: Deze functie ombouwen
//8-10-2026: Tijdelijk aanpassen const vector<KnowledgeItem>& VectorDatabase::getVectors() const
//{
    //return items;
//}
const vector<unique_ptr<KnowledgeItem>>& VectorDatabase::getVectors() const
{
    return items;
}

//19-8-2026: Functie om terug te vinden in Vector
// BELANGRIJK: ipv vectors[index] doen we vectors.at(index): Die controleert gelijk of de index geldig is.
const KnowledgeItem& VectorDatabase::get(std::size_t index) const
{
    return *items.at(index);
}




// 19-8-26: Functie die dichtsbijzijnde vector binnen de Vectorendatabase zoekt
// de functie findNearest krijgt als parameter query, waarbij query een referentie is naar een Vector en die
// vector mag niet gewijzigd worden.
// 19-8-26: Deze gaan we aanpassen: const Vector& VectorDatabase::findNearest(const Vector& query) const
NearestResult VectorDatabase::findNearest(const Vector&query) const
{
    //als de vectordatabase leeg is, dan gooi dit naar de uitzondering en 
    // geef het bericht: "Database is empty"
    if(items.empty())
    {
        throw std::runtime_error("Database is empty");
    }

    std::size_t bestIndex = 0;

    // Maak een varaibele smallestDistance aan met datatype Double en geef hem als beginwaarde
    // de afstand tussen qeury en de eerste Vector in de database.
    // Let op: we houden logischerwijs tijdens het doorlopen van de database als het kleinste element bij, dat
    // scheelt straks als we in O(n) nogmaals de hele vector zouden moeten doorlopen met de 'min' functie. 
    //8-10-26: dus van double smallestDistance = query.distanceTo(items.at(0).vector); want unique pointer
    double smallestDistance = query.distanceTo(items.at(0)->vector);
    //Loop vanaf de tweede vector naar het einde van de vectorDatabase. (de eerste was al opgenomen)
    for(size_t i = 1; i<items.size();i++)
    {
        // Bereken de afstand van huidige vector met de andere vector door het oproepen van de distance functie
        // en sla de uitkomst op in een variabele distance van het datatype double
        double distance = query.distanceTo(items.at(i)->vector);

        // Als de huidige afstand kleiner is dan de kleinste afstand, dan wordt de kleinste afstand de huidige afstand
        // en dan wordt bestIndex de huidige index waar we nu op staan
        if(distance < smallestDistance)
        {
            smallestDistance = distance;
            bestIndex = i;
        }
    }
    //19-8-26:
    //20-8-26: gewijzigd omdat we ipv referentie nu met pointers gaan werken. was: NearestResult result{vectors.at(bestIndex), smallestDistance}; wordt:
    //8-10-2026: gewijzigd NearestResult result{&items.at(bestIndex), smallestDistance};
    NearestResult result{items.at(bestIndex).get(),smallestDistance};

    // Return de vector op de beste index.
    // 19-8-26 Nu dit aanpassen omdat we de functie hebben aangepast return vectors.at(bestIndex);
    return result; 
}


//19-8-26: Maak een functie findNearestK die als input een referentie naar een vector heeft die niet te wijzigen is en noem
// hem query en een variabele k van het type size_t. Als output een vector van dichtsbijzijnde resultaten
std::vector<NearestResult> VectorDatabase::findNearestK(const Vector& query, size_t k) const
{
    //20-8-2026: Controle als k=0
    if(k==0)
    {
        //gooi naar exepction: invalid argument: k must be greater than 0l
        throw invalid_argument("k must be greater than 0");
    }

    //20-8-26: als vectors leeg zijn
    if(items.empty())
    {
        //gooi naar exception: 
        throw std::runtime_error("Database is empty");
    }

    //Maak een variabele results van het type Vector en gebruik Nearest Results als datatype
    //20-8-26: Vervangen omdat we nu een heap gaan vector<NearestResult> results;
    //25-8-26: PQ kan eruit: priority_queue<NearestResult, vector<NearestResult>,CompareDistance> heap;
    MaxHeap heap;

    //Loop door de vector omvang heen 
    for(size_t i = 0; i < items.size();i++)
    {
        //maak een variabele distance van het type double die met de afstand tussen query en de Vector op index i
        //30-9-2026: er staat nu dus items.at. Maar kan nog niet omdat de MaxHeap nog aangepast moet worden.
        //8-10-2026: Gewijzigd van double distance = query.distanceTo(items.at(i).vector);
        double distance = query.distanceTo(items.at(i)->vector);
        // Maak een NearestResult-object en noem deze result en initialiseer de referentie naar
        // de huidige Vector en de bijbehorende afstand.
        // We gaan nu met pointers werken ipv referenties dus NearestResult result{vectors.at(i), distance}; wordt
        // BELANGRIJK: & kan in C++ verschillende dingen betekenen.
        // Bij een referentie staat & achter het datatype: const Vector&
        // Bij een pointer kan & vóór een object staan: &vectors.at(i)
        // Dan betekent &:
        // "geef het geheugenadres van dit object".
        // Dat adres wordt opgeslagen in de pointer.
        // Dus hier geven we het adres van de Vector op index i door aan result.vector.
        // 25-8-26 kan eruit: NearestResult result{&vectors.at(i), distance};
        
        //voeg resultaat toe aan heap result
        // 25-8-26: MaxHeap dus pair {} nodig ipv heap.push(result); wordt
        //8-10-2026 heap.push({distance, &items.at(i)});
        heap.push({distance, items.at(i).get()});
        //20-8-26: wordt een heap results.push_back(result);
        //Als heap groter is dan k
        if(heap.size()>k)
        {
            //Haal top eraf en push
            heap.pop();
        }
    }

    std::vector<NearestResult> results;

    while(!heap.empty())
    {
        //25-8-26: auto result: omdat de functie top() teruggeeft: optional<pair<double, const Vector*>> MaxHeap::top() const. Dus dan is auto makkelijker
        // en je kan aan de functie naam al zien wat het gaat worden.
        auto result = heap.top();

        //25-8-26: has_value: memberfunction van optional<int> (omdat de waarde 0 kan betekenen: 0 of ongeldig(geen waarde gevonden))
        if(result.has_value())
        {
        //25-8-26: MaxHeap gebruiken dus van results.push_back(heap.top()); wordt
            results.push_back({result->second, result->first});
            heap.pop();
        }
    }
    //20-8-2026: Sorteren bruteforce met lambda expression eruit, want we gebruiken een heap
    //20-8-2026: Reverse functie nu gebruiken om de resultaten om te draaien
    reverse(results.begin(), results.end());
    // retourneer resultaat
    return results;

}

const KnowledgeItem& VectorDatabase::getById(int id) const
{
    const KnowledgeItem* item = idIndex.find(id);

    if(item == nullptr)
    {
        throw std::runtime_error("ID not found");
    }

    return *item;
}

//8-10-2026 Toegevoegd
bool VectorDatabase::containsId(int id) const
{
    return idIndex.find(id) != nullptr;
}
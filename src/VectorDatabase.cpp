//18-8-2026 Gemaakt
#include "../include/VectorDatabase.h"

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



void VectorDatabase::add(const Vector& vector)
{
    vectors.push_back(vector);
}

std::size_t VectorDatabase::size() const
{
    return vectors.size();
}

//19-8-2026: Functie om terug te vinden in Vector
// BELANGRIJK: ipv vectors[index] doen we vectors.at(index): Die controleert gelijk of de index geldig is.
const Vector& VectorDatabase::get(std::size_t index) const
{
    return vectors.at(index);
}




// 19-8-26: Functie die dichtsbijzijnde vector binnen de Vectorendatabase zoekt
// de functie findNearest krijgt als parameter query, waarbij query een referentie is naar een Vector en die
// vector mag niet gewijzigd worden.
// 19-8-26: Deze gaan we aanpassen: const Vector& VectorDatabase::findNearest(const Vector& query) const
NearestResult VectorDatabase::findNearest(const Vector&query) const
{
    //als de vectordatabase leeg is, dan gooi dit naar de uitzondering en 
    // geef het bericht: "Database is empty"
    if(vectors.empty())
    {
        throw std::runtime_error("Database is empty");
    }

    std::size_t bestIndex = 0;

    // Maak een varaibele smallestDistance aan met datatype Double en geef hem als beginwaarde
    // de afstand tussen qeury en de eerste Vector in de database.
    // Let op: we houden logischerwijs tijdens het doorlopen van de database als het kleinste element bij, dat
    // scheelt straks als we in O(n) nogmaals de hele vector zouden moeten doorlopen met de 'min' functie. 
    double smallestDistance = query.distanceTo(vectors.at(0));

    //Loop vanaf de tweede vector naar het einde van de vectorDatabase. (de eerste was al opgenomen)
    for(size_t i = 1; i<vectors.size();i++)
    {
        // Bereken de afstand van huidige vector met de andere vector door het oproepen van de distance functie
        // en sla de uitkomst op in een variabele distance van het datatype double
        double distance = query.distanceTo(vectors.at(i));

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
    NearestResult result{&vectors.at(bestIndex), smallestDistance};

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
    if(vectors.empty())
    {
        //gooi naar exception: 
        throw std::runtime_error("Database is empty");
    }

    //Maak een variabele results van het type Vector en gebruik Nearest Results als datatype
    //20-8-26: Vervangen omdat we nu een heap gaan vector<NearestResult> results;
    priority_queue<NearestResult, vector<NearestResult>,CompareDistance> heap;

    //Loop door de vector omvang heen 
    for(size_t i = 0; i < vectors.size();i++)
    {
        //maak een variabele distance van het type double die met de afstand tussen query en de Vector op index i
        double distance = query.distanceTo(vectors.at(i));

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
        NearestResult result{&vectors.at(i), distance};
        
        //voeg resultaat toe aan heap result
        heap.push(result);
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
        results.push_back(heap.top());
        heap.pop();
    }

    //20-8-2026: Sorteren bruteforce met lambda expression eruit, want we gebruiken een heap
    //20-8-2026: Reverse functie nu gebruiken om de resultatne om te draaien
    reverse(results.begin(), results.end());
    // retourneer resultaat
    return results;

}

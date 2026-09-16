//17-8-2026: 

#include "../include/Vector.h"
#include "KDTree.h"

//18-8-2026: Gebruik voor throw en exception (indien we foutieve waarden in gaan voegen)
#include <stdexcept>
#include <cmath>
#include <algorithm>

using namespace std;

// BELANGRIJK: Eerste Constructor: Kan eruit, want eigenlijk dubbel en 
// We kiezen een hoofdconstructor die een andere constructor kan aanroepen.
// ===========DIT WEG==============
// Vector::Vector(initializer_list<double> initialValues):values(initialValues)
// {
//     if(initialValues.size() !=MAX_DIMENSIONS)
//     {
//         //18-8-26: in onze V! versie mogen we maar maximaal 3 waarden invoeren. Maar wat als het er meer of minder zijn?
//         // Dat is fout en die fout moeten we afhandelen met bijv. throw.
//         // Hier: als er dus meer of minder dan 3 argumenten worden opgegeven (exception), dan gooien (throw) we deze naar
//         // een plek waar deze opgevangen wordt (catch).
//         // Momenteel staat de try catch dus in main
//         throw invalid_argument("Vector must have exactly 3 dimensions");
//     }
// }
// ============TOT HIER WEG ==================
// BELANGRIJK: DUs hier roepen we met de eerste constructor de tweede op:
// Dit heet een DELEGATING Constructor.
Vector::Vector(initializer_list<double> initialValues):Vector(vector<double>(initialValues))
{

}

// HOOFDCONSTRUCTOR
// BELANGRIJK: Een constructor kan een andere constructor aanroepen.
Vector::Vector(const std::vector<double>&initialValues):values(initialValues)
{
    if(initialValues.size() != MAX_DIMENSIONS)
    {
        throw invalid_argument("Vector must have exactly 3 dimensions");
    }
}


size_t Vector::dimension() const
{
    return values.size();
}

double Vector::at(size_t index) const
{
    //19-8-26: dus nite; vlaues[index]maar values.at(index)
    return values.at(index);
}

// 19-8-2026: Deze functie berekent de afstand van huidige Vector met 1 andere vector.
// Deze functie wordt zo vaak als er Vectoren zijn in de vectorDatabase opgeroepen. Want iedere
// vector moet met elke andere vector uit de database worden vergeleken.
double Vector::distanceTo(const Vector& other) const
{
    //declaratie en initialisatie van een vector sum met 0.0
    double sum = 0.0;

    //for loop om per Vector door de dimensies te lopen (momenteel 3)
    for(std::size_t i=0; i<dimension();i++)
    {
        // Dus: we vergelijken hier 2 vectoren: de huidige op index i, en de andere op index i.
        // De andere moeten we nog doorgeven.
        double difference = at(i) - other.at(i);

        //We kwadrateren het verschil en tellen deze op
        sum += difference*difference;

    }
    // Van de totale som doen we de wortel en dat is het antwoord.
    return sqrt(sum);
}

bool Vector::operator==(const Vector& other) const
{
    return values == other.values;
}
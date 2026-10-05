//17-8-2026: 

#include "../include/Vector.h"

//18-8-2026: Gebruik voor throw en exception (indien we foutieve waarden in gaan voegen)
#include <cmath>

using namespace std;


// BELANGRIJK: DUs hier roepen we met de eerste constructor de tweede op:
// Dit heet een DELEGATING Constructor.
Vector::Vector(initializer_list<double> initialValues):Vector(vector<double>(initialValues))
{

}

Vector::Vector(const std::vector<double>& initialValues)
{
    values = initialValues;
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

    //for loop om per Vector door de dimensies te lopen
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
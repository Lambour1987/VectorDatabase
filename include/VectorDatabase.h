//18-8-2026: gemaakt.

#pragma once

#include "Vector.h"
#include <vector>

//19-8-2026: struct maken: dit omdat we in een functie een return willen geven die meerdere waarden bevat
// waarvan we de betekenis ook willen weten. Namelijk om welke vectoren het gaat en welke afstand het is.
// We zouden ook daarvoor ipv een struct een gewone vector kunnen gebruiken, maar we weten daarvan niet
// wat de betekenis is van de indices.

struct NearestResult
{
    // 'vector' is een const reference naar de gevonden Vector.
    // 'distance' bevat de afstand tussen de query en die Vector.
    //20-8-2026: BELANGRIJK: Deze gewijzigd van referentie naar pointer: het was: const Vector& vector. Het wordt const Vector* vector
    // We stappen alleen bij NearestResult van een referentie naar een pointer over omdat sort de NearestResult objecten onderling moet
    // kunnen verwisselen. Een pointer kan daarbij naar hetzelfde Vectorobject blijven wijzen en worden verwisseld.
    const Vector* vector;
    double distance;
};


class VectorDatabase
{
    private:
        //een vector van onze eigen class Vector
        std::vector<Vector> vectors;
    
    public:
        //Functie om Vector(mijn eigen Class) toe te voegen aan de database.
        void add(const Vector& vector);
        std::size_t size() const;

        //Functie om vectors terug te vinden
        //1e const: je krijgt een referentie terug naar een Vector, die je niet kan aanpassen. 
        //2e const: je kan het object van de functie zelf niet aanpassen.
        const Vector& get(std::size_t index) const;

        //19-8-26: Functie die beoordeelt welke Vector het dichtst bij ligt (bij wat ligt?)
        // 19-8-26: aangepast van: const Vector& findNearest(const Vector& query) const;naar:
        NearestResult findNearest(const Vector& query) const;

        //19-8-2026: Functie om distance te sorteren (later heap gebruiken)
        std::vector<NearestResult> findNearestK(const Vector& query, std::size_t k) const;

        
};


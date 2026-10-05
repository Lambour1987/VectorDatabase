//17-8-2026: Start met VectorDatabase

//19-8-26: Vb: stel. main.cpp importeert bestand1.h en bestand2.h. En bestand1.h importeert bestand2.h zelf ook. Dan gaat het dubbel.
// Met pragma once voorkom je dt het dubbel gaat.
#pragma once

#include <vector>
#include <initializer_list>
#include <cmath>

//Eigen class vector maken
class Vector
{
    private:
    // Data binnen private
        std::vector<double> values;
    public:
    // Functies binnen public
    // BELANGRIJK: Dit is onze constructor: n.l. Vector::Vector (functienaam is classnaam)
    // Tweede deel na ':' is de member initializer list van de constructor
        Vector(std::initializer_list<double> initialValues);
        Vector(const std::vector<double>& initialValues);
        
        //Onze Vector heeft een publieke functie dimension() die het aantal dimensies teruggeeft
        //const: deze functie mag het Vector object niet veranderen
        std::size_t dimension() const;

        //at functie
        double at(size_t index) const;

        //19-8-2026: Functie om afstand tot andere Vectoren te berekenen: Bereken de afstand van mij (this) naar de ander
        //&other is een referentie naar een andere vector
        double distanceTo(const Vector& other) const;

        //16-9-2026: Nieuw anders werkt iets niet
        bool operator==(const Vector& other)const;


};
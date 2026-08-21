//21-8-2026

#pragma once

#include "VectorDatabase.h"

//Functie vulDatabase die als input heeft een referentie naar de VectorDatabase genaamd database en een
// integer aantal vectoren. Geen const want we mogen de database aanpassen
void vulDatabase(VectorDatabase& database, int aantalVectors);
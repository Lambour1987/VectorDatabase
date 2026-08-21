//21-8-2026

#pragma once

#include "VectorDatabase.h"

// Functie voerQueryUit die als input heeft een referentie genaamd database naar een object van de VectorDatabase 
// die we niet mogen wijzigen. de functie geeft geen waarde terug.
// BELANGRIJK: Met deze verwijzing verwijzen we naar het object 'VectorDatabase', maar neit naar een vector zelf

void voerQueryUit(const VectorDatabase& database);
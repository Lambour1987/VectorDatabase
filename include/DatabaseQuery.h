//21-8-2026

#pragma once

#include "VectorDatabase.h"

// Functie voerQueryUit die als input heeft een referentie genaamd database naar een object van de VectorDatabase 
// die we niet mogen wijzigen. de functie geeft geen waarde terug.
// BELANGRIJK: Met deze verwijzing verwijzen we naar het object 'VectorDatabase', maar neit naar een vector zelf
// 22-9-26: BELANGRIJK: Deze functie staat niet binnen een class; dus wordt niet op een object aangeroepen.

void voerQueryUit(const VectorDatabase& database);
//5-10-2026

#pragma once

#include "VectorDatabase.h"
#include <string>

class DatabaseStorage
{
    public:
        //5-10-2026 de functie save is const omdat het niet de database mag aanpassen, maar wel de HD om het op te slaan
        void save(const VectorDatabase& database, const std::string& filename)const;
        VectorDatabase load(const std::string& filename) const;
};
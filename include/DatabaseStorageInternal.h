//13-10-2026: Gemaakt per 13 oktober 2026


#pragma once

#include "VectorDatabase.h"
#include <ostream>

namespace DatabaseStorageInternal
{
    void writeDatabase(const VectorDatabase& database, std::ostream& output);
}

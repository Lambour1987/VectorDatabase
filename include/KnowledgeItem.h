//29-9-2026: Knowledge item: Hiermee koppelen we onze vectorendatabase aan een betekenis

#pragma once

#include "Vector.h"
#include <string>

struct KnowledgeItem
{
    //7-10-2026: Toegevoegd
    int id;
    Vector vector;
    std::string text;

    KnowledgeItem(int id, const Vector& vector, const std::string& text)
        : id(id),vector(vector), text(text)
    {
    }
};
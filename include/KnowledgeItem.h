//29-9-2026: Knowledge item: Hiermee koppelen we onze vectorendatabase aan een betekenis

#pragma once

#include "Vector.h"
#include <string>

struct KnowledgeItem
{
    Vector vector;
    std::string text;
    KnowledgeItem(const Vector& vector, const std::string& text)
        : vector(vector), text(text)
    {
    }
};
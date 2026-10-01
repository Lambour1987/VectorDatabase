//1-10-2026

#include "TextLoader.h"

#include <fstream>
#include <stdexcept>

std::vector<std::string> loadTextsFromFile(const std::string& filename)
{
    std::ifstream file(filename);

    if(!file)
    {
        throw std::runtime_error("Kon bestand niet openen: " + filename);
    }

    std::vector<std::string> texts;

    std::string line;

    while(std::getline(file, line))
    {
        if(!line.empty())
        {
            texts.push_back(line);
        }
    }

    return texts;
}
//1-10-2026: VectorLoader: Brug tussen Python embedding en C++ database.
// Nog een keer goed doornemen

#include "VectorLoader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

//1-10-2026
std::vector<std::vector<double>> loadVectorsFromFile(const std::string& filename)
{
    //Bestand openen, bijvoorbeeld embedding/vectors.txt
    std::ifstream file(filename);

    // Als bestand openen niet gelukt is dan moeten we dit naar exception gooien.
    if(!file)
    {
        throw std::runtime_error("Kon bestand niet openen: " + filename);
    }

    //Container waarin we de embeddings gaan verzamelen:
    std::vector<std::vector<double>> vectors;

    //Hierin bewaren we iedere regel uit vectors.txt
    std::string line;

    while(std::getline(file, line))
    {
        std::istringstream stream(line);

        //1-10-2026: Deze waarden komen uit vectors.txt (vanuit python/ hugging face)
        std::vector<double> values;
        double value;

        //Probeer telkens het volgende getal uit stream te lezen en stop dat getal in value
        while(stream >> value)
        {
            values.push_back(value);
        }

        if(!values.empty())
        {
            vectors.push_back(values);
        }
    }

    return vectors;
}
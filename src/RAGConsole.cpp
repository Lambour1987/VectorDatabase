//14-10-2026: RAG: Retrieval-Augmented Generation

#include "../include/VectorDatabase.h"
#include "../include/VectorLoader.h"
#include "../include/TextLoader.h"
#include "../include/Vector.h"
//14-10-2026: Header only bibliotheek
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <vector>
//15-10-2026: Gebruiken om HTTP antwoord op te vangen
#include <sstream>


// Ontvangt HTTP-data van Ollama en verzamelt die in een string.
static size_t writeCallback(void* contents,size_t size,size_t nmemb,void* userData)
{
    const size_t totalSize = size * nmemb;
    auto* response = static_cast<std::string*>(userData);

    response->append(static_cast<char*>(contents),totalSize);

    return totalSize;
}

// Stuurt een prompt naar Ollama en geeft het antwoord terug.
std::string askOllama(const std::string& prompt)
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        throw std::runtime_error("Kon geen CURL-sessie starten.");
    }

    nlohmann::json request;
    request["model"] = "qwen2.5:3b";
    request["prompt"] = prompt;
    request["stream"] = false;

    const std::string requestBody = request.dump();
    std::string responseBody;

    curl_easy_setopt(curl,CURLOPT_URL,"http://localhost:11434/api/generate");

    curl_easy_setopt(curl,CURLOPT_POST,1L);

    curl_easy_setopt(curl,CURLOPT_POSTFIELDS,requestBody.c_str());

    curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE,static_cast<long>(requestBody.size()));

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        writeCallback
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &responseBody
    );

    curl_slist* headers = nullptr;
    headers = curl_slist_append(
        headers,
        "Content-Type: application/json"
    );

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers
    );

    const CURLcode result = curl_easy_perform(curl);

    long httpStatus = 0;

    if (result == CURLE_OK)
    {
        curl_easy_getinfo(
            curl,
            CURLINFO_RESPONSE_CODE,
            &httpStatus
        );
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        throw std::runtime_error(
            std::string("Ollama-aanroep mislukt: ") +
            curl_easy_strerror(result)
        );
    }

    if (httpStatus != 200)
    {
        throw std::runtime_error(
            "Ollama gaf HTTP-status " +
            std::to_string(httpStatus)
        );
    }

    const auto response =
        nlohmann::json::parse(responseBody);

    if (!response.contains("response"))
    {
        throw std::runtime_error(
            "Ollama-respons bevat geen antwoord."
        );
    }

    return response.at("response").get<std::string>();
}

// Ontvangt stukjes HTTP-respons van Ollama en voegt ze samen.
static size_t receiveOllamaResponse(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userData)
{
    const size_t totalSize = size * nmemb;

    auto* response = static_cast<std::string*>(userData);
    response->append(
        static_cast<char*>(contents),
        totalSize
    );

    return totalSize;
}

//14-10-2026: Extern commando
void createQueryEmbedding(const std::string& question)
{
    std::string command =
        ".\\.venv\\Scripts\\python.exe .\\embedding\\embedding.py \"" +
        question + "\"";

    int result = std::system(command.c_str());

    if (result != 0)
    {
        std::cout << "Fout: Python kon de embedding niet maken.\n";
    }
}

std::vector<double> loadQueryVector()
{
    auto vectors = loadVectorsFromFile(
        "embedding/query_vector.txt"
    );

    if (vectors.empty())
    {
        throw std::runtime_error(
            "Het query-vectorbestand bevat geen vectors."
        );
    }

    return vectors.front();
}


void loadKnowledgeBase(VectorDatabase& database)
{
    auto vectors = loadVectorsFromFile(
        "embedding/vectors.txt"
    );

    auto texts = loadTextsFromFile(
        "embedding/texts.txt"
    );

    if (vectors.size() != texts.size())
    {
        throw std::runtime_error(
            "Het aantal vectors en teksten komt niet overeen."
        );
    }

    for (std::size_t i = 0; i < vectors.size(); ++i)
    {
        Vector vector(vectors[i]);
        database.add(vector, texts[i]);
    }

    std::cout << "Kennisdatabase geladen: "
              << database.size()
              << " items.\n";
}

std::string searchKnowledgeBase(const VectorDatabase& database,const Vector& query)
{
    auto results = database.findNearestK(query, 3);

    std::cout << "\nMeest relevante kennis:\n";

    std::string context;

    for (const auto& result : results)
    {
        std::cout << "- " << result.item->text
                  << " (afstand: "
                  << result.distance
                  << ")\n";

        context += "- ";
        context += result.item->text;
        context += "\n";
    }

    return context;
}

//14-10-2026: Aparte main, want apart programma.
int main()
{
    //15-10-2026: Test voor Ollama
    // try
    // {
    //     std::string antwoord = askOllama(
    //         "Antwoord in het Nederlands: wat is 2 + 2?"
    //     );

    //     std::cout << "\nAntwoord van Ollama: "
    //               << antwoord << "\n";
    // }
    // catch (const std::exception& e)
    // {
    //     std::cerr << "Fout: " << e.what() << "\n";
    // }

    VectorDatabase database;
    loadKnowledgeBase(database);

    std::cout << "\n";
    std::string question;

    std::cout << "=== Mijn RAG-console ===\n";
    std::cout << "Typ een vraag (of 'exit' om te stoppen).\n\n";

    while (true)
    {
        std::cout << "Jij: ";

        if (!std::getline(std::cin, question))
        {
            break;
        }

        if (question == "exit")
        {
            break;
        }

        if (question.empty())
        {
            continue;
        }

        std::cout << "Je vraag is: " << question << "\n";

        createQueryEmbedding(question);

        auto queryVector = loadQueryVector();

        Vector query(queryVector);

        std::cout << "Query-vector geladen met "
          << query.dimension()
          << " dimensies.\n";

        std::cout << "\n";

        //15-10-2026: Deze regel vervangen searchKnowledgeBase(database, query); voor heel blok:
        std::string context = searchKnowledgeBase(database, query);

        std::string prompt =
            "Beantwoord de vraag uitsluitend op basis van de onderstaande context. "
            "Als de context onvoldoende informatie bevat, zeg dat dan eerlijk. "
            "Verzin geen feiten.\n\n"
            "Context:\n" + context +
            "\nVraag: " + question +
            "\nAntwoord:";

        try
        {
            std::string antwoord = askOllama(prompt);

            std::cout << "\nAntwoord van Ollama:\n"
                    << antwoord << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Fout bij het genereren van het antwoord: "
                    << e.what() << "\n";
        }
    }

    std::cout << "RAG-console afgesloten.\n";

    return 0;
}

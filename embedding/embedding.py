from sentence_transformers import SentenceTransformer
import numpy as np
from pathlib import Path
import sys



def create_embeddings(model, texts):
    return model.encode(texts, convert_to_numpy=True)


def save_vectors(path, vectors):
    np.savetxt(path, vectors)


def save_texts(path, texts):
    with open(path, "w", encoding="utf-8") as file:
        for text in texts:
            file.write(text + "\n")

def main():
    embedding_dir = Path(__file__).parent

    model_name = "sentence-transformers/all-MiniLM-L6-v2"
    model = SentenceTransformer(model_name)

    texts = [
        # Wagner / opera
        "Wagner componeerde Tristan und Isolde.",
        "Tristan und Isolde is een opera van Wagner.",
        "Richard Wagner schreef muziek voor grote Duitse opera's.",

        # Klassieke muziek
        "Johann Sebastian Bach componeerde veel werken voor orgel.",
        "Mozart schreef symfonieën, opera's en kamermuziek.",
        "Beethoven componeerde negen bekende symfonieën.",

        # Jazz
        "Miles Davis was een invloedrijke jazzmuzikant en trompettist.",
        "John Coltrane staat bekend om zijn vernieuwende jazzmuziek.",
        "Jazz maakt vaak gebruik van improvisatie en complexe ritmes.",

        # Auto's
        "Een auto gebruikt een motor om zich over de weg te verplaatsen.",
        "Elektrische auto's gebruiken een elektromotor en een batterij.",
        "Een verbrandingsmotor zet brandstof om in mechanische energie."
    ]

    #Kanweg: embeddings = model.encode(texts, convert_to_numpy=True)
    #Kanwegembeddings = create_embeddings(model, texts)

    #KAnwegprint("Aantal dimensies per vector:", len(embeddings[0]))

    #2-10-26 Kanweg: np.savetxt(embedding_dir / "vectors.txt", embeddings)
    #Kanwegvectors_path = embedding_dir / "vectors.txt"
    #2-10-26: kanweg? np.savetxt(vectors_path, embeddings)

    # Hiermee slaan we ook de originele teksten op:
    # Tekst -> model -> 12 zinnen met 384 dimensies -> vectors.txt
    # De teksten en vectoren moeten dezelfde volgorde behouden,
    # zodat tekst 1 bij vector 1 hoort, tekst 2 bij vector 2, enzovoort.
    # Kanweg: with open(embedding_dir / "texts.txt", "w", encoding="utf-8") as file:
    #Kanwegtexts_path = embedding_dir / "texts.txt"

    #Kanwegsave_vectors(vectors_path, embeddings)
    #Kanweg save_texts(texts_path, texts)

    # Nieuwe zoekopdracht
    # 2-10-26: ipv hardcoded query = "Hoe werkt een elektrische auto?" nu:


    # De knowledge vectors zijn al eerder gemaakt.
    # Tijdens een query hoeven we die niet opnieuw te berekenen.

    # Nieuwe zoekopdracht vanuit de command line
    if len(sys.argv) < 2:
        print('Gebruik: python embedding\\embedding.py "jouw vraag"')
        return

    query = sys.argv[1]

    # Maak van de query een embedding
    #Kanweg: query_embedding = model.encode([query], convert_to_numpy=True)
    query_embedding = create_embeddings(model, [query])


    print()
    print("Query:", query)
    print("Aantal query-dimensies:", len(query_embedding[0]))

    # Sla de query-vector op
    # Kanweg: np.savetxt(embedding_dir / "query_vector.txt", query_embedding)
    query_vector_path = embedding_dir / "query_vector.txt"
    
    #Kanweg: np.savetxt(query_vector_path, query_embedding)
    save_vectors(query_vector_path, query_embedding)


if __name__ == "__main__":
    main()
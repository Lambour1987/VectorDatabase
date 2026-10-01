"""#1-10-2026

from sentence_transformers import SentenceTransformer

model = SentenceTransformer("sentence-transformers/all-MiniLM-L6-v2")

text = "Wagner componeerde Tristan und Isolde."

embedding = model.encode(text)

print(embedding)
print("Aantal dimensies:", len(embedding))
 """
from sentence_transformers import SentenceTransformer
import numpy as np

model = SentenceTransformer("sentence-transformers/all-MiniLM-L6-v2")

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

embeddings = model.encode(texts)

print("Aantal dimensies:", len(embeddings[0]))


#1-10-26: Hiermee wordt de Euclidische afstand berekend
distance_1_2 = np.linalg.norm(embeddings[0] - embeddings[1])
distance_1_3 = np.linalg.norm(embeddings[0] - embeddings[2])

print()
print("Afstand Wagner ↔ Wagner:")
print(distance_1_2)

print()
print("Afstand Wagner ↔ kat:")
print(distance_1_3)

np.savetxt("embedding/vectors.txt", embeddings)

#Hiermee slaan we ook de orgineel ingevoerde text op:
#Tekst->Model (sentence transformer)-> 3 zinnen met 384 vectoren ->vectors.txt
#echter de teksten zelf de drie zinnen zijn dan niet meer gekoppeld aan de vectoren.
#Dus we moeten de teksten koppelen aan de vectoren
with open("embedding/texts.txt", "w", encoding="utf-8") as file:
    for text in texts:
        file.write(text + "\n")

# Nieuwe zoekopdracht
query = "Hoe werkt een elektrische auto?"

# Maak van de query een embedding
query_embedding = model.encode([query])

print()
print("Query:", query)
print("Query dimensies:", len(query_embedding[0]))

# Sla de query-vector op
np.savetxt("embedding/query_vector.txt", query_embedding)
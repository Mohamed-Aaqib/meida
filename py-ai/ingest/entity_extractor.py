import spacy
from .text_normalizer import normalize


nlp = spacy.load("en_core_web_md",disable=["parser", "lemmatizer", "attribute_ruler"])

# temporary, we should close it in
VALID_LABELS = {"ORG", "PRODUCT", "GPE", "PERSON", "LAW", "EVENT"}

def extract_entities(texts:list[str]):

    normalized = [normalize(t) for t in texts]
    results = []

    # generator function so it yields for us instead of putting everything in memory at once like normal for loop
    for doc in nlp.pipe(normalized,batch_size=50):
        entities = {
            ent.text
            for ent in doc.ents
            if ent.label in VALID_LABELS
        }
        results.append(entities)

    return results
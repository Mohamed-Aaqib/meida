

def classify_query(query:str) -> str:

    q = query.lower()
    if any( k in q for k in ["compare", "relationship", "cause", "effect"]):
        return "complex"
    elif any(k in q for k in ["verify", "source", "external"]):
        return "external"
    return "simple"
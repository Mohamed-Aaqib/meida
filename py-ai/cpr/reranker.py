from rapidfuzz import fuzz

def reranker(context:str, query:str):

    lines = context.splitlines()
    scored = [
        (fuzz.partial_ratio(query.lower(), l.lower()), l)
        for l in lines
    ]
    
    scored.sort(reverse=True)
    return "\n".join(l for _, l in scored[:20])
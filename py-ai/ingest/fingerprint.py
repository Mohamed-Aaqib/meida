import xxhash

def fingerprint(e1:str,rel:str,e2:str,context:str) -> str :
    combined = f"{e1}|{rel}|{e2}|{context[:128]}"
    # .encode(), converts a Python string into a bytes object.
    return xxhash.xxh64(combined.encode()).hexdigest()
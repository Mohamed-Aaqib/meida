
def build_prompt(context:str):
    return f"""
    Answer the question using ONLY the information below.
    If information is insufficient, say so explicitly.

    {context}

    Answer:
    """
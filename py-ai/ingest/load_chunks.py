from pathlib import Path
import json

def load_chunks(folder:str) -> list[dict]:

    chunks = []
    for path in Path().glob("*.json"):
        with open(path,"r",encoding="utf-8") as f:
            data = json.load(f["chunks"])
            chunks.extend(data)
    return chunks
from collections import defaultdict
from pathlib import Path
from rapidfuzz import fuzz
import orjson

class LSMRetriever:

    def __init__(self):
        self.index = defaultdict()
        self.entities = set()
        self._load()

    def _load(self):

        for path in Path("storage/lsm").glob("segment_*.json"):
            with open(path,"rb") as f:
                for r in orjson.load(f.read()):
                    for e in (r["e1"],r["e2"]):
                        el = e.lower()
                        self.index[el].append(r)
                        self.entities.add(el)


    def search(self,query,threshold=10):

        q = query.lower()
        results = []

        if q in self.index:
            results.extend((100,r) for  r in self.index[q])

        for e in self.entities:
            if fuzz.partial_ratio(query,e) >= threshold:
                results.append((80,r) for r in self.index[e])

        return results

_retriever = None
def search_lsm(query):
    global _retriever
    if _retriever is None:
        _retriever = LSMRetriever()
    return _retriever.search(query)


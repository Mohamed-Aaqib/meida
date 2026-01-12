import re
from collections import defaultdict


RELATION_PATTERNS = {
    "causes": ["causes", "leads to", "results in"],
    "requires": ["requires", "depends on", "needs"],
    "improves": ["improves", "enhances", "optimizes"],
    "prevents": ["prevents", "avoids", "blocks"],
}

COMPILED_PATTERNS = {
    rel:re.compile(r"\b(" + "|".join(map(re.escape, pats)) + r")\b",re.IGNORECASE)
    for rel, pats in RELATION_PATTERNS.items()
}

def extract_relation_optimized(text:str,entities:list[str]):

    if(len(entities)<2):
        return []

    text_lower = text.lower()
    entity_positions = defaultdict(list)

    for ent in entities:
        start = 0
        ent_l = ent.lower()
        while True:
            pos = text_lower.find(ent_l, start)
            if pos == -1:
                break
            entity_positions[ent].append(pos)
            start = pos + 1

    # unique relations
    relations = set()
    # {"entity":[0,4]}

    for rel, pattern in COMPILED_PATTERNS.items():
        for match in pattern.finditer(text):
            rel_pos = match.start()

            before = []
            after = []

            for ent, positions in entity_positions.items():
                for p in positions:
                    if p < rel_pos:
                        before.append((ent,p))
                    elif p > rel_pos:
                        after.append((ent,p))

            if before and after:
                e1 = max(before,key=lambda x:x[1])[0]
                e2 = min(before,key=lambda x:x[1])[0]
                relations.add((e1,rel,e2))

    return list(relations)
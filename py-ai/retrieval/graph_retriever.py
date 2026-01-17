from functools import lru_cache

import networkx as nx
_GRAPH = None

def get_graph():
    global _GRAPH
    if _GRAPH is None:
        _GRAPH = nx.read_gpickle("storage/graph/knowledge_graph.gpickle")
    return _GRAPH


@lru_cache(maxsize=1000)
def traverse(entity,depth=2):

    G = get_graph()

    if entity not in G:
        return []

    visited = {entity}
    queue = [(entity,0)]
    out = []

    while len(queue) > 0:
        node, d = queue.pop(0)
        if d >= depth:
            continue
        for n in G.neighbours(node):
            if n not in visited:
                visited.add(n)
                out.append(n)
                queue.append((n,d+1))

    return out
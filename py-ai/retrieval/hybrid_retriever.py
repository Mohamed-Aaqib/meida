import asyncio
from retrieval.lsm_retriever import search_lsm
from retrieval.graph_retriever import traverse

async def retrieve(query:str, session_id:str):
    lsm_results =  search_lsm(query)
    entities = [r[1]["e1"] for r in lsm_results]

    graph_tasks = [traverse(e,2) for e in entities]
    graph_results = await asyncio.gather(*graph_tasks)

    graph_context = set()
    for g in graph_results:
        graph_context.update(g)

    return {
        "entities": entities[:5],
        "graph_context": list(graph_context),
        "raw": lsm_results[:5]
    }


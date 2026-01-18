from agents.base import BaseAgent
from retrieval.graph_retriever import traverse
import asyncio


class GraphAgent(BaseAgent):

    async def run(self,query:str, context:dict) -> dict:
        entities = context.get("entities", [])
        graph_context = set()
        tasks = [traverse(e, depth=2) for e in entities[:3]]
        results = await asyncio.gather(*tasks)
        for r in results:
            graph_context.update(r)
        return {
            "agent": "GraphRAG",
            "graph_context": list(graph_context)
        }



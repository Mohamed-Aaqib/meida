from agents.base import BaseAgent
from retrieval.lsm_retriever import search_lsm


class LSMAgent(BaseAgent):

    async def run(self,query:str,context:dict) -> dict:
        results = await search_lsm(query)
        return{
            "agent": "LSM",
            "results": results[:10]
        }
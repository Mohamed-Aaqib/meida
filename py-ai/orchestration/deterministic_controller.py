

class DeternministicController:

    def __init__(self,agents:list):
        self.agents = agents

    async def run(self,query:str,context:dict):
        results = {}
        for agent in self.agents:
            res = await agent.run(query, context)
            results[res["agent"]] = res
            if res["agent"] == "LSM":
                context["entities"] = [
                    r[1]["e1"] for r in res["results"]
                ]
        return results

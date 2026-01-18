import aiohttp
from bs4 import BeautifulSoup

from agents.base import BaseAgent


class ExternalDoc(BaseAgent):

    async def run(self,query:str,context:dict) -> dict:

        urls = context.get(
            "urls",
            [f"https://en.wikipedia.org/wiki/{query.replace(' ', '_')}"]
        )

        searh_res = []
        async with aiohttp.ClientSession() as session:
            for url in urls[:2]:
                async with session.get(url,timeout=10) as resp:
                    html = await resp.text()
                    soup = BeautifulSoup(html, "html.parser")
                    text = soup.get_text(" ",strip=True)
                    searh_res.append({
                        "url": url,
                        "text": text
                    })
        return{
            "agent": "Search",
            "results": searh_res
        }
from db.db import get_async_pool
from memory.cache import cahce_get, cache_set
import aiomysql


class CDM:

    def __init__(self,session_id:str):
        self.session_id = session_id

    async def write_batch(self,entries:list[tuple]):
        pool = await get_async_pool()
        async with pool.acquire() as conn:
            async with conn.cursor() as cursor:
                await cursor.executemany(
                    """
                    INSERT INTO memory_entries
                    (session_id, section,content)
                    VALUES (%s, %s, %s)
                    """,
                    [
                        (e[0], e[1], e[2])
                        for e in entries
                    ]

                )
        # cache invalidation needed or what ?

    async def read_batch(self,section:str,limit=10):
        key = f"cdm:{self.session_id}:{section}:{limit}"
        cached = await cahce_get(key)
        if cached:
            return cached

        pool = await get_async_pool()
        async with pool.acquire() as conn:
            async with conn.cursor(aiomysql.DictCursor) as cur:
                await cur.execute(
                    """
                      SELECT memory_id, content
                      FROM memory_entries
                      WHERE session_id = %s
                      AND section =%s
                      ORDER BY effective_weight DESC, last_access DESC
                        LIMIT %s
                    """, (self.session_id, section, limit))
                rows = await cur.fetchall()

        await cache_set(key, rows)
        return rows
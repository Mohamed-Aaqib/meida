import orjson
from bloom_filter2 import BloomFilter
import aioredis

_redis = None
_bloom = BloomFilter(max_elements=10_000_000, error_rate=0.001)


async def get_redis():
    global _redis
    if _redis is None:
        _redis = await aioredis.from_url(
            # initialize with upstash
            "redis://127.0.0.1:6379/0",
            econding=None,
            # we store bytes so we get back bytes, no strings
            decode_responses=False,
            max_connections=50
        )
    return _redis


async def cache_set(key:str,value,ttl=300):
    r = await get_redis()
    await r.setex(key,ttl,orjson.dumps(value))
    _bloom.add(key)

async def cahce_get(key:str):
    if key not in _bloom:
        return None
    r = await get_redis()
    val = await r.get(key)
    return orjson.loads(val) if val else None

async def cache_invalidate_session(session_id: str):
    r = await get_redis()
    async for key in r.scan_iter(match=f"cdm:{session_id}:*"):
        await r.delete(key)
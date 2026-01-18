import aiomysql
from mysql.connector import pooling


_async_pool = None

# initialize the pools correctly with the correct credentials 
async def get_async_pool():
    global _async_pool
    if _async_pool is None:
        _async_pool = await aiomysql.create_pool(
            host="localhost",
            user="root",
            password="root",
            db="cdm_memory",
            minsize=10,
            maxsize=50,
            autocommit=True
        )
    return _async_pool


_sync_pool = pooling.MySQLConnectionPool(
    pool_name="cdm_sync",
    pool_size=10,
    host="localhost",
    user="root",
    password="root",
    database="cdm_memory",
    autocommit=True,
)


def get_sync_conn():
    return _sync_pool.get_connection()
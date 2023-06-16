import time

def check_token(conn, token):
    # 检查令牌是否存在并返回对应的的用户
    return conn.hget('login:', token)

def update_token(conn, token, user, item=None):
    # 获取当前时间戳
    timestamp = time.time()

    # 维持令牌与已登录用户之间的映射
    conn.hset('login:', token, user)
    # 记录令牌最后一次出现的时间
    conn.zadd('recent:', token, timestamp)
    # 如果用户浏览了商品，那么就将这个商品添加到记录最近浏览商品的有序集合里面
    if item:
        # 记录用户浏览过的商品
        conn.zadd('viewed:' + token, item, timestamp)
        # 移除旧的记录，只保留用户最近浏览过的25个商品
        conn.zremrangebyrank('viewed:' + token, 0, -26)

QUIT = False
LIMIT = 10000000

def clean_sessions(conn):
    while not QUIT:
        size = conn.zcard('recent:')
        if size <= LIMIT:
            time.sleep(1)
            continue
        
        # 最多移除100个令牌
        end_index = min(size - LIMIT, 100)
        tokens = conn.zrange('recent:', 0, end_index-1)

        # 为那些将要被删除的令牌构建键名
        session_keys = []
        for token in tokens:
            session_keys.append('viewed:' + token)

        # 移除最旧的那些令牌
        conn.delete(*session_keys)
        conn.hdel('login:', *tokens)
        conn.zrem('recent:', *tokens)

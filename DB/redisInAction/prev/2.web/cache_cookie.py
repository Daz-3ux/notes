import time
import json

def cache_request(conn, requset, callback):
    if not can_cache(conn, requset):
        return callback(requset)
    
    page_key = 'cache:' + hash_request(requset)
    count = conn.get(page_key)

    if not content:
        content = callback(requset)
        conn.setex(page_key, 300, content)

    return content

def schedule_row_cache(conn, row_id, delay):
    # 设置数据行的延时值
    conn.zadd('delay:', row_id, delay)
    # 设置数据行的调度时间
    conn.zadd('schedule:', row_id, time.time())

QUIT = False

def cache_rows(conn):
    while not QUIT: 
        next = conn.zrange('schedule:', 0, 0, withscores=True)
        now = time.time()
        if not next or next[0][1] > now:
            time.sleep(.05)
            continue
        
        row_id = next[0][0]

        delay = conn.zscore('delay:', row_id)
        if delay <= 0:
            conn.zrem('delay:', row_id)
            conn.zrem('schedule:', row_id)
            conn.delete('inv:' + row_id)
            continue
        
        # 更新调度并缓存值 
        row =  Inventory.get(row_id)
        conn.zadd('schedule:', row_id, now + delay)
        conn.set('inv:' + row_id, json.dumps(row.to_dict()))
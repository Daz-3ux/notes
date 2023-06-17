import time

ONE_WEEK_IN_SECONDS = 7 * 86400
VOTE_SCORE = 432

def article_vote(conn, user, article):
    # 计算投票截止时间
    cutoff = time.time() - ONE_WEEK_IN_SECONDS
    if conn.zscore('time:', article) < cutoff:
        return
    
    # 从article:id标识符里面取出文章id
    article_id = article.partition(':')[-1]
    # 如果用户是第一次为这篇文章投票
    if conn.sadd('voted:' + article_id, user):
        conn.zincrby('score:', article, VOTE_SCORE)
        conn.hincrby(article, 'votes', 1)

def post_article(conn, user, title, link):
    # 生成文章 ID
    article_id = str(conn.incr('article:'))

    # 将发布文章的用户添加到文章的已投票用户名单里面，并将文章的投票截止时间设置为一周
    voted = 'voted:' + article_id
    conn.sadd(voted, user)
    conn.expire(voted, ONE_WEEK_IN_SECONDS)

    # 整理文章信息数据，将文章信息存储到一个散列里面
    now = time.time()
    article = 'article:' + article_id
    conn.hmset(article, {
        'title': title,
        'link': link,
        'poster': user,
        'time': now,
        'votes': 1,
    })
    conn.zadd('score:', article, now + VOTE_SCORE)
    conn.zadd('time:', article, now)

    return article_id

ARTICLES_PER_PAGE = 25

def get_articles(conn, page, order='scores:'):
    # 设置文章的起始索引以及结束索引
    start = (page - 1) * ARTICLES_PER_PAGE
    end = start + ARTICLES_PER_PAGE - 1

    # 获取多个文章ID
    ids = conn.zrevrange(order, start, end)
    articles = []
    # 根据文章 ID 获取文章详细信息
    for id in ids:
        article_data = conn.hgetall(id)
        article_data['id'] = id
        articles.append(article_data)

    return articles

def add_remove_groups(conn, article_id, to_add=[], to_remove=[]):
    # 构建存储文章信息的键名
    article = 'article:' + article_id
    # 将文章添加到它所属的群组里面
    for group in to_add:
        conn.sadd('group:' + group, article)
    # 从群组里面移除文章
    for group in to_remove:
        conn.srem('group:' + group, article)

def get_group_articles(conn, group, page, order='score;'):
    # 为每个群组的每种排列顺序都创建一个键
    key = order + group
    # 检查是否有已缓存的排序结果，如果没有的话就现在进行排序
    if not conn.exists(key):
        # 根据评分或者发布时间，对群组文章进行排序
        conn.zinterstore(key,
            ['group:' + group, order],
            aggregate='max',
        )
        # 让Redis在60秒钟之后自动删除这个有序集合
        conn.expire(key, 60)
    # 调用之前定义的get_articles()函数来进行分页并获取文章数据
    return get_articles(conn, page, key)
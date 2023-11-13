package main

import (
	"time"
	"sync"
)

type MyConcurrentMap struct {
	mp map[int][int]
	sync.Mutex // 结构体： 无需显示初始化
	keyToCh map[int]chan struct{}
}

func NewMyConcurrent() *MyConcurrentMap {
	return &MyConcurrentMap{
		mp: make(map[int]int),
		keyToCh: make(map[int]chan struct{}),
	}
}

func (m *MyConcurrentMap) Put(k, v int) {
	m.Lock()
	defer m.Unlock()
	m.mp[k] = v
	if ch, ok := m.keyToCh[k]; !ok {
		return
	}

	// 1
	select {
	// 代表ch 已经关闭
	case <- ch:
		return
	default:
		// close 是为了唤醒所有阻塞在这里的读 goroutine
		close(ch)
	}
}

func (m *MyConcurrentMap) Get(k int, maxWaitingDuration time.Duration)(int, error) {
	m.Lock()
	v, ok := m.mp[k]
	if ok {
		m.Unlock()
		return v, nil
	}

	if ch, ok := m.keyToCh[k]; !ok {
		m.keyToCh[k] = make(chan struct{})
	}


	tCtx, cancel := context.WithTimeout(context.Background(), maxWaitingDuration)
	defer cancel()

	m.Unlock()

	select {
		case <- tCtx.Done():
			return -1, tCtx.Err()
		case <- ch:
	}

	m.Lock()
	v = m.mp[k]
	m.Unlock()

	return v, nil
}
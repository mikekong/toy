#include <iostream>

#include "queue/comm/ring.h"

using namespace std;

//  以下方法都可以用最多两次memcpy来优化
//  现在就是偷懒=.=
s64 RingQueue::write_buf(u8 * in_buf, int sz) {
    if (left() < sz) return -1;

    for (int i = 0; i < sz; ++i) {
        this->buf[(write +i) % buf_len] = in_buf[i];
    }

    s64 ret = write;

    write += sz;

    return ret;
}

int RingQueue::read_buf(u8 * out_buf, int & in_out_sz) {
    if (reader < read) {
        return -1;
    }

    int max_sz = in_out_sz;
    s64 end_reader = reader + max_sz;
    if (end_reader > write) end_reader = write;

    in_out_sz = end_reader - reader;
    if (in_out_sz <= 0) return 0;

    for (s64 i = reader; i < end_reader; ++i) {
        out_buf[i - reader] = this->buf[i % buf_len];
    }

    if (reader < read) {
        return -1;
    }

    reader = end_reader;

    return 0;
}



#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef long s64;
typedef unsigned char u8;

//  先暂时用struct吧=。= 方便点
struct RingQueue {
    //  仅仅由producer修改
    //  Producer保证read->write之间的数据总是有效的
    //      write, read都由Producer修改
    s64 write;
    s64 read;

    //  每个reader记录自己读指针[可以扩展到多个reader]
    //      每次读的时候，先读buf, 然后比较reader数值是否在有效区间内
    //          有效: 读取成功
    //          无效: 丢弃读到的buf, reader = read 
    s64 reader;

    //  边界情况: write超过2^64的时候，考虑每秒产生1G数据[2^30], 可以支持2^34秒，
    //            远大于机器寿命,可以忽略
    s64 buf_len;
    u8  buf[0];

    s64 size() { return (write >= read) ? (write - read) : (write + buf_len - read);}
    s64 left() { return (buf_len - size()); }

    int read_buf(u8 * out_buf, int & in_out_sz);
    s64 write_buf(u8 * buf, int sz);
    void reset_reader() { reader = read; }
};


/*
 Case讨论与实现:
 write修改: 
    当且仅当,Producer已经把消息buff复制到shm中时增加，只有Producer读和修改.[安全]
 read修改:
    Consumer读, Producer写;
    当且仅当shm中的消息已经超时或者已经无效时增加;
    Producer:
        如果消息超时主动增加，否则比较并且确定是否所有reader都已经读取过该消息
        注:直接增加即可,无需修改sync_fetch_and_add;
    Consumer:
        读取shm中的buff数据时先判断自身的reader是否有效,
            无效: 
                将reader=read，并且开始下一个读取;
            有效:
                读取shm的buff数据,读完以后再次判断reader是否有效，无效同上面，即: 将reader=read，并且开始下一个读取;


 reader修改:
    Consumer直接修改即可;

*/


#endif


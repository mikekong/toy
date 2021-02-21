
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <list>
#include <map>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <unistd.h>
#include "queue/comm/ring.h"
#include "queue/comm/message.h"

using namespace std;

static const int MAX_BUF_SZ     = 1024 * 1 ;
static const int EXPIRE_SEC     = 5;
static const int MSG_PER_TICK   = 1;
static const int SHM_KEY        = 66699;
static const int SHM_MODE       = 0600;

//  用于记录放进消息队列中的消息的Header
//  用于超时淘汰
map<s64, Header> id2header;

//  将列表中缓存的消息写入消息队列
void write_tick(RingQueue * ring, list<Msg *> & to_writes);

//  超时和已读处理
void read_tick(RingQueue * ring);

//  每隔一段时间生产一些消息[长度和内容随机]
void producer_tick(list<Msg *> & to_writes);

//int main(int argc, char ** argv) {
int main() {
    int shmid = shmget(SHM_KEY
                    , MAX_BUF_SZ + sizeof(RingQueue)
                    , SHM_MODE | IPC_CREAT);
    void * ptr = shmat(shmid, NULL, 0);
    assert(ptr);

    list<Msg *> to_writes;
    RingQueue * ring = (RingQueue *)ptr;
    ring->write     = 0;
    ring->read      = 0;
    ring->reader    = 0;
    ring->buf_len   = MAX_BUF_SZ;

    while (1) {
        read_tick(ring);
        producer_tick(to_writes);
        write_tick(ring, to_writes);
        sleep(1);
    }

    return 0;
}

//  将列表中缓存的消息写入消息队列
void write_tick(RingQueue * ring, list<Msg *> & to_writes) {
    int now_ts = time(NULL);
    while (1) {
        if (to_writes.size() <= 0) break;
        Msg * msg = to_writes.front();
        to_writes.pop_front();

        if ((now_ts - msg->header.timestamp) >= EXPIRE_SEC) {
            free(msg);
        } else {
            int total_sz = msg->header.size + sizeof(Header);
            int left_sz = ring->left();
            if (left_sz < total_sz) {
                //  the free space of queue is not enough
                to_writes.push_front(msg);
                printf("Producer: Queue is full[%lu,%lu], unable to write message ts[%d] to queue\n"
                    , ring->read, ring->write, msg->header.timestamp);
                break;
            } else {
                //  pack the message & write to the queue
                static u8 OUT_BUF[MAX_BUF_SZ];
                Msg2Buf(msg, OUT_BUF, MAX_BUF_SZ);
                Header header = msg->header;
                id2header[ring->write]  =   header;
                ring->write_buf(OUT_BUF, total_sz);
                printf("Producer: Write message ts[%d] to queue[%lu,%lu]\n"
                    , msg->header.timestamp, ring->read, ring->write);
                free(msg);
            }
        }
    }
}

//  每隔一段时间生产一些消息[长度和内容随机]
void producer_tick(list<Msg *> & to_writes) {
    for (int i = 0; i < MSG_PER_TICK; ++i) {
        int body_sz = rand() % 512 + 1;
        Msg * msg = (Msg *)malloc(sizeof(Header) + body_sz);
        msg->header.size = body_sz;
        msg->header.timestamp = time(NULL);
        for (int i = 0; i < body_sz; ++i) {
            msg->body[i] = rand() % 256;
        }
        msg->header.checksum = calc_checksum((u8 *)msg->body, body_sz);
        printf("Producer: Produce one message ts[%d]\n", msg->header.timestamp);
        to_writes.push_back(msg);
    }
}

//  超时和已读处理
void read_tick(RingQueue * ring){
    int now_ts = time(NULL);

    while (1) {
        if (ring->read >= ring->write) {
            break;
        }

        map<s64, Header>::iterator it = id2header.find(ring->read); 
        if (it == id2header.end()) {
            assert(NULL);
        }

        int msg_sz = it->second.size;
        int msg_ts = it->second.timestamp;
        int total_sz = msg_sz + sizeof(Header);
        if ((now_ts - msg_ts) >= EXPIRE_SEC) {
            id2header.erase(ring->read);
            //  最早的消息已经超时
            ring->read += total_sz;
            printf("Producer: Expire one message ts[%d]\n", msg_ts);
        } else {
            //  消息已经被消费
            if (ring->reader >= (ring->read + total_sz)) {
                id2header.erase(ring->read);
                printf("Producer: Delete one message ts[%d] as read\n", msg_ts);
                ring->read += total_sz;
            } else {
                break;
            }
        }
    }
}


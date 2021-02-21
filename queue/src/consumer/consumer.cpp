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

static const int SHM_KEY        = 66699;
static const int SHM_MODE       = 0600;
static const int MAX_BUF_SZ     = 102400;

//  消费消息
void consumer_tick(RingQueue * ring, u8 * buf, int & buf_len, int max_buf_len);

//  逐个处理消息
void proc_message(u8 * buf, int & buf_len);

//int main(int argc, char ** argv) {
int main() {
    int shmid = shmget(SHM_KEY
                    , 0, SHM_MODE);
    void * ptr = shmat(shmid, NULL, 0);
    assert(ptr);

    RingQueue * ring = (RingQueue *)ptr;
    static u8 buf[MAX_BUF_SZ];
    static int buf_len = 0;

    while (1) {
        consumer_tick(ring, buf, buf_len
                    , MAX_BUF_SZ);
        proc_message(buf, buf_len);
        sleep(10);
    }

    return 0;
}

void consumer_tick(RingQueue * ring, u8 * buf
                , int & buf_len
                , int max_buf_len) 
{
    while (1) {
        int left_buf_sz = max_buf_len - buf_len;
        int ret = ring->read_buf(buf + buf_len, left_buf_sz);   
        if (ret < 0) {
            //  当自身的reader已经失效将重新消费
            printf("Reader is expire[%lu] Compare[%lu,%lu]... reset now\n"
                , ring->reader, ring->read, ring->write);
            ring->reset_reader();
            buf_len = 0;
        } else {
            buf_len += left_buf_sz;
            printf("Consumer read: [%d] data Total[%d]\n", left_buf_sz, buf_len);
            break;
        }
    }
}

void proc_message(u8 * buf, int & buf_len) {
    int curr    =   0;
    while (1) {
        if ((buf_len - curr) < (int)sizeof(Header)) break;

        Header h;
        memcpy(&h, buf + curr, sizeof(h));
        int total_sz = h.size + sizeof(Header);
        if ((buf_len - curr) < total_sz) break;

        Msg * msg = (Msg *)malloc(total_sz);
        memcpy(msg, buf + curr, total_sz);

        int new_checksum = calc_checksum((u8 *)msg->body, h.size);

        if (new_checksum == msg->header.checksum) {
            curr += total_sz;
            printf("Consumer proc message, ts[%d] size[%d] checksum[%d]\n"
                , msg->header.timestamp, msg->header.size, msg->header.checksum);
            free(msg);
        } else {
            printf("Discard all message as checksum is invalid\n");
            buf_len = 0;
            free(msg);
            return ;
        }
    }

    if (curr == buf_len) {
        //  所有数据都已经处理完成
        buf_len = 0;
    } else {
        //  没有处理任何信息
        if (curr <= 0) return ;

        assert(buf_len >= curr);

        buf_len -= curr;

        //  注意overlap
        for (int i = 0; i < buf_len; ++i) {
            buf[i] = buf[i + curr];
        }

        printf("Left [%d] bytes\n", buf_len);

    }
}


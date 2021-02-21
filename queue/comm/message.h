
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <cstring>

struct Header {
    int size;       //  size of the body
    int timestamp;  //  timestamp
    int checksum;
};

struct Msg {
    Header header;
    char body[0];
};

inline void Msg2Buf(const Msg * msg, unsigned char * buf, int buf_sz) {
    int total_sz = msg->header.size + sizeof(Header);
    if (total_sz > buf_sz) return ;

    memcpy(buf, msg, msg->header.size + sizeof(Header));
}

void Buf2Msg(const unsigned char * buf, Msg ** msg) {
    Header h;
    memcpy(&h, buf, sizeof(h));

    //  TODO: 对Header的相应参数进行检查

    Msg * nmsg = (Msg *)malloc(sizeof(Header) + h.size);
    memcpy(nmsg, buf, h.size + sizeof(Header));

    *msg = nmsg;
}

int calc_checksum(u8 * body, int size) {
    int ret = 0;
    for (int i = 0; i < size; ++i) {
        ret ^= body[i];
    }
    return ret;
}

#endif


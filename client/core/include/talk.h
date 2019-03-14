#ifndef TALK_H_
#define TALK_H_

#include <vector>

#include "neu_talk_protocol.h"

enum TalkType {
    TALK_TYPE_PRIATE,
    TALK_TYPE_GROUP
};

struct Talk {
    TalkType type;
    bool isReceviewNewMsg;
    unsigned long sender_id;
    unsigned long recv_id;
    std::vector<DataMsg> msgs;
};

#endif // TALK_H_

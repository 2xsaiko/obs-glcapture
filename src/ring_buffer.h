#pragma once

#include <cstdint>
#include "frame_data.h"

class RingBuffer {
private:
    uint32_t size;
    FrameData** data;
    uint32_t head;
    uint32_t tail;

public:
    void push(FrameData* ptr);

    FrameData* pop();

    uint32_t length();

    bool empty();

    bool full();

public:
    explicit RingBuffer(uint32_t size);

    ~RingBuffer();
};
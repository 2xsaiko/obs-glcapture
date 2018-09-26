#include <cstdio>
#include "ring_buffer.h"

RingBuffer::RingBuffer(uint32_t size) {
    this->size = size;
    head = tail = 0;
    data = new FrameData* [size];
}

RingBuffer::~RingBuffer() {
    FrameData* data;
    while (data = pop()) {
        delete data;
    }
}

void RingBuffer::push(FrameData* ptr) {
    if (full()) {
        // printf("Buffer overflow\n");
        delete ptr;
        return;
    };
    data[head] = ptr;
    head = (head + 1) % size;
}

FrameData* RingBuffer::pop() {
    if (empty()) return nullptr;
    FrameData* pData = data[tail];
    tail = (tail + 1) % size;
    return pData;
}

uint32_t RingBuffer::length() {
    int32_t i = head - tail;
    if (i < 0) return i + size;
    else return i;
}

bool RingBuffer::empty() {
    return head == tail;
}


bool RingBuffer::full() {
    return length() == size - 1;
}

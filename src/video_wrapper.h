#pragma once

#include "ssr/GLInjectInput.h"
#include "obs_sink.h"

class VideoWrapper {
public:
    GLInjectInput* source;
    OBSSink* sink;

public:
    uint32_t get_width();

    uint32_t get_height();

    void get_size(uint32_t* width, uint32_t* height);

    void set_capturing(bool capture);

public:
    explicit VideoWrapper();

    ~VideoWrapper();
};
#pragma once

#include "ssr/GLInjectInput.h"
#include "obs_sink.h"

class VideoWrapper {
private:
    const char* channel;
    bool relax_permissions;
    bool record_cursor;
    bool limit_fps;
    uint32_t fps;

    bool changed;

    GLInjectInput* source;
    OBSSink* sink;
    bool capturing;

public:
    FrameData* get_next_frame();

    uint32_t get_width();

    uint32_t get_height();

    void get_size(uint32_t* width, uint32_t* height);

    void set_capturing(bool capture);

    bool is_capturing();

    void set_channel(const char* channel);

    void set_relax_permissions(bool relax_permissions);

    void set_record_cursor(bool record_cursor);

    void set_limit_fps(bool limit_fps);

    void set_fps(uint32_t fps);

    void reset(bool force);

private:
    void deinit();
    void init();

public:
    explicit VideoWrapper();

    ~VideoWrapper();
};
#include "video_wrapper.h"
#include "glsource.h"

#define STREAM_FPS 60

VideoWrapper::VideoWrapper() {
    sink = new OBSSink(STREAM_FPS);
    source = new GLInjectInput(nullptr, false, true, false, STREAM_FPS);

    sink->ConnectVideoSource(source);
    printf("connected sinks\n");
}

VideoWrapper::~VideoWrapper() {
    sink->ConnectVideoSource(nullptr);

    delete sink;
    delete source;
}

uint32_t VideoWrapper::get_width() {
    uint32_t width = 0;
    uint32_t height = 0;
    get_size(&width, &height);
    return width;
}

uint32_t VideoWrapper::get_height() {
    uint32_t width = 0;
    uint32_t height = 0;
    get_size(&width, &height);
    return height;
};

void VideoWrapper::get_size(uint32_t* width, uint32_t* height) {
    source->GetCurrentSize(width, height);
}

void VideoWrapper::set_capturing(bool capture) {
    source->SetCapturing(capture);
}
#pragma once

#include "common.h"
#include "ssr/SourceSink.h"
#include "frame_data.h"
#include "ring_buffer.h"

#include <queue>

class OBSSink : public VideoSink {

private:
    int64_t next_frame;
    uint32_t fps;
    RingBuffer* buffer;

public:
    // Returns the preferred next video timestamp.
    // This function is thread-safe.
    int64_t GetNextVideoTimestamp() override;

    // Reads a video frame from the video source.
    // This function is thread-safe.
    void ReadVideoFrame(unsigned int width, unsigned int height, const uint8_t* data, int stride, AVPixelFormat format, int64_t timestamp) override;

    FrameData* get_next_frame();

private:
    void push_frame(FrameData* data);

public:
    explicit OBSSink(uint32_t fps);

    ~OBSSink() override;
};
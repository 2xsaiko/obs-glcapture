#include "video_wrapper.h"
#include "glsource.h"

#define CHANNEL nullptr
#define RELAX_PERMISSIONS false
#define RECORD_CURSOR true
#define LIMIT_FPS false
#define STREAM_FPS 60

VideoWrapper::VideoWrapper() {
    channel = CHANNEL;
    relax_permissions = RELAX_PERMISSIONS;
    record_cursor = RECORD_CURSOR;
    limit_fps = LIMIT_FPS;
    fps = STREAM_FPS;

    sink = nullptr;
    source = nullptr;
    capturing = false;

    reset(true);
}

VideoWrapper::~VideoWrapper() {
    deinit();
}

FrameData* VideoWrapper::get_next_frame() {
    return sink->get_next_frame();
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
    capturing = capture;
}

bool VideoWrapper::is_capturing() {
    return capturing;
}

void VideoWrapper::set_channel(const char* channel) {
    if (channel && strlen(channel) == 0) {
        set_channel(nullptr);
    } else if (channel != this->channel) {
        this->channel = channel;
        changed = true;
    }
}

void VideoWrapper::set_relax_permissions(bool relax_permissions) {
    if (this->relax_permissions != relax_permissions) {
        this->relax_permissions = relax_permissions;
        changed = true;
    }
}

void VideoWrapper::set_record_cursor(bool record_cursor) {
    if (this->record_cursor != record_cursor) {
        this->record_cursor = record_cursor;
        changed = true;
    }
}

void VideoWrapper::set_limit_fps(bool limit_fps) {
    if (this->limit_fps != limit_fps) {
        this->limit_fps = limit_fps;
        changed = true;
    }
}

void VideoWrapper::set_fps(uint32_t fps) {
    if (this->fps != fps) {
        this->fps = fps;
        changed = true;
    }
}

void VideoWrapper::reset(bool force) {
    if (changed || force) {
        deinit();
        init();
    }
    changed = false;
}

void VideoWrapper::deinit() {
    blog(LOG_DEBUG, "VideoWrapper::deinit()");

    if (sink) {
        sink->ConnectVideoSource(nullptr);
    }

    if (source && capturing) {
        source->SetCapturing(false);
    }

    delete sink;
    delete source;
}

void VideoWrapper::init() {
    blog(LOG_DEBUG, "VideoWrapper::init()");

    sink = new OBSSink(fps);
    source = new GLInjectInput(channel, relax_permissions, record_cursor, limit_fps, fps);

    sink->ConnectVideoSource(source);

    if (capturing) {
        source->SetCapturing(true);
    }
}

#pragma once

#include <cstdint>

class FrameData {
    friend class OBSSink;

private:
    uint32_t width;
    uint32_t height;
    uint8_t* color_data;

public:
    uint32_t get_width();

    uint32_t get_height();

    uint8_t* get_color_data();

protected:
    FrameData(uint32_t width, uint32_t height, uint8_t* color_data);

public:
    ~FrameData();
};
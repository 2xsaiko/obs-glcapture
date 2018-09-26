#include <cstdio>
#include "frame_data.h"

FrameData::FrameData(uint32_t width, uint32_t height, uint8_t* color_data) {
    this->width = width;
    this->height = height;
    this->color_data = color_data;
}

FrameData::~FrameData() {
    delete[] color_data;
}

uint32_t FrameData::get_width() {
    return width;
}

uint32_t FrameData::get_height() {
    return height;
}

uint8_t* FrameData::get_color_data() {
    return color_data;
}

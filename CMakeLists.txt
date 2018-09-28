cmake_minimum_required(VERSION 3.12)
project(obs-glcapture CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED Core)

add_library(obs-glcapture MODULE
        src/glcaptureplugin.cpp
        src/glsource.cpp src/glsource.h
        src/ssr/AVWrapper.cpp src/ssr/AVWrapper.h
        src/ssr/GLInjectInput.cpp src/ssr/GLInjectInput.h
        src/ssr/Global.h
        src/ssr/MutexDataPair.h
        src/ssr/ShmStructs.h
        src/ssr/SourceSink.cpp src/ssr/SourceSink.h
        src/ssr/SSRVideoStream.h
        src/ssr/SSRVideoStreamReader.cpp src/ssr/SSRVideoStreamReader.h
        src/ssr/SSRVideoStreamWatcher.cpp src/ssr/SSRVideoStreamWatcher.h
        src/obs_sink.cpp src/obs_sink.h
        src/video_wrapper.cpp src/video_wrapper.h
        src/frame_data.cpp src/frame_data.h
        src/ring_buffer.cpp src/ring_buffer.h
        src/common.h)

target_link_libraries(obs-glcapture obs Qt5::Core)
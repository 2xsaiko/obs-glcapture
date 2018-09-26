cmake_minimum_required(VERSION 3.12)
project(obs-glcapture CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED Core)

add_library(obs-glcapture MODULE
        glcaptureplugin.cpp
        glsource.cpp glsource.h
        ssr/AVWrapper.cpp ssr/AVWrapper.h
        ssr/GLInjectInput.cpp ssr/GLInjectInput.h
        ssr/Global.h
        ssr/Logger.cpp ssr/Logger.h
        ssr/MutexDataPair.h
        ssr/ShmStructs.h
        ssr/SourceSink.cpp ssr/SourceSink.h
        ssr/SSRVideoStream.h
        ssr/SSRVideoStreamReader.cpp ssr/SSRVideoStreamReader.h
        ssr/SSRVideoStreamWatcher.cpp ssr/SSRVideoStreamWatcher.h
        obs_sink.cpp obs_sink.h
        video_wrapper.cpp video_wrapper.h
        frame_data.cpp frame_data.h
        ring_buffer.cpp ring_buffer.h)

target_link_libraries(obs-glcapture obs Qt5::Core)
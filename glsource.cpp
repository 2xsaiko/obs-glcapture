#include "glsource.h"

#include <obs/obs-module.h>
#include <pthread.h>

struct obs_source_info glsource = {
        .id = "gl-source",
        .type = OBS_SOURCE_TYPE_INPUT,
        .output_flags = OBS_SOURCE_VIDEO,
        .get_name = gl_get_name,
        .create = gl_create,
        .destroy = gl_destroy,
        .get_width = gl_get_width,
        .get_height = gl_get_height,
        .update = gl_update,
        .show = gl_show,
        .hide = gl_hide,
        .video_render = gl_render,
};

static const char* gl_get_name(void* unused) {
    UNUSED_PARAMETER(unused);
    return obs_module_text("GL Source");
}

static void* gl_create(obs_data_t* settings, obs_source_t* source) {
    auto* data = new gl_data;
    data->source = source;
    data->vid = new VideoWrapper();
    data->active_texture = nullptr;

    gl_update(data, settings);

    return data;
}

static void gl_destroy(void* data_in) {
    auto* data = (gl_data*) data_in;
    if (!data)
        return;

    if(data->active_texture) {
        obs_enter_graphics();
        gs_texture_destroy(data->active_texture);
        obs_leave_graphics();
    }

    delete data->vid;
    delete data;
}

static void gl_update(void* data_in, obs_data_t* settings) {
    // no settings
}

static uint32_t gl_get_width(void* data_in) {
    auto* data = (gl_data*) data_in;
    return data->vid->get_width();
}

static uint32_t gl_get_height(void* data_in) {
    auto* data = (gl_data*) data_in;
    return data->vid->get_height();
}

static void gl_render(void* data_in, gs_effect_t* effect) {
    auto* data = (gl_data*) data_in;
    OBSSink* sink = data->vid->sink;
    FrameData* frame = sink->get_next_frame();

    if (frame) {
        if (data->active_texture) {

            uint32_t tex_width = gs_texture_get_width(data->active_texture);
            uint32_t tex_height = gs_texture_get_height(data->active_texture);
            if (tex_width != frame->get_width() || tex_height != frame->get_height()) {
                printf("%s\n", "destroying texture");
                gs_texture_destroy(data->active_texture);
                data->active_texture = nullptr;
            }
        }

        if (!data->active_texture) {
            // format will always be AV_PIX_FMT_BGRA (see GLInjectInput.cpp)
            gs_texture_t* pTexture = gs_texture_create(frame->get_width(), frame->get_height(), gs_color_format::GS_BGRA, 1, nullptr, GS_DYNAMIC);
            printf("creating texture: gs_texture_create(%d, %d, %d, %d, %p, %d) = %p\n", frame->get_width(), frame->get_height(), gs_color_format::GS_BGRA, 1, nullptr, GS_DYNAMIC, pTexture);
            data->active_texture = pTexture;
        }

        gs_texture_set_image(data->active_texture, frame->get_color_data(), frame->get_width() * 4, false);

    }
    delete frame;

    if (data->active_texture) {
        unsigned int width = 0;
        unsigned int height = 0;
        data->vid->get_size(&width, &height);

        gs_reset_blend_state();
        gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"), data->active_texture);
        gs_draw_sprite(data->active_texture, 0, width, height);
    }

}

static void gl_show(void* data_in) {
    auto* data = (gl_data*) data_in;

    data->vid->set_capturing(true);
}

static void gl_hide(void* data_in) {
    auto* data = (gl_data*) data_in;

    data->vid->set_capturing(false);
}
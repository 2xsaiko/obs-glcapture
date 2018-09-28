#include "glsource.h"

#include "common.h"
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
        .get_defaults = gl_get_defaults,
        .get_properties = gl_get_properties,
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

    if (data->active_texture) {
        obs_enter_graphics();
        gs_texture_destroy(data->active_texture);
        obs_leave_graphics();
    }

    delete data->vid;
    delete data;
}

static void gl_update(void* data_in, obs_data_t* settings) {
    auto* data = (gl_data*) data_in;

    data->channel = obs_data_get_string(settings, "channel");
    data->relax_permissions = obs_data_get_bool(settings, "relax_permissions");
    data->command = obs_data_get_string(settings, "command");

    data->vid->set_channel(obs_data_get_string(settings, "channel"));
    data->vid->set_relax_permissions(data->relax_permissions);
    data->vid->set_record_cursor(obs_data_get_bool(settings, "record_cursor"));
    data->vid->set_limit_fps(obs_data_get_bool(settings, "limit_fps"));
    data->vid->set_fps(static_cast<uint32_t>(obs_data_get_int(settings, "fps")));
    data->vid->reset(false);
}

static uint32_t gl_get_width(void* data_in) {
    auto* data = (gl_data*) data_in;
    return data->vid->get_width();
}

static uint32_t gl_get_height(void* data_in) {
    auto* data = (gl_data*) data_in;
    return data->vid->get_height();
}

static obs_properties_t* gl_get_properties(void* data_in) {
    UNUSED_PARAMETER(data_in);

    obs_properties_t* props = obs_properties_create();

    obs_properties_add_text(props, "channel", obs_module_text("Channel"), OBS_TEXT_DEFAULT);
    obs_properties_add_bool(props, "relax_permissions", obs_module_text("RelaxPermissions"));
    obs_properties_add_bool(props, "record_cursor", obs_module_text("RecordCursor"));
    obs_properties_add_bool(props, "limit_fps", obs_module_text("LimitFPS"));
    obs_properties_add_int(props, "fps", obs_module_text("FPS"), 1, 200, 1);

    // TODO: start program button added this way doesn't do jack shit, need to debug
    // obs_properties_add_text(props, "command_line", obs_module_text("CommandLine"), OBS_TEXT_DEFAULT);
    // obs_properties_add_button(props, "start_program", obs_module_text("StartProgram"), start_program_button_clicked);

    return props;
}

static void gl_get_defaults(obs_data_t* data) {
    obs_data_set_default_string(data, "channel", "");
    obs_data_set_default_bool(data, "relax_permissions", false);
    obs_data_set_default_bool(data, "record_cursor", true);
    obs_data_set_default_bool(data, "limit_fps", false);
    obs_data_set_default_int(data, "fps", 60);

    obs_data_set_default_string(data, "command_line", "glxgears");
}

static void gl_render(void* data_in, gs_effect_t* effect) {
    auto* data = (gl_data*) data_in;
    FrameData* frame = data->vid->get_next_frame();

    if (frame) {
        uint32_t width = frame->get_width();
        uint32_t height = frame->get_height();

        if (data->active_texture) {
            uint32_t tex_width = gs_texture_get_width(data->active_texture);
            uint32_t tex_height = gs_texture_get_height(data->active_texture);
            if (tex_width != width || tex_height != height) {
                blog(LOG_DEBUG, "Window size changed from %dx%d to %dx%d, destroying texture", tex_width, tex_height, width, height);
                gs_texture_destroy(data->active_texture);
                data->active_texture = nullptr;
            }
        }

        if (!data->active_texture) {
            // format will always be AV_PIX_FMT_BGRA (see GLInjectInput.cpp)
            gs_texture_t* pTexture = gs_texture_create(width, height, gs_color_format::GS_BGRX, 1, nullptr, GS_DYNAMIC);
            blog(LOG_DEBUG, "Creating %dx%d texture = %p", width, height, pTexture);
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

static bool start_program_button_clicked(obs_properties_t* props, obs_property_t* property, void* data_in) {
    auto* data = (gl_data*) data_in;

    blog(LOG_DEBUG, "GLInjectInput::LaunchApplication(%s, %d, %s, %s)", data->channel, data->relax_permissions, data->command, ".");

    GLInjectInput::LaunchApplication(data->channel, data->relax_permissions, data->command, ".");

    return true;
}
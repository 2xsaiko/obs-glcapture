#include "common.h"
#include "ssr/GLInjectInput.h"
#include "obs_sink.h"
#include "video_wrapper.h"

#pragma once

static const char* gl_get_name(void* unused);

static void* gl_create(obs_data_t* settings, obs_source_t* source);

static void gl_destroy(void* vptr);

static void gl_update(void* vptr, obs_data_t* settings);

static uint32_t gl_get_width(void* vptr);

static uint32_t gl_get_height(void* vptr);

static obs_properties_t* gl_get_properties(void* vptr);

static void gl_get_defaults(obs_data_t* data);

static void gl_render(void* vptr, gs_effect_t* effect);

static void gl_show(void* vptr);

static void gl_hide(void* vptr);

static bool start_program_button_clicked(obs_properties_t* props, obs_property_t* property, void* vptr);

extern struct obs_source_info glsource;

struct gl_data {
    const char* channel;
    bool relax_permissions;
    const char* command;

    obs_source_t* source;
    VideoWrapper* vid;
    gs_texture_t* active_texture;
};
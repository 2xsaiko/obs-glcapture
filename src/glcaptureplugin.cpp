#include "glsource.h"
#include "ssr/Logger.h"

#include <obs/obs-module.h>

OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE("obs_glcapture", "en-US")

extern "C" bool obs_module_load() {
    new Logger();
    obs_register_source(&glsource);
    return true;
}

extern "C" void obs_module_unload() {
    delete Logger::GetInstance();
}
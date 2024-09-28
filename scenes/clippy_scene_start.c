#include "../clippy_app_i.h"
#include "core/core_defines.h"

void clippy_scene_start_on_enter(void* context) {
    ClippyApp* app = context;

    view_dispatcher_switch_to_view(app->view_dispatcher, ClippyAppViewStart);
}

bool clippy_scene_start_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    ClippyApp* app = context;
    UNUSED(app);

    // view_dispatcher_switch_to_view(app->view_dispatcher, ClippyAppViewStart);

    return false;
}

void clippy_scene_start_on_exit(void* context) {
    UNUSED(context);
    ClippyApp* app = context;
    UNUSED(app);
}

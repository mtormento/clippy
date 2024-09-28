#include "../clippy_app_i.h"
#include "core/core_defines.h"
#include "gui/modules/widget.h"
#include "gui/modules/widget_elements/widget_element.h"

static void
    clippy_copy_or_paste_button_callback(GuiButtonType result, InputType type, void* context) {
    ClippyApp* app = context;

    if(result == GuiButtonTypeRight && type == InputTypeShort) {
        scene_manager_next_scene(app->scene_manager, ClippyAppPasteItemSelection);
    }
}

void clippy_scene_copy_paste_sel_on_enter(void* context) {
    ClippyApp* app = context;

    widget_add_button_element(
        app->widget, GuiButtonTypeLeft, "Copy", clippy_copy_or_paste_button_callback, app);
    widget_add_button_element(
        app->widget, GuiButtonTypeRight, "Paste", clippy_copy_or_paste_button_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, ClippyAppCopyPasteSelection);
}

bool clippy_scene_copy_paste_sel_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    ClippyApp* app = context;
    UNUSED(app);

    return false;
}

void clippy_scene_copy_paste_sel_on_exit(void* context) {
    UNUSED(context);
    ClippyApp* app = context;

    widget_reset(app->widget);
}

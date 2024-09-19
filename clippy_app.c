#include "applications_user/clippy/clippy_app.h"
#include "gui/modules/loading.h"
#include <furi.h>
#include <gui/gui.h>
#include "clippy_app_i.h"

static bool clippy_app_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    ClippyApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool clippy_app_back_event_callback(void* context) {
    furi_assert(context);
    ClippyApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void clippy_app_tick_event_callback(void* context) {
    furi_assert(context);
    ClippyApp* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

ClippyApp* clippy_app_alloc(char* arg) {
    ClippyApp* app = malloc(sizeof(ClippyApp));

    app->file_path = furi_string_alloc();

    if(arg != NULL) {
        furi_string_set_str(app->file_path, arg);
    } else {
        furi_string_set_str(app->file_path, CLIPPY_APP_BASE_FOLDER);
    }

    app->gui = furi_record_open(RECORD_GUI);
    app->fs_api = furi_record_open(RECORD_STORAGE);
    app->dialogs = furi_record_open(RECORD_DIALOGS);

    app->view_dispatcher = view_dispatcher_alloc();

    app->scene_manager = scene_manager_alloc(&clippy_scene_handlers, app);

    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_tick_event_callback(
        app->view_dispatcher, clippy_app_tick_event_callback, 500);
    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, clippy_app_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, clippy_app_back_event_callback);

    app->clippy = clippy_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, ClippyAppViewStart, clippy_get_view(app->clippy));

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    scene_manager_next_scene(app->scene_manager, ClippySceneStart);

    return app;
}

void clippy_app_free(ClippyApp* app) {
    furi_assert(app);

    // Views
    view_dispatcher_remove_view(app->view_dispatcher, ClippyAppViewStart);

    clippy_free(app->clippy);

    // View dispatcher
    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);

    furi_string_free(app->file_path);

    // Close records
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_STORAGE);
    furi_record_close(RECORD_DIALOGS);

    free(app);
}

int32_t clippy_app(void* p) {
    ClippyApp* clippy_app = clippy_app_alloc((char*)p);
    view_dispatcher_run(clippy_app->view_dispatcher);
    clippy_app_free(clippy_app);
    return 0;
}

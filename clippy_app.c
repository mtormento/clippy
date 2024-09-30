#include "applications_user/clippy/clippy_app.h"
#include "core/string.h"
#include "gui/modules/loading.h"
#include "gui/modules/variable_item_list.h"
#include "gui/modules/widget.h"
#include "gui/view_dispatcher.h"
#include "storage/storage.h"
#include "views/clippy_mass_storage_view.h"
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

void clippy_app_show_loading_popup(ClippyApp* app, bool show) {
    if(show) {
        // Raise timer priority so that animations can play
        furi_timer_set_thread_priority(FuriTimerThreadPriorityElevated);
        view_dispatcher_switch_to_view(app->view_dispatcher, ClippyAppViewLoading);
    } else {
        // Restore default timer priority
        furi_timer_set_thread_priority(FuriTimerThreadPriorityNormal);
    }
}

ClippyApp* clippy_app_alloc() {
    ClippyApp* app = malloc(sizeof(ClippyApp));

    app->fat_image_file_path = furi_string_alloc_set_str(APP_ASSETS_PATH("fat.img"));

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

    // Loading view
    app->loading = loading_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, ClippyAppViewLoading, loading_get_view(app->loading));

    // Clippy mass storage view
    app->mass_storage_view = clippy_mass_storage_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        ClippyAppViewMassStorageWork,
        clippy_mass_storage_get_view(app->mass_storage_view));

    // Copy/Paste selection
    app->widget = widget_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, ClippyAppViewCopyPasteSelection, widget_get_view(app->widget));

    // Paste item selection
    items_array_init(app->items_array);
    app->variable_item_list = variable_item_list_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        ClippyAppViewPasteItemSelection,
        variable_item_list_get_view(app->variable_item_list));

    // Usb locked
    view_dispatcher_add_view(
        app->view_dispatcher, ClippyAppViewUsbLocked, widget_get_view(app->widget));

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    scene_manager_next_scene(app->scene_manager, ClippyAppViewCopyPasteSelection);

    return app;
}

void clippy_app_free(ClippyApp* app) {
    furi_assert(app);

    // Paste item selection
    items_array_clear(app->items_array);
    variable_item_list_free(app->variable_item_list);

    // Copy/Paste selection
    widget_free(app->widget);

    // Clippy mass storage view
    clippy_mass_storage_free(app->mass_storage_view);

    // Loading view
    loading_free(app->loading);

    // Views
    view_dispatcher_remove_view(app->view_dispatcher, ClippyAppViewCopyPasteSelection);
    view_dispatcher_remove_view(app->view_dispatcher, ClippyAppViewPasteItemSelection);
    view_dispatcher_remove_view(app->view_dispatcher, ClippyAppViewMassStorageWork);
    view_dispatcher_remove_view(app->view_dispatcher, ClippyAppViewUsbLocked);
    view_dispatcher_remove_view(app->view_dispatcher, ClippyAppViewLoading);

    // View dispatcher
    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);

    furi_string_free(app->fat_image_file_path);

    // Close records
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_STORAGE);
    furi_record_close(RECORD_DIALOGS);

    free(app);
}

int32_t clippy_app() {
    ClippyApp* clippy_app = clippy_app_alloc();
    view_dispatcher_run(clippy_app->view_dispatcher);
    clippy_app_free(clippy_app);
    return 0;
}

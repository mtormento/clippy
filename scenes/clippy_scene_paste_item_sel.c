#include "../clippy_app_i.h"
#include "core/core_defines.h"
#include "core/string.h"
#include "gui/modules/variable_item_list.h"
#include "gui/scene_manager.h"
#include "helpers/clippy_fatreader.h"

void test_fatreader(ClippyApp* app) {
    const char* image_filename = APP_ASSETS_PATH("fat.img");

    FHandle* handle = malloc(sizeof(FHandle));
    FRESULT res = fatreader_open_image(handle, image_filename, app->fs_api);
    if(res != FR_OK) {
        free(handle);
        return;
    }
    fatreader_print_info(handle);

    RootDirectory* root_directory = malloc(sizeof(RootDirectory));
    res = fatreader_root_directory_open(root_directory, handle);
    if(res != FR_OK) {
        goto free_stuff;
    }

    DIR dir;
    res = fatreader_root_directory_find_first(&dir, root_directory);
    if(res != FR_OK) {
        goto close_stuff;
    }
    if(dir.attr == FR_ATTR_ARCHIVE) {
        variable_item_list_add(
            app->variable_item_list,
            furi_string_get_cstr(furi_string_alloc_set_str(dir.name)),
            0,
            NULL,
            app);
    }

    while((res = fatreader_root_directory_find_next(&dir, root_directory)) == FR_OK) {
        if(dir.attr == FR_ATTR_ARCHIVE) {
            variable_item_list_add(
                app->variable_item_list,
                furi_string_get_cstr(furi_string_alloc_set_str(dir.name)),
                0,
                NULL,
                app);
        }
    }

close_stuff:
    res = fatreader_root_directory_close(root_directory);
    if(res != FR_OK) {
        goto free_stuff;
    }

    res = fatreader_close_image(handle);
    if(res != FR_OK) {
        goto free_stuff;
    }
free_stuff:

    // free(file_buffer);
    free(root_directory);
    free(handle);
}

void clippy_scene_paste_item_sel_on_enter(void* context) {
    ClippyApp* app = context;

    test_fatreader(app);

    view_dispatcher_switch_to_view(app->view_dispatcher, ClippyAppPasteItemSelection);
}

bool clippy_scene_paste_item_sel_on_event(void* context, SceneManagerEvent event) {
    ClippyApp* app = context;

    bool consumed = false;

    if(event.type == SceneManagerEventTypeBack) {
        consumed = scene_manager_previous_scene(app->scene_manager);
    }

    return consumed;
}

void clippy_scene_paste_item_sel_on_exit(void* context) {
    UNUSED(context);
    ClippyApp* app = context;

    variable_item_list_reset(app->variable_item_list);
}

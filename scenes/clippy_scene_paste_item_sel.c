#include "../clippy_app_i.h"
#include "core/core_defines.h"
#include "gui/modules/variable_item_list.h"
#include "gui/scene_manager.h"
#include "helpers/clippy_fatreader.h"
#include "m-string.h"

void prepare_varible_item_list(ClippyApp* app) {
    const char* image_filename = APP_ASSETS_PATH("fat.img");

    FHandle* handle = malloc(sizeof(FHandle));
    FRESULT res = fatreader_open_image(handle, image_filename, app->fs_api);
    if(res != FR_OK) {
        free(handle);
        return;
    }

    RootDirectory* root_directory = malloc(sizeof(RootDirectory));
    res = fatreader_root_directory_open(root_directory, handle);
    if(res != FR_OK) {
        goto free_stuff;
    }

    const char* file_to_look_for = "CLIPPY  TXT";

    DIR dir;
    res = fatreader_root_directory_find_by_name(file_to_look_for, &dir, root_directory);
    if(res != FR_OK) {
        goto close_stuff;
    }
    if(dir.file_size > 1024) {
        // TODO: we need to bail out here
    }
    // Clipboard file found: let's read it now
    FIL file;
    res = fatreader_file_open(&file, &dir);
    if(res != FR_OK) {
        goto close_stuff;
    }
    u8* buffer = malloc(1025);
    memset(buffer, 0, 1025);
    size_t bytes_read;
    res = fatreader_file_read(buffer, &file, 1024, &bytes_read);
    string_t str;
    string_init(str);

    // Read lines
    size_t start_of_line = 0;
    for(size_t i = 0; i < bytes_read; i++) {
        u8 ch = buffer[i];
        if(ch == '\n') {
            buffer[i] = 0x00;
            if((i - 1 - start_of_line) > 0) {
                string_printf(str, "%s", &buffer[start_of_line]);
                items_array_push_back(app->items_array, str);
            }
            start_of_line = i + 1;
        } else if(i == (bytes_read - 1) && (i - start_of_line) > 0) {
            string_printf(str, "%s", &buffer[start_of_line]);
            items_array_push_back(app->items_array, str);
        }
    }
    string_clear(str);
    free(buffer);

    for(size_t i = 0; i < items_array_size(app->items_array); i++) {
        variable_item_list_add(
            app->variable_item_list,
            string_get_cstr(*items_array_get(app->items_array, i)),
            0,
            NULL,
            app);
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

    free(root_directory);
    free(handle);
}

void clippy_scene_paste_item_sel_on_enter(void* context) {
    ClippyApp* app = context;

    prepare_varible_item_list(app);

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
    items_array_reset(app->items_array);
}
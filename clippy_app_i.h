#pragma once

#include "clippy_app.h"

#include "gui/modules/loading.h"
#include "scenes/clippy_scene.h"
#include "storage/filesystem_api_internal.h"
#include <gui/gui.h>
#include <assets_icons.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <dialogs/dialogs.h>
#include <notification/notification_messages.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/widget.h>
#include <furi_hal_usb.h>
#include "storage/storage.h"
#include "views/clippy_view.h"

#define CLIPPY_APP_BASE_FOLDER        EXT_PATH("clippy")
#define CLIPPY_APP_PATH_LAYOUT_FOLDER CLIPPY_APP_BASE_FOLDER "/assets/layouts"
#define CLIPPY_APP_SCRIPT_EXTENSION   ".txt"
#define CLIPPY_APP_LAYOUT_EXTENSION   ".kl"

typedef enum {
    ClippyAppErrorNoFiles,
    ClippyAppErrorCloseRpc,
} ClippyAppError;

struct ClippyApp {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    DialogsApp* dialogs;
    Storage* fs_api;
    Widget* widget;
    VariableItemList* variable_item_list;
    Loading* loading;

    Clippy* clippy;

    ClippyAppError error;
    FuriString* file_path;

    FuriHalUsbInterface* usb_if_prev;
};

typedef enum {
    ClippyAppViewStart,
    ClippyAppCopyPasteSelection,
    ClippyAppPasteItemSelection,
} ClippyAppView;

enum ClippyCustomEvent {
    // Reserve first 100 events for button types and indexes, starting from 0
    MassStorageCustomEventReserved = 100,

    MassStorageCustomEventEject,
    MassStorageCustomEventFileSelect,
    MassStorageCustomEventNewImage,
    MassStorageCustomEventNameInput,
};

#pragma once

#include "clippy_app.h"

#include <gui/gui.h>
#include <assets_icons.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <dialogs/dialogs.h>
#include <notification/notification_messages.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/widget.h>
#include <furi_hal_usb.h>

#define CLIPPY_APP_BASE_FOLDER        EXT_PATH("clippy")
#define CLIPPY_APP_PATH_LAYOUT_FOLDER BAD_USB_APP_BASE_FOLDER "/assets/layouts"
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
    NotificationApp* notifications;
    DialogsApp* dialogs;
    Widget* widget;
    VariableItemList* var_item_list;

    ClippyAppError error;
    FuriString* file_path;
    FuriString* keyboard_layout;

    FuriHalUsbInterface* usb_if_prev;
};

typedef enum {
    ClippyAppViewError,
    ClippyAppViewWork,
    ClippyAppViewConfig,
} ClippyAppView;

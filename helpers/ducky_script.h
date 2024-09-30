#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <furi.h>
#include <furi_hal.h>
#include "bad_usb_hid.h"

typedef enum {
    ClippyBadUsbStateInit,
    ClippyBadUsbStateNotConnected,
    ClippyBadUsbStateIdle,
    ClippyBadUsbStateWillRun,
    ClippyBadUsbStateRunning,
    ClippyBadUsbStateDelay,
    ClippyBadUsbStateStringDelay,
    ClippyBadUsbStateWaitForBtn,
    ClippyBadUsbStatePaused,
    ClippyBadUsbStateDone,
    ClippyBadUsbStateScriptError,
    ClippyBadUsbStateFileError,
} ClippyBadUsbWorkerState;

typedef struct {
    ClippyBadUsbWorkerState state;
    size_t line_cur;
    size_t line_nb;
    uint32_t delay_remain;
    size_t error_line;
    char error[64];
} ClippyBadUsbState;

typedef struct BadUsbScript BadUsbScript;

BadUsbScript* clippy_bad_usb_script_open(FuriString* file_path, BadUsbHidInterface interface);

void clippy_bad_usb_script_close(BadUsbScript* bad_usb);

void clippy_bad_usb_script_set_keyboard_layout(BadUsbScript* bad_usb, FuriString* layout_path);

void clippy_bad_usb_script_start(BadUsbScript* bad_usb);

void clippy_bad_usb_script_stop(BadUsbScript* bad_usb);

void clippy_bad_usb_script_start_stop(BadUsbScript* bad_usb);

void clippy_bad_usb_script_pause_resume(BadUsbScript* bad_usb);

ClippyBadUsbState* clippy_bad_usb_script_get_state(BadUsbScript* bad_usb);

#ifdef __cplusplus
}
#endif

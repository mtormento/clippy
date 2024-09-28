#pragma once

#include <storage/storage.h>
#include "clippy_scsi.h"

typedef struct MassStorageUsb MassStorageUsb;

MassStorageUsb* mass_storage_usb_start(const char* filename, SCSIDeviceFunc fn);
void mass_storage_usb_stop(MassStorageUsb* mass);

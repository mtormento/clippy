#pragma once

#include <gui/view.h>

typedef struct Clippy Clippy;

Clippy* clippy_alloc();

void clippy_free(Clippy* clippy);

View* clippy_get_view(Clippy* clippy);

void clippy_set_file_name(Clippy* clippy, FuriString* name);

void clippy_set_stats(Clippy* clippy, uint32_t read, uint32_t written);

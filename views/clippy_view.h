#pragma once

#include <gui/view.h>

typedef struct Clippy Clippy;

Clippy* clippy_alloc();

void clippy_free(Clippy* clippy);

View* clippy_get_view(Clippy* clippy);

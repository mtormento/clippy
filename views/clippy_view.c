#include "clippy_view.h"
#include "../clippy_app_i.h"
#include <gui/elements.h>

struct Clippy {
    View* view;
};

typedef struct {
} ClippyModel;

static void clippy_draw_callback(Canvas* canvas, void* _model) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, canvas_width(canvas) / 2, 0, AlignCenter, AlignTop, "Clippy");
}

Clippy* clippy_alloc() {
    Clippy* clippy = malloc(sizeof(Clippy));

    clippy->view = view_alloc();
    view_allocate_model(clippy->view, ViewModelTypeLocking, sizeof(ClippyModel));

    // Set model defaults

    view_set_context(clippy->view, clippy);
    view_set_draw_callback(clippy->view, clippy_draw_callback);

    return clippy;
}

void clippy_free(Clippy* clippy) {
    furi_assert(clippy);

    // Free model resources

    view_free(clippy->view);
    free(clippy);
}

View* clippy_get_view(Clippy* clippy) {
    furi_assert(clippy);
    return clippy->view;
}

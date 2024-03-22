#ifndef RENDER_H
# define RENDER_H

# include <Compound/common.h>
# include <Compound/pen.h>

/** Rendering */
void compound_pen_render_refresh();
void compound_pen_render_full();
void compound_pen_render_full_with(int *buff);
void compound_pen_render_partial(Selection selc);
void compound_pen_render_partial_with(Selection selc, int *buff);
void compound_pen_render_highlight(Selection selc, int method);
void compound_pen_set_cursor_highlight_method(Pen *pen, int method);

#endif  /* RENDER_H */

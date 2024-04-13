#ifndef PEN_H
# define PEN_H

# include <stdint.h>
# include <stdlib.h>
# include <stdbool.h>
# include <wchar.h>

# include <Compound/common.h>
# include <Compound/status.h>

# define PEN_BUFFER_MAX_LEN  (UINT16_MAX + 1)

enum PEN_OPERATION {
  MOVE  = 0,
  READ  = 1,
  WRITE = 2,
};

enum PEN_RENDER_HIGHLIGHT_METHOD {

  NONE              = 0b00000000,
  
  BOLD              = 0b00000001,
  ITALIC            = 0b00000010,
  UNDERSCORE        = 0b00000100,
  STRIKETHROUGH     = 0b00001000,
  INVERT            = 0b00010000,
  BOX               = 0b00100000,
  BLINK             = 0b01000000,
  BLOCK             = 0b10000000,

  UNBOLD            =-0b00000001,
  UNITALIC          =-0b00000010,
  UNUNDERSCORE      =-0b00000100,
  UNSTRIKETHROUGH   =-0b00001000,
  UNINVERT          =-0b00010000,
  UNBOX             =-0b00100000,
  UNBLINK           =-0b01000000,
  UNBLOCK           =-0b10000000,
};

# define PEN_CURSOR_STYLE_ACTIVATED  BLOCK
# define PEN_CURSOR_STYLE_INACTIVATED  BOX
# define PEN_CURSOR_STYLE_IDLE  (PEN_CURSOR_STYLE_ACTIVATED | BLINK)

typedef struct {
  const int id;
  Coordination coord;
  int value;
  int operation;  // -1 for UNKNOWN, tough it is not specified in enumeration,
                  // the detection will still consider any number that below 0
                  // as an valid, but unknown one.
  int cursor_style;  // -1 for disabling
} Pen;

# define PEN_SELECTION_DOT(x, y)  (Selection){ (Coordination){ x, y },     \
                                               (Coordination){ x, y } }
# define PEN_VALUE_EMPTY  32

# define PEN_NONULL(o, n)  if (o == NULL)  return (n);

enum PenError {
  PEN_ERROR_FAILED_ON_PEN_NULLITY = -1,
  PEN_ERROR_FAILED_ON_FUNCTION_POINTER_REFERENCE = -2
};

Status pen_create(Pen *pen, int cursor_style);
Status pen_others(Pen *pen, Pen src);
Status pen_constr(Pen *pen, const int id, Coordination coord, int value,
                  int operation, int cursor_style);
void pen_delete(Pen *pen);

#endif  /* PEN_H */

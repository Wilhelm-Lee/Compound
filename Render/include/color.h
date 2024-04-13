#ifndef COMPOUND_RENDER_COLOR_H
# define COMPOUND_RENDER_COLOR_H

# include <Compound/common.h>

/*
 * 0   to restore default color
 * 1   for brighter colors   
 * 4   for underlined text   
 * 5   for flashing text     
 * 30  for black foreground  
 * 31  for red foreground    
 * 32  for green foreground  
 * 33  for yellow (or brown) foreground
 * 34  for blue foreground   
 * 35  for purple foreground 
 * 36  for cyan foreground   
 * 37  for white (or gray)   foreground
 * 40  for black background  
 * 41  for red background    
 * 42  for green background  
 * 43  for yellow (or brown) background
 * 44  for blue background   
 * 45  for purple background 
 * 46  for cyan background   
 * 47  for white (or gray)   background
 */

// enum Colour16 {
//   COLOR16_DIM_BLACK,
//   COLOR16_DIM_RED,
//   COLOR16_DIM_GREEN,
//   COLOR16_DIM_BLUE,
//   COLOR16_DIM_YELLOW,
//   COLOR16_DIM_MAGENTA,
//   COLOR16_DIM_CYAN,
//   COLOR16_DIM_GREY,
  
//   COLOR16_BRIGHT_BLACK,
//   COLOR16_BRIGHT_RED,
//   COLOR16_BRIGHT_GREEN,
//   COLOR16_BRIGHT_BLUE,
//   COLOR16_BRIGHT_YELLOW,
//   COLOR16_BRIGHT_MAGENTA,
//   COLOR16_BRIGHT_CYAN,
//   COLOR16_WHITE,
// };

# define COLOR_VALUE_MIN  0
# define COLOR_VALUE_MAX  255

typedef struct {
  bool color_red;
  bool color_green;
  bool color_blue;
  bool color_on_foreground;
  
  bool effect_bold;
  bool effect_italic;
  bool effect_underline;
  bool effect_box;
  bool effect_strikethrough;
  bool effect_invert;  
} ColorProperty;

typedef struct {
  int red;
  int green;
  int blue;
  bool onfg;
} Color;

Color color(int red, int green, int blue, bool onfg);
ColorProperty color_to_property(Color color);
Color color_from_property(ColorProperty prop);
void color_wrapvalue(int *v);

# define COLOR_INRANGE(v) \
  (INRANGE(COLOR_VALUE_MIN, true, COLOR_VALUE_MAX, true, v))

# define COLOR_ATRANGE(v) \
  (ATRANGE(COLOR_VALUE_MIN, COLOR_VALUE_MAX, v))

#endif  /* COMPOUND_RENDER_COLOR_H */

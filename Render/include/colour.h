#warning No colour will be used at the stage.  --22th Mar 2024

#ifndef COMPOUND_RENDER_COLOUR_H
# define COMPOUND_RENDER_COLOUR_H

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
static const char *COLOUR8_BLACK = "\\[";
static const char *COLOUR8_RED = "";
static const char *COLOUR8_GREEN = "";
static const char *COLOUR8_BLUE = "";
static const char *COLOUR8_YELLOW = "";
static const char *COLOUR8_MAGENTA = "";
static const char *COLOUR8_CYAN = "";
static const char *COLOUR8_WHITE = "";

// enum Colour16 {
//   COLOUR16_DIM_BLACK,
//   COLOUR16_DIM_RED,
//   COLOUR16_DIM_GREEN,
//   COLOUR16_DIM_BLUE,
//   COLOUR16_DIM_YELLOW,
//   COLOUR16_DIM_MAGENTA,
//   COLOUR16_DIM_CYAN,
//   COLOUR16_DIM_GREY,
  
//   COLOUR16_BRIGHT_BLACK,
//   COLOUR16_BRIGHT_RED,
//   COLOUR16_BRIGHT_GREEN,
//   COLOUR16_BRIGHT_BLUE,
//   COLOUR16_BRIGHT_YELLOW,
//   COLOUR16_BRIGHT_MAGENTA,
//   COLOUR16_BRIGHT_CYAN,
//   COLOUR16_WHITE,
// };

static const bit COMPOUNR_COLOUR_BACKGROUND = 0;
static const bit COMPOUNR_COLOUR_FOREGROUND = 1;

#endif  /* COMPOUND_RENDER_COLOUR_H */

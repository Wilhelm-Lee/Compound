#include <Compound/color.h>

Color color(int red, int green, int blue, bool onfg)
{
  color_wrapvalue(&red);
  color_wrapvalue(&green);
  color_wrapvalue(&blue);
  
  return (Color) {
    .red = red,
    .green = green,
    .blue = blue,
    .onfg = onfg
  };
}

ColorProperty color_to_property(Color color)
{  
  return (ColorProperty) {
    .color_red = color.red,
    .color_green = color.green,
    .color_blue = color.blue,
    .color_on_foreground = color.onfg,
    .effect_bold = false,
    .effect_italic = false,
    .effect_underline = false,
    .effect_box = false,
    .effect_strikethrough = false,
    .effect_invert = false
  };
}

Color color_from_property(ColorProperty prop)
{
  return (Color) {
    .red = (prop.color_red ? (COLOR_VALUE_MAX) : (COLOR_VALUE_MIN)),
    .green = (prop.color_green ? (COLOR_VALUE_MAX) : (COLOR_VALUE_MIN)),
    .blue = (prop.color_blue ? (COLOR_VALUE_MAX) : (COLOR_VALUE_MIN)),
    .onfg = prop.color_on_foreground
  };
}

void color_wrapvalue(int *v)
{
  if (v == NULL) {
    return;
  }
  
  const int outrange = COLOR_ATRANGE(*v);

  /* Within the range */
  if (outrange == 0) {
    return;
  }
  
  /* Above the range */
  if (outrange > 0) {
    *v = COLOR_VALUE_MAX;
    return;
  }
  
  /* Below the range */
  *v = COLOR_VALUE_MIN;
}

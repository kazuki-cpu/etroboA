#ifndef colormit_h
#define colormit_h
#include "ColorSensor.h" // <2>
using namespace ev3api;

enum eColorname{
  clr_none=0,
  clr_red=1,
  clr_blue=2,
  clr_black=3
};
typedef struct {
  unsigned int R;
  unsigned int G;
  unsigned int B;
  double bright;//0dark~255bright
  enum eColorname name;
}sColor;

class ColorMonit{
  public:
    ColorMonit();
    sColor cycle();
    sColor getColor();
  private:
    ColorSensor colorsens;
    sColor color_now;
    enum eColorname getColorname(int R,int G,int B);
};

#endif


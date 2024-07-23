#include "ColorMonit.h" //self header
#include "valdef.h"
ColorMonit::ColorMonit():
  colorsens(PORT_2){}

sColor ColorMonit::cycle(){
  rgb_raw_t rgb_now;
  colorsens.getRawColor(rgb_now);

  color_now.R=rgb_now.r;
  color_now.G=rgb_now.g;
  color_now.B=rgb_now.b;
  color_now.bright=  COLOR_Midgray-(color_now.R + color_now.G + color_now.B)/3.0;
  if (color_now.bright<-30){
    color_now.bright=-30;
  }
  if (color_now.bright>30){
    color_now.bright=30;
  }
  color_now.name=getColorname(rgb_now.r,rgb_now.g,rgb_now.b);

  return color_now;
}
sColor ColorMonit::getColor(){
  return color_now;
}

enum eColorname ColorMonit::getColorname(int R,int G,int B){
  if (R<60&&G<60&&B<60){
    return clr_black;
  }
  if ((R<60&&G<60&&B>110)||(B>110&&(B-G>90)&&(B-R>90))){
    return clr_blue;
  }
  if ((R>110&&G<60&&B<60)||(R>110&&(R-G>90)&&(R-B>90))){
    return clr_red;
  }
  return clr_none;
}
/*
ColorMonitor::ColorMonitor():
            colorSensor(PORT_2){
            }

void Color::init(){
    colorSensor.getRAWColor(rgb)
    Red = rgb.r;
    Green = rgb.g;
    Blue = rgb.b;

}
*/
/////↓はHSV計算式/////

/*
//コンストラクタ

ColorMonitor::ColorMonitor():
            mColorSensor(PORT_2),
            mRed(0.0f),
            mGreen(0.0f),
            mBlue(0.0f){}

void ColorMonitor::getRGB_raw(rgb_raw_t & rgb){
    mColorSensor.getRAWColor(rgb_val);

    mRed = (float)(rgb_val.r);
    mGreen = (float)(rgb_val.g);
    mBlue = (float)(rgb_val.b)
}
*/

/* 反射光は使用しないため無効化
int8_t getBrightness() constz{
    return ColorSensor.getBrightness(); //反射光の強さ(0-100)
}
*/


/*
//黒色判定
bool ColorMonitor::Black_HSV(){
    bool jdg_val;

    calcHSV();
    
    //↓の数値はあてずっぽうなので実機で計測したい
    if((loc_hsv.h < 0.40f) && (loc_hsv.s < 0.1f)){
        jdg_val = true;
    }else{
        jdg_val = false;
    }
    return jdg_val;
}

//白色判定
bool ColorMonitor::White_HSV(){
    bool jdg_val;

    calcHSV();
    
    //↓の数値はあてずっぽうなので実機で計測したい
    if((loc_hsv.h > 0.40f) && (loc_hsv.s > 0.1f)){
        jdg_val = true;
    }else{
        jdg_val = false;
    }
    return jdg_val;
}

//青色判定
bool ColorMonitor::Blue_HSV(){
    bool jdg_val;

    calcHSV();
    
    //↓の数値はあてずっぽうなので実機で計測したい
    if((loc_hsv.h > 0.30f) && (loc_hsv.h < 0.33f)){
        jdg_val = true;
    }else{
        jdg_val = false;
    }
    return jdg_val;
}

//キャリブレーションを行い
//黒と白の時のrgb値をオフセットする
void ColorMonitor::getRGB(){
    mColorSensor.getRAWColor(rgb_val);

    //start_calivration
    mRed = ((float)(rgb_val.r - offsetBlack.r))*coefficientWhite.r;
    mGreen = ((float)(rgb_val.g - offsetBlack.g))*coefficientWhite.g;
    mBlue = ((float)(rgb_val.b - offsetBlack.b))*coefficientWhite.b;
}

void ColorMonitor::getRAWColor(rgb_raw_t & rgb){
    mColorSensor.getRAWColor(rgb_val);

    rgb.r = rgb_val.r;
    rgb.g = rgb_val.g;
    rgb.b = rgb_val.b;
}


//
void ColorMonitor::getHSVValue(hsv_val_t & hsv){
    calcHSV();
    hsv = loc_hsv;
}

//RGB→HSV変換処理
//calcHSV();で
//loc_hsv.h, loc_hsv.s, loc_hsv.vとして呼び出し
void ColorMonitor::calcHSV(){
    getRGB();
    float r,g,b;
    r = (float)(mRed)/100.0f;
    g = (float)(mGreen)/100.0f;
    b = (float)(mBlue)/100.0f;

    float max = r > g ? r : g;
    max = max > b ? max : b;

    float min = r < g ? r : g;
    min = min > b ? min : b;

    float h = max - min;
    loc_hsv.s = h;

    if(max != 0.0f){
        loc_hsv.s /= max; //Saturation 彩度
    }
    if(h > 0.0f){
        if(max == r){
            h = (g - b)/h;
            if(h < 0.0f){
                h += 6.0f;
            }
        }else if(max == g){
            h = 2.0f + (b - r)/h;
        }else{
            h = 4.0f + (r - g)/h;
        }
    }
    loc_hsv.h = h/6.0f; //Hue 色相

    loc_hsv.v = max; //Value,Braightness 明度
}

//キャリブレーション用の黒rgbと白rgb
void ColorMonitor::ColorCalib(offset_val_t offset, coef_val_t coefficient){
    offsetBlack.r = offset.r;
    offsetBlack.g = offset.g;
    offsetBlack.b = offset.b;

    coefficientWhite.r = coefficient.r;
    coefficientWhite.g = coefficient.g;
    coefficientWhite.b = coefficient.b;
}
*/
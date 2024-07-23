#ifndef mathfnc_h
#define mathfnc_h

#include "valdef.h"
#define LEFT 1
#define RIGHT 2
#define NONE 0

#define PI		3.14159265358979323846

namespace MathFnc {
  double abs(double in);

  /*from rad to degree*/
  double rad2deg(double rad);

  /*return rad一周の値 -PI~PI以内に換算する*/
  double rad1round (double rad);

  double deg2rad(double deg);
};

class PIDFnc{
  public:
    PIDFnc();
    PIDFnc(double init_kp,double init_ki,double init_kd);
    double cycle(double en);
    void clean();
  private:
    double kp,ki,kd;
    double MVn,MVn_last,MVn_delta;
    double en,en_1,en_2;
    
};
/*まず、サンプリング方式のＰＩＤ制御の基本の式は下記で表されます。
操作量＝Kp×偏差＋Ki×偏差の累積値＋Kd×前回偏差との差
　　　　　（比例項）　　（積分項）　　　（微分項）
記号で表すと
　MVn = MVn-1 + ΔMVn
　ΔMVn = Kp(en-en_1) + Ki en + Kd((en-en_1) - (en_1-en_2))

MVn、MVn_1:今回、前回操作量　　ΔMVn:今回操作量差分
en,en_1,en_2：今回、前回、前々回の偏差

*/
#endif
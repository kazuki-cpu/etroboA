#ifndef distancemit_h
#define distancemit_h
#include "SonarSensor.h"
using namespace ev3api;

// 定義
typedef struct {
  bool listen_flg;
  int16_t Distance;//cm
}sDistance;

class DistanceMonitor {
  public:
    DistanceMonitor();
    sDistance cycle();
    sDistance getDistance();
  private:
    SonarSensor sonarsens;
    sDistance distance_now;
};

#endif

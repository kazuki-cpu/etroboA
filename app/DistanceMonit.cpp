#include "DistanceMonit.h"

DistanceMonitor::DistanceMonitor():
  sonarsens(PORT_3){}

sDistance DistanceMonitor::cycle(){
  distance_now.Distance = sonarsens.getDistance();
  distance_now.listen_flg= sonarsens.listen();
  return distance_now;
}
sDistance DistanceMonitor::getDistance(){
  return distance_now;
}

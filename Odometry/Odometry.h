/* 円周率 */
#define PI 3.14159265358
#define TREAD 50
#define TIRE_DIAMETER 0.81

  void odom_print_distance();
  void odom_terminate();
  void odom_Distance_reset();
  void odom_Distance_update();
  float odom_Distance_getDistance();
  float odom_Distance_getDistanceRight();
  float odom_Distance_getDistanceLeft();
  void odom_Direction_reset();
  float odom_Direction_getDirection();
  void odom_Direction_update();

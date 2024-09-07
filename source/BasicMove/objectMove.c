#include <stdlib.h>
#include <kernel.h>

#include <spike/hub/system.h>

#include <WRO.h>

#include "spike/pup/motor.h"
#include "spike/pup/colorsensor.h"
#include "spike/pup/forcesensor.h"
#include "spike/pup/ultrasonicsensor.h"

#include "spike/hub/battery.h"
#include "spike/hub/button.h"
#include "spike/hub/display.h"
#include "spike/hub/imu.h"
#include "spike/hub/light.h"

#include "../Include/arm.h"
#include "../Include/colorSensor.h"
#include "../Include/gyro.h"
#include "../Include/motor.h"
#include "../Include/def.h"
#include "../Include/system.h"


#include "../general_var.h"

#include <pbio/color.h>

#include "kernel_cfg.h"

char objectColor[4][2] = {0};   //0 : near Start / 1 : near Waterpipe / 2 : in bridge / 3 : in step
int bluefloors = 0;
int greenfloors = 0;

//House elementを回収する時に使用
//GetInside = 0 : Inside / 1 : Move / 2 : No move
void ObjectCatch(int StartPoint, int TargetAngle, bool MotorReset, int GetInside)
{ 
  ArmOpen(220);
  GyroMotor(50, StartPoint, TargetAngle, 0, MotorReset);
  ArmClose();
  ArmLock(true);
  GyroMotor(-40, -10, TargetAngle, 0, true);
  ArmUpdown(1);
  dly_tsk(100000);
  GyroMotor(40, 20, TargetAngle, 3, false);
  GyroMotor(50, 40, TargetAngle, 3, false);
  GyroMotor(40, 60, TargetAngle, 0, false);
  dly_tsk(100000);
  ArmUpdown(-1);
  dly_tsk(100000);
  pup_motor_set_power(armB, 50);
  ArmLock(false);
  ArmOpen(180);
  pup_motor_set_power(armB, -20);
  dly_tsk(300000);
  GyroMotor(35, 90, TargetAngle, 3, false);
  pup_motor_set_power(armB, -10);
  GyroMotor(35, 110, TargetAngle, 0, false);
  ArmUpdown(0);
  ArmClose();
  
  
  if (GetInside == 0)
  {
    ArmOpen(200);
    GyroMotor(-30, 90, TargetAngle, 0, false);
    ArmUpdown(2);
    dly_tsk(100000);
    GyroMotor(35, 210, TargetAngle, 3, false);
    pup_motor_set_power(armB, -10);
    GyroMotor(35, 240, TargetAngle, 0, false);
    while (pup_motor_get_count(armB) >= 10)
    {
      if (pup_motor_is_stalled(armB))
      {
        pup_motor_set_power(armB, 60);
        dly_tsk(300000);
        pup_motor_set_power(armB, -40);
      }
    }
    dly_tsk(100000);
  }
  else if (GetInside == 1)
  {
    ArmLock(true);
    tGyroMotor(1, 60, 30, 30, 240, TargetAngle, 0, false);
    //ADJMotor(50, 230, 0, false);
  }
  else
  {
    ArmLock(true);
    dly_tsk(100000);
  }

}

//House elementを4階建てにするときに使用
void Object4floorPiled()
{ 
  ArmLock(true);
  ArmUpdown(2);
  dly_tsk(100000);
  ADJMotor(-50, -95, 0, true);
  dly_tsk(200000);
  pup_motor_set_power(armB, -5);
  pup_motor_set_power(armB, 0);
  while (pup_motor_get_count(armB) >= 55){}
  pup_motor_hold(armB);
  dly_tsk(100000);
  pup_motor_set_power(armB, 30);
  ArmLock(false);
  ArmOpen(200);
  pup_motor_set_speed(armB, 0);
  dly_tsk(100000);
}

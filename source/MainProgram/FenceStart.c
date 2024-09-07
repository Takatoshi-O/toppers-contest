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

#include "general_var.h"
#include "all_include.h"

#include "../Include/def.h"

#include <pbio/color.h>

#include "kernel_cfg.h"

void FenceStart()
{
  //北側の赤・黄のHouse element回収
  pup_motor_set_power(armB, -30);
  pup_motor_set_power(armF, -60);
  SecMotor(-50, 0.4, 1);
  GyroReset();
  OffsetGyro(true);
  pup_motor_reset_count(armF);
  pup_motor_reset_count(armB);
  pup_motor_stop(armB);
  pup_motor_stop(armF);
  ArmOpen(220);
  dly_tsk(100000);
  GyroSpinTurn(-60, 3, false);
  GyroPivotTurn(-90, 1, 0, false);
  dly_tsk(100000);
  ADJBasePower = 50;
  ADJMotorOn(true);
  while (pup_color_sensor_reflection(UnderColorSensor) < 70)
  {
    dly_tsk(1);
  }
  while (pup_color_sensor_reflection(UnderColorSensor) > 35)
  {
    dly_tsk(1);
  }
  ADJMotorOn(false);
  ObjectCatch(190, -90, true, 0);
  ObjectCatch(120, -90, true, 2);

  //南側の赤のHouse elementに移動
  GyroMotor(-35, -30, -90, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(0, 0, false);
  SecMotor(-50, 0.4, 1);
  GyroReset();
  OffsetGyro(true);
  tGyroMotor(1, 95, 30, 60, 1200, 0, 3, true);
  ADJBasePower = 60;
  GyroMotorOn(true);
  while (pup_color_sensor_reflection(UnderColorSensor) < 60)
  {
    dly_tsk(1);
  }
  GyroMotor(40, 50, 0, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(90, 0, false);
  dly_tsk(100000);
  tGyroMotor(-1, 60, 30, 30, 240, 90, 0, true);
  ArmLock(false);
  ArmOpen(190);
  dly_tsk(100000);
  tGyroMotor(-1, 70, 30, 30, 340, 90, 0, true);
  GyroSpinTurn(180, 0, false);
  dly_tsk(100000);
  tGyroMotor(-1, 70, 50, 50, 320, 180, 3, true);
  //GyroMotor(-60, -240, 180, 3, true);
  ADJBasePower = -50;
  GyroMotorOn(true);
  dly_tsk(300000);
  GyroMotorOn(false);
  MotorStop(1);
  GyroReset();
  OffsetGyro(true);

  //南側の赤のHouse element回収
  ArmOpen(220);
  dly_tsk(100000);
  GyroSpinTurn(-60, 3, false);
  GyroPivotTurn(-90, 1, 0, false);
  dly_tsk(100000);
  ADJBasePower = 50;
  ADJMotorOn(true);
  while (pup_color_sensor_reflection(UnderColorSensor) < 70)
  {
    dly_tsk(1);
  }
  while (pup_color_sensor_reflection(UnderColorSensor) > 35)
  {
    dly_tsk(1);
  }
  ADJMotorOn(false);
  ObjectCatch(200, -90, true, 2);

  //赤のHouse elementをHouse Areaで4階建てにする
  tGyroMotor(-1, 65, 30, 50, 230, -90, 3, true);
  GyroMotorOn(true);
  while (pup_color_sensor_reflection(UnderColorSensor) < 70)
  {
    dly_tsk(1);
  }
  GyroMotor(-50, -30, -90, 3, true);
  GyroMotor(-40, -60, -90, 0, false);
  dly_tsk(100000);
  GyroPivotTurn(0, 1, 0, false);
  dly_tsk(100000);
  tGyroMotor(1, 80, 30, 30, 650, 0, 0, true);
  Object4floorPiled();

  //赤のHouse element 4階建て完成

  //南側の黄色のHouse elementに移動
  tGyroMotor(-1, 95, 30, 50, 600, 0, 3, true);
  pup_motor_set_power(armB, -10);
  GyroMotorOn(true);
  dly_tsk(400000);
  GyroMotorOn(false);
  MotorStop(1);
  GyroReset();
  OffsetGyro(true);

  //南側の黄色のHouse element回収
  ArmOpen(220);
  dly_tsk(100000);
  GyroSpinTurn(-60, 3, false);
  GyroPivotTurn(-90, 1, 0, false);
  dly_tsk(100000);
  ADJMotor(-50, -80, 0, true);
  dly_tsk(100000);
  ADJBasePower = 50;
  ADJMotorOn(true);
  while (pup_color_sensor_reflection(UnderColorSensor) < 70)
  {
    dly_tsk(1);
  }
  while (pup_color_sensor_reflection(UnderColorSensor) > 35)
  {
    dly_tsk(1);
  }
  ADJMotorOn(false);
  tGyroMotor(1, 70, 30, 40, 420, -90, 3, true);
  ObjectCatch(180, -90, true, 2);
  tGyroMotor(-1, 70, 30, 30, 300, -90, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(0, 0, false);
  dly_tsk(100000);
  SecMotor(-50, 0.4, 1);

  //黄色のHouse elementをHouse Areaで4階建てにする
  ADJMotor(35, 50, 0, true);
  ArmLock(false);
  ArmOpen(200);
  ADJMotor(-35, -20, 0, true);
  ArmUpdown(2);
  dly_tsk(100000);
  ADJMotor(40, 110, 3, false);
  pup_motor_set_power(armB, -10);
  ADJMotor(40, 130, 0, false);
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
  GyroMotor(50, 100, 0, 3, true);
  pup_motor_set_power(armF, -70);
  tGyroMotor(1, 95, 50, 30, 750, 0, 0, false);
  dly_tsk(100000);
  GyroSpinTurn(-190, 0, false);
  GyroSpinTurn(-180, 0, false);
  dly_tsk(100000);
  Object4floorPiled();

  //黄色のHouse element 4階建て完成

  //北側の青・緑のHouse elementに移動
  dly_tsk(100000);
  tGyroMotor(-1, 90, 30, 50, 650, -180, 3, true);
  GyroMotorOn(true);
  dly_tsk(400000);
  GyroMotorOn(false);
  MotorStop(1);
  GyroReset();
  OffsetGyro(true);

  //北側の青・緑のHouse elementを回収
  ArmOpen(220);
  dly_tsk(100000);
  tGyroMotor(1, 50, 30, 30, 170, 0, 0, true);
  dly_tsk(100000);
  GyroPivotTurn(-90, 1, 0, false);
  ArmUpdown(0);
  ObjectCatch(360, -90, true, 0);

  //西半分にあるDebris elementをCollection Areaに運搬
  tGyroMotor(1, 60, 30, 30, 270, -90, 0, true);
  ArmClose();
  GyroMotor(-40, -90, -90, 0, true);
  ArmOpen(80);
  dly_tsk(100000);
  GyroSpinTurn(0, 0, false);
  ArmOpen(200);
  tGyroMotor(1, 95, 30, 30, 1480, 0, 0, true);
  ArmOpen(180);
  GyroPivotTurn(-90, -1, 0, false);
  dly_tsk(100000);
  tGyroMotor(1, 80, 30, 50, 300, -90, 3, true);
  GyroMotorOn(true);
  while (pup_color_sensor_reflection(UnderColorSensor) < 70)
  {
    dly_tsk(1);
  }
  GyroMotorOn(false);
  MotorStop(0);
  ArmOpen(220);
  tGyroMotor(-1, 60, 30, 30, 150, -90, 0, true);
  GyroSpinTurn(-180, 0, false);
  SecMotor(-50, 0.8, 1);
  GyroReset();
  OffsetGyro(true);

  //西半分のDebris element 運搬完了

  //西にあるWater Pipeを修理
  tGyroMotor(1, 70, 30, 30, 390, 0, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(90, 0, false);
  ArmOpen(325);
  tGyroMotor(1, 80, 30, 30, 400, 90, 3, true);
  GyroMotorOn(true);
  dly_tsk(500000);
  GyroMotorOn(false);
  MotorStop(0);
  GyroMotor(-40, -40, 90, 0, true);
  pup_motor_set_power(armB, 100);
  dly_tsk(200000);
  ArmUpdown(3);
  pup_motor_set_power(armB, -10);
  GyroMotor(30, 15, 90, 0, true);
  while (pup_motor_get_count(armB) >= 10)
  {
    if (pup_motor_is_stalled(armB))
    {
      pup_motor_set_power(armB, 60);
      dly_tsk(300000);
      pup_motor_set_power(armB, -40);
    }
  }

  //西のWater Pipe 修理完了

  //北東にある青・緑のHouse elementに移動
  tGyroMotor(-1, 95, 30, 30, 800, 90, 0, true);
  GyroPivotTurn(0, 1, 0, false);
  tGyroMotor(-1, 70, 30, 50, 300, 3, 0, true);
  GyroMotorOn(true);
  dly_tsk(300000);
  GyroMotorOn(false);
  GyroReset();
  OffsetGyro(true);
  dly_tsk(100000);
  tGyroMotor(1, 65, 30, 30, 330, 0, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(90, 0, false);
  dly_tsk(100000);
  tGyroMotor(-1, 95, 30, 30, 1550, 90, 0, true);
  ArmOpen(200);
  GyroMotor(30, 20, 90, 0, true);
  ArmUpdown(2);
  tGyroMotor(-1, 65, 30, 30, 250, 90, 0, false);
  GyroMotorOn(false);
  MotorStop(0);
  dly_tsk(100000);
  GyroSpinTurn(180, 0, false);
  dly_tsk(100000);
  pup_motor_set_power(armB, -10);
  tGyroMotor(-1, 95, 30, 30, 1350, 180, 3, true);
  GyroMotorOn(true);
  dly_tsk(300000);
  GyroMotorOn(false);
  MotorStop(1);
  GyroReset();
  OffsetGyro(true);
  dly_tsk(100000);
  
}
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

void BridgeStart()
{
  //南側の赤・黄のHouse element回収
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

  //北側の赤のHouse elementに移動
  GyroMotor(-50, -80, -90, 0, true);
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
  ArmLock(false);
  ArmOpen(190);
  GyroMotor(30, 50, 90, 0, true);
  //ADJMotor(40, 50, 0, true);
  dly_tsk(100000);
  tGyroMotor(-1, 90, 30, 30, 700, 90, 0, true);
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
  dly_tsk(100000);
  GyroSpinTurn(-60, 3, false);
  GyroPivotTurn(-90, 1, 0, false);
  dly_tsk(100000);

  //北側の赤のHouse elementを回収
  ArmOpen(220);
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
  GyroMotor(-40, -50, -90, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(0, 0, false);
  dly_tsk(100000);
  tGyroMotor(1, 95, 30, 30, 780, 0, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(-90, 0, false);
  dly_tsk(100000);
  tGyroMotor(1, 85, 30, 30, 740, -90, 0, true);
  dly_tsk(100000);
  Object4floorPiled();

  //赤のHouse element 4階建て完成

  //北側の黄色のHouse elementに移動
  GyroMotor(-50, -60, -90, 3, true);
  GyroMotorOn(true);
  while (pup_color_sensor_reflection(UnderColorSensor) < 60)
  {
    dly_tsk(1);
  }
  pup_motor_set_power(armB, -10);
  tGyroMotor(-1, 80, 50, 30, 560, -90, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(0, 0, false);
  dly_tsk(100000);
  tGyroMotor(-1, 95, 30, 50, 750, 0, 3, true);
  GyroMotorOn(true);
  dly_tsk(400000);
  GyroMotorOn(false);
  MotorStop(1);
  GyroReset();
  OffsetGyro(true);
  dly_tsk(100000);
  GyroSpinTurn(-60, 3, false);
  GyroPivotTurn(-90, 1, 0, false);

  //北側の黄色のHouse elementを回収
  ArmOpen(220);
  dly_tsk(100000);
  tGyroMotor(1, 60, 30, 50, 250, -90, 3, true);
  ObjectCatch(410, -90, false, 2);
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
  tGyroMotor(1, 85, 50, 30, 550, 0, 0, true);
  ArmLock(true);
  Object4floorPiled();

  //黄色のHouse element 4階建て完成

  //西半分にあるDebris elementをCollection Areaに運搬
  tGyroMotor(-1, 95, 30, 50, 640, 0, 3, true);
  SecMotor(-50, 0.4, 1);
  GyroReset();
  OffsetGyro(true);
  ArmUpdown(0);
  GyroMotor(40, 50, 0, 0, true);
  //ADJMotor(40, 70, 0, true);
  GyroSpinTurn(-90, 0, false);
  ArmOpen(220);
  dly_tsk(100000);
  tGyroMotor(1, 90, 30, 30, 580, -90, 0, true);
  ArmClose();
  GyroMotor(-40, -50, -90, 0, true);
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
  pup_motor_set_power(armB, -20);

  //西のWater Pipe 修理完了

  //南側の青・緑のHouse elementに移動
  tGyroMotor(-1, 95, 30, 30, 800, 90, 0, true);
  GyroPivotTurn(0, 1, 0, false);
  tGyroMotor(-1, 70, 30, 50, 300, 3, 0, true);
  GyroMotorOn(true);
  dly_tsk(300000);
  GyroMotorOn(false);
  GyroReset();
  OffsetGyro(true);

  //南側の青・緑のHouse elementを回収
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
  tADJMotor(1, 95, 50, 30, 750, 3, true);
  ObjectCatch(240, -90, true, 2);
  
  //北東にある青・緑のHouse elementに移動
  dly_tsk(100000);
  GyroSpinTurn(0, 0, false);
  GyroMotor(40, 50, 0, 3, true);
  ADJBasePower = 60;
  GyroMotorOn(true);
  while (pup_color_sensor_reflection(UnderColorSensor) < 70)
  {
    dly_tsk(1);
  }
  while (pup_color_sensor_reflection(UnderColorSensor) > 20)
  {
    dly_tsk(1);
  }
  GyroMotor(40, 50, 0, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(90, 0, false);
  dly_tsk(100000);
  tGyroMotor(-1, 60, 30, 30, 350, 90, 0, true);
  ArmLock(false);
  ArmOpen(200);
  tGyroMotor(-1, 60, 30, 30, 200, 90, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(180, 0, false);
  dly_tsk(100000);
  tGyroMotor(-1, 95, 30, 30, 1350, 180, 3, true);
  GyroMotorOn(true);
  dly_tsk(300000);
  GyroMotorOn(false);
  MotorStop(1);
  GyroReset();
  OffsetGyro(true);
  dly_tsk(100000);
}
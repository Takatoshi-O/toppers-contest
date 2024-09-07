/*
 * Copyright (c) 2023 Afrel Co., Ltd. 
 * All rights reserved
 */
#include <stdlib.h>
#include <kernel.h>

#include <spike/hub/system.h>

#include <WRO.h>

#include "kernel_cfg.h"

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

#include "Include/arm.h"
#include "Include/colorSensor.h"
#include "Include/gyro.h"
#include "Include/motor.h"
#include "Include/def.h"
#include "Include/system.h"
#include "Include/objectMove.h"

#include <pbio/color.h>


pup_motor_t *motorL;             //左側モーターを使う変数 left > right
pup_motor_t *motorR;             //右側モーターを使う変数 left > right
pup_motor_t *armF;               //前のアームを使う変数 open + / close -
pup_motor_t *armB;               //後ろのアームを使う変数 up + / down -
pup_device_t *ColorSensor;       //カラーセンサーを使う変数
pup_device_t *UnderColorSensor;  //カラーセンサーを使う変数 

const int startSide = 0;        //Fence Side : 0 / Bridge Side : 1


//メインプログラム
void Main(intptr_t exinf)
{
  //battery charge
  //exit(0);

  //setting
  hub_imu_init();
  motorL = pup_motor_get_device(PBIO_PORT_ID_B);
  motorR = pup_motor_get_device(PBIO_PORT_ID_F);
  armF = pup_motor_get_device(PBIO_PORT_ID_D);
  armB = pup_motor_get_device(PBIO_PORT_ID_E);
  ColorSensor = pup_color_sensor_get_device(PBIO_PORT_ID_A);
  UnderColorSensor = pup_color_sensor_get_device(PBIO_PORT_ID_C);

  pup_motor_setup(motorL, PUP_DIRECTION_COUNTERCLOCKWISE, true);
  pup_motor_setup(motorR, PUP_DIRECTION_CLOCKWISE, true);
  pup_motor_setup(armF, PUP_DIRECTION_CLOCKWISE, false);
  pup_motor_setup(armB, PUP_DIRECTION_CLOCKWISE, false);

  ButtonStart();
  
  //GyroSetting();
  //WarmUp();


  //WRO Japan Mission Charenge

  //Start Areaごとの分岐
  if (startSide == 1)
  {
    BridgeStart();
  }
  else
  {
    FenceStart();
  }

  //北東にある青・緑のHouse elementを回収
  GyroPivotTurn(95, 1, 0, false);
  GyroMotorBaseAngle = 95;
  ADJBasePower = -50;
  GyroMotorOn(true);
  while (pup_color_sensor_reflection(UnderColorSensor) < 60)
  {
    dly_tsk(1);
  }
  GyroMotorOn(false);
  MotorStop(0);
  ArmOpen(220);
  tGyroMotor(1, 80, 30, 30, 540, 95, 0, true);
  ArmClose();
  ArmLock(true);
  ArmUpdown(2);
  dly_tsk(100000);
  GyroSpinTurn(0, 0, false);
  dly_tsk(100000);
  pup_motor_set_power(armB, -10);
  SecMotor(-50, 0.5, 1);
  GyroReset();
  OffsetGyro(true);
  dly_tsk(100000);
  ArmUpdown(0);
  ArmLock(false);
  ObjectCatch(80, 0, true, 2);
  ArmLock(false);
  pup_motor_set_power(armF, 100);
  while (pup_motor_get_count(armF) < 400){}
  pup_motor_hold(armF);
  GyroMotor(-30, 90, 0, 0, false);
  ArmUpdown(2);
  dly_tsk(100000);
  GyroMotor(40, 210, 0, 3, false);
  pup_motor_set_power(armB, -10);
  GyroMotor(35, 240, 0, 0, false);
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

  //東にあるWater Pipeを修理
  tGyroMotor(1, 70, 30, 30, 375, 0, 0, true);
  GyroMotor(-40, -30, 0, 0, true);
  GyroPivotTurn(90, -1, 0, false);
  ArmOpen(325);
  GyroMotor(50, 100, 90, 3, true);
  ADJBasePower = 30;
  GyroMotorOn(true);
  dly_tsk(500000);
  GyroMotorOn(false);
  MotorStop(0);
  GyroMotor(-40, -40, 90, 0, true);
  pup_motor_set_power(armB, 100);
  dly_tsk(200000);
  ArmUpdown(3);
  ADJMotor(40, 10, 0, true);
  pup_motor_set_power(armB, -20);
  while (pup_motor_get_count(armB) >= 10)
  {
    if (pup_motor_is_stalled(armB))
    {
      pup_motor_set_power(armB, 60);
      dly_tsk(300000);
      pup_motor_set_power(armB, -40);
    }
  }
  GyroMotor(-50, -90, 90, 0, true);

  //東半分にあるDebris elementを回収
  GyroSpinTurn(0, 0, false);
  dly_tsk(100000);
  GyroMotor(50, 150, 0, 0, true);
  ArmOpen(130);
  dly_tsk(100000);
  GyroSpinTurn(-90, 0, false);
  dly_tsk(100000);
  ArmOpen(220);
  tGyroMotor(1, 70, 30, 30, 300, -90, 0, true);
  dly_tsk(100000);
  GyroPivotTurn(-55, 1, 0, false);
  dly_tsk(100000);
  tGyroMotor(1, 95, 30, 30, 1180, -55, 0, true);
  ArmOpen(325);
  GyroPivotTurn(-90, 1, 0, false);
  dly_tsk(100000);
  GyroMotor(-50, -100, -90, 0, true);
  dly_tsk(100000);
  GyroSpinTurn(90, 0, false);
  dly_tsk(100000);
  tGyroMotor(1, 60, 30, 40, 350, 90, 3, true);
  pup_motor_set_power(armF, -70);
  GyroMotor(40, 450, 90, 0, false);
  ArmClose();
  ArmLock(true);
  
  //1階になるHouse elementの色によって条件分岐
  if (GetColor(ColorSensor) == 'g')
  {
    //緑・青のHouse elementを緑のHouse Areaで4階建てにする
    GyroMotor(-40, -90, 90, 0, true);
    dly_tsk(100000);
    GyroSpinTurn(180, 0, false);
    tGyroMotor(1, 70, 30, 30, 420, 180, 0, true);
    Object4floorPiled();

    //緑・青のHouse element 4階建て完成

    //回収したDebris elementをCollection Areaに運搬
    tGyroMotor(-1, 65, 30, 30, 320, 180, 0, true);
    pup_motor_set_power(armB, -20);
    GyroSpinTurn(270, 0, false);
    dly_tsk(100000);
    tGyroMotor(1, 95, 30, 50, 2000, 270, 0, true);
    GyroPivotTurn(320, 1, 0, false);

    //東半分のDebris element 運搬完了
  }
  else
  {
    //緑・青のHouse elementを青のHouse Areaで4階建てにする
    tGyroMotor(-1, 70, 30, 30, 570, 90, 0, true);
    GyroPivotTurn(180, 1, 0, false);
    tGyroMotor(1, 65, 30, 30, 330, 180, 0, true);
    Object4floorPiled();

    //緑・青のHouse element 4階建て完成

    //回収したDebris elementをCollection Areaに運搬
    tGyroMotor(-1, 65, 30, 30, 320, 180, 0, true);
    pup_motor_set_power(armB, -20);
    GyroSpinTurn(270, 0, false);
    dly_tsk(100000);
    tGyroMotor(1, 95, 30, 50, 1620, 270, 0, true);
    GyroPivotTurn(320, 1, 0, false);

    //東半分のDebris element 運搬完了
  }

  //プログラム終了

  exit(0);
}

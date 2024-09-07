#include <stdlib.h>
#include <kernel.h>

#include <math.h>

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

#include "../general_var.h"
#include "../Include/gyro.h"
#include "../Include/motor.h"

#include <pbio/color.h>

#include "kernel_cfg.h"

float ang_v[3] = {0};                  //ジャイロセンサーの角度で使う配列

#define GYRO_Y_DIFF 2.046008682f
#define GYRO_Y_ADJ (GYRO_Y_DIFF / 1000)
#define GYROTURN_ADJ 86

//ジャイロセンサーの角度計測用タスク(修正値なし)
void gyro_monitor(intptr_t exinf)
{
  float ang_raw[3];
  hub_imu_get_angular_velocity(ang_raw);
  ang_v[0] -= ang_raw[0] * 0.001;
  ang_v[1] -= ang_raw[1] * 0.001;
  ang_v[2] -= ang_raw[2] * 0.001;
}

//ジャイロセンサーの角度計測用タスク(修正値あり)
void OffsetGyroTsk(intptr_t exinf)
{
  float ang_raw[3];
  hub_imu_get_angular_velocity(ang_raw);
  ang_v[0] -= ang_raw[0] * 0.001;
  ang_v[1] -= (ang_raw[1] * 0.001) + GYRO_Y_ADJ;
  ang_v[2] -= ang_raw[2] * 0.001;

}

//周期ハンドラ "OFFSET_GYRO_CYC" の起動 / 停止用
//Start "OFFSET_GYRO_CYC"
void OffsetGyro(bool TaskStart)
{
  if (TaskStart)
  {
    sta_cyc(OFFSET_GYRO_CYC);
  }
  else
  {
    stp_cyc(OFFSET_GYRO_CYC);
  }
  
}

//ジャイロセンサーをリセット
void GyroReset()
{
  stp_cyc(OFFSET_GYRO_CYC);
  hub_imu_init();
  dly_tsk(1000);
  for (int i = 0; i < 3; i++)
  {
    ang_v[i] = 0;
  }
}

//ジャイロセンサーの修正値取得
void GyroSetting()
{
  float degOrigin = 0;
  int degDisplay;
  hub_button_t pressed;

  dly_tsk(5000000);
  for (int i = 0; i < 5; i++)
  {
    for (int i = 0; i < 3; i++)
    {
      ang_v[i] = 0;
    }
    hub_imu_init();
    dly_tsk(100000);
    sta_cyc(GYRO_CYC);
    dly_tsk(3000000);
    stp_cyc(GYRO_CYC);
    degOrigin += ang_v[1] / 3;
  }
  

  hub_light_on_color(PBIO_COLOR_ORANGE);
  degOrigin /= 5;
  degOrigin *= 10;
  for (int i = 0; i < 5; i++)
  {
    degDisplay = degOrigin;
    hub_display_number(degDisplay);
    degOrigin -= degDisplay;
    degOrigin *= 100;
    while (!(pressed&HUB_BUTTON_CENTER))
    {
      hub_button_is_pressed(&pressed);
    }
    while (pressed&HUB_BUTTON_CENTER)
    {
      hub_button_is_pressed(&pressed);
    }
  }

  exit(0);
}

//未使用
/*
void GyroTurn(float Deg, int LeftPower, int RightPower, int stop, bool reset)
{
  float deg = 0;

  if (reset == true)
  {
    GyroReset();
  }

  sta_cyc(OFFSET_GYRO_CYC);
  while (abs(deg) <= Deg * GYROTURN_ADJ / 90)
  {
    deg = ang_v[1];
    pup_motor_set_power(motorL, LeftPower);
    pup_motor_set_power(motorR, RightPower);
  }
  
  MotorStop(stop);
}
*/

//ジャイロセンサーの角度を利用したスピンターン
//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//Deg = + : Left, - : Right
void GyroSpinTurn(float Deg, int stop, bool reset)
{
  float deg = 0;
  int powerAdj, direction;

  if (reset == true)
  {
    GyroReset();
  }
  deg = ang_v[1];
  if ((Deg - deg) > 0)
  {
    direction = 1;
  }
  else
  {
    direction = -1;
  }
  
  sta_cyc(OFFSET_GYRO_CYC);
  while ((Deg - deg) * direction > 0)
  {
    deg = ang_v[1];
    powerAdj = ((Deg - deg) * direction - 10) * 0.5; 
    if ((30 + powerAdj) > 70)
    {
      powerAdj = 30;
    }
    pup_motor_set_power(motorR, (30 + powerAdj) * direction);
    pup_motor_set_power(motorL, (-30 - powerAdj) * direction);
  }
  
  MotorStop(stop);
}

//ジャイロセンサーの角度を利用したピポットターン
//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//Deg = + : Left, - : Right
//UseMotor = -1 : Left / 1 : Right
void GyroPivotTurn(float Deg, int UseMotor, int stop, bool reset)
{
  float deg = 0;
  int powerAdj, direction;
  pup_motor_t *motor;

  if (reset == true)
  {
    GyroReset();
  }
  deg = ang_v[1];
  if ((Deg - deg) > 0)
  {
    direction = 1;
  }
  else
  {
    direction = -1;
  }
  if (UseMotor == 1)
  {
    motor = motorR;
    pup_motor_set_power(motorL, 0);
  }
  else
  {
    motor = motorL;
    pup_motor_set_power(motorR, 0);
  }
  
  sta_cyc(OFFSET_GYRO_CYC);
  while ((Deg - deg) * direction > 0)
  {
    deg = ang_v[1];
    powerAdj = ((Deg - deg) * direction - 10) * 0.8f; 
    if ((40 + powerAdj) > 75)
    {
      powerAdj = 35;
    }
    pup_motor_set_power(motor, (40 + powerAdj) * direction * UseMotor);
  }

  MotorStop(stop);
}
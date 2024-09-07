#include <stdlib.h>
#include <kernel.h>

#include <math.h>

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

#include "../general_var.h"
#include "../Include/motor.h"

#include <pbio/color.h>

#define ADJMOTOR_GAIN 0.7f
#define MOTOR_GAIN 1.00000f
int ADJBasePower = 0;
float ADJPowerGain = 0;
int GyroMotorBaseAngle = 0;
int BeforeRotation = 0;
int NowRotation;
double MotorSin, MotorCos, BeforeMotorSin;
double Rad;
int ADJleftAngle, ADJrightAngle, ADJGain;

//駆動用モータ停止用
//Mode = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void MotorStop(int Mode)
{
  if (Mode == 0)
  {
    pup_motor_hold(motorL);
    pup_motor_hold(motorR);
    pup_motor_set_power(motorL, 0);
    pup_motor_set_power(motorR, 0);
  }
  else if (Mode == 1)
  {
    pup_motor_brake(motorL);
    pup_motor_brake(motorR);
    pup_motor_set_power(motorL, 0);
    pup_motor_set_power(motorR, 0);
  }
  else if (Mode == 2)
  {
    pup_motor_stop(motorL);
    pup_motor_stop(motorR);
    pup_motor_set_power(motorL, 0);
    pup_motor_set_power(motorR, 0);
  }
  else if (Mode == 3)
  {
    
  }
  
}

//駆動用モーター制御
//Mode = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void BaseMotor(int LeftPower, int RightPower, int MotorAngle, int Stop, bool Reset)
{
  int leftAngle, rightAngle, direction;
  if (Reset)
  {
    pup_motor_reset_count(motorL);
    pup_motor_reset_count(motorR);
  }
  pup_motor_set_power(motorL, LeftPower);
  pup_motor_set_power(motorR, RightPower);
  if (LeftPower >= 0)
  {
    direction = 1;
  }
  else
  {
    direction = -1;
  }
  while (true)
  {
    leftAngle = pup_motor_get_count(motorL);
    rightAngle = pup_motor_get_count(motorR);
    if ((MotorAngle - leftAngle) * direction <= 0 && (MotorAngle - rightAngle) * direction <= 0)
    {
      break;
    }
  }
  
  MotorStop(Stop);
}

//周期ハンドラ用タスク
//モーターエンコーダーによる同期制御
void ADJMotorOnTsk(intptr_t exinf)
{
  ADJleftAngle = pup_motor_get_count(motorL);
  ADJrightAngle = pup_motor_get_count(motorR);
  ADJGain = (ADJleftAngle - ADJrightAngle) * ADJMOTOR_GAIN;
  if (abs(ADJGain) > 20)
  {
    pup_motor_set_power(motorL, ADJBasePower);
    pup_motor_set_power(motorR, ADJBasePower);
    return;
  }
  ADJPowerGain = 1 + 1 - ((abs(ADJBasePower) - 40) / 50);
  pup_motor_set_power(motorR, ADJBasePower);
  pup_motor_set_power(motorL, ADJBasePower - (ADJGain * ADJPowerGain));
}

//周期ハンドラ "ADJ_ON_CYC" の起動 / 停止用
//Start "ADJ_ON_CYC"
void ADJMotorOn(bool TaskStart)
{
  if (TaskStart)
  {
    sta_cyc(ADJ_ON_CYC);
  }
  else
  {
    stp_cyc(ADJ_ON_CYC);
  }
  
}

//周期ハンドラ用タスク
//ジャイロセンサーの角度による同期制御
void GyroMotorOnTsk(intptr_t exinf)
{
  float ang_raw[3];
  hub_imu_get_angular_velocity(ang_raw);
  //ADJGain = ((GyroMotorBaseAngle - ang_v[1]) * 6.5f) - (ang_raw[1] * 0.2 * (ADJBasePower / abs(ADJBasePower)));
  ADJGain = (GyroMotorBaseAngle - ang_v[1]) * 6.5f;
  if (abs(ADJGain) > 30)
  {
    pup_motor_set_power(motorL, ADJBasePower);
    pup_motor_set_power(motorR, ADJBasePower);
    return;
  }
  //ADJPowerGain = 1 + 1 - ((abs(ADJBasePower) - 40) / 50);
  //pup_motor_set_power(motorR, ADJBasePower + (ADJGain * ADJPowerGain / 2));
  //pup_motor_set_power(motorL, ADJBasePower - (ADJGain * ADJPowerGain / 2));
  pup_motor_set_power(motorR, ADJBasePower + (ADJGain / 2));
  pup_motor_set_power(motorL, ADJBasePower - (ADJGain / 2));
}

//周期ハンドラ "GYRO_MOTOR_ON_CYC" の起動 / 停止用
//Start "GYRO_MOTOR_ON_CYC"
void GyroMotorOn(bool TaskStart)
{
  if (TaskStart)
  {
    sta_cyc(GYRO_MOTOR_ON_CYC);
  }
  else
  {
    stp_cyc(GYRO_MOTOR_ON_CYC);
  }
  
}


//未使用
void TriMotorReset()
{
  MotorSin = 0;
  MotorCos = 0;
}


//未使用
//周期ハンドラ用タスク
//ジャイロセンサーの角度とモーターエンコーダーによる同期制御
void TriMotorOnTsk(intptr_t exinf)
{
  float ang_raw[3];
  NowRotation = (pup_motor_get_count(motorL) + pup_motor_get_count(motorR)) / 2;
  hub_imu_get_angular_velocity(ang_raw);
  Rad = (ang_v[1] - GyroMotorBaseAngle) / 180 * PI;
  MotorSin += (NowRotation - BeforeRotation) * sin(Rad);
  MotorCos += (NowRotation - BeforeRotation) * cos(Rad);
  //ADJGain = (MotorSin * 2) - (ang_raw[1] * 0.2 * (ADJBasePower / abs(ADJBasePower)));
  ADJGain = MotorSin * 0.1;
  
  //if (abs(ADJGain) > 50)
  //{
  //  pup_motor_set_power(motorL, ADJBasePower);
  //  pup_motor_set_power(motorR, ADJBasePower);
  //  return;
  //}
  
  pup_motor_set_power(motorR, ADJBasePower);
  pup_motor_set_power(motorL, ADJBasePower + (ADJGain * (ADJBasePower / abs(ADJBasePower))));
  BeforeRotation = NowRotation;
}

//未使用
//周期ハンドラ "TRI_MOTOR_ON_CYC" の起動 / 停止用
//Start "TRI_MOTOR_ON_CYC"
void TriMotorOn(bool TaskStart)
{
  if (TaskStart)
  {
    sta_cyc(TRI_MOTOR_ON_CYC);
  }
  else
  {
    stp_cyc(TRI_MOTOR_ON_CYC);
  }
  
}

//駆動用モーター制御(モーターエンコーダーによる同期制御)
//Mode = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void ADJMotor(int MotorPower, int motorAngle, int Stop, bool Reset)
{
  int leftAngle, rightAngle, direction;

  if (Reset == true)
  {
    pup_motor_reset_count(motorL);
    pup_motor_reset_count(motorR);
  }
  if (MotorPower >= 0)
  {
    direction = 1;
  }
  else
  {
    direction = -1;
  }
  
  ADJBasePower = MotorPower;
  sta_cyc(ADJ_ON_CYC);
  while (true)
  { 
    leftAngle = pup_motor_get_count(motorL);
    rightAngle = pup_motor_get_count(motorR);
    if ((motorAngle - leftAngle) * direction <= 0 && (motorAngle - rightAngle) * direction <= 0)
    {
      break;
    }
  }
  stp_cyc(ADJ_ON_CYC);
  MotorStop(Stop);
}

//駆動用モーター制御(ジャイロセンサーの角度による同期制御)
//Mode = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void GyroMotor(int MotorPower, int motorAngle, int TargetAngle, int Stop, bool Reset)
{
  int leftAngle, rightAngle, direction;

  if (Reset == true)
  {
    pup_motor_reset_count(motorL);
    pup_motor_reset_count(motorR);
  }
  if (MotorPower >= 0)
  {
    direction = 1;
  }
  else
  {
    direction = -1;
  }
  
  ADJBasePower = MotorPower;
  GyroMotorBaseAngle = TargetAngle;
  sta_cyc(OFFSET_GYRO_CYC);
  sta_cyc(GYRO_MOTOR_ON_CYC);
  while (true)
  { 
    leftAngle = pup_motor_get_count(motorL);
    rightAngle = pup_motor_get_count(motorR);
    if ((motorAngle - ((leftAngle + rightAngle) / 2)) * direction <= 0)
    {
      break;
    }
  }
  stp_cyc(GYRO_MOTOR_ON_CYC);
  MotorStop(Stop);
}

//未使用
//駆動用モーター制御(ジャイロセンサーの角度とモーターエンコーダーによる同期制御)
//Mode = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void TriMotor(int MotorPower, int motorAngle, int TargetAngle, int Stop, bool Reset)
{
  int leftAngle, rightAngle, direction;

  if (Reset == true)
  {
    pup_motor_reset_count(motorL);
    pup_motor_reset_count(motorR);
    TriMotorReset();
    BeforeRotation = 0;
  }
  else
  {
    BeforeRotation = (pup_motor_get_count(motorL) + pup_motor_get_count(motorR)) / 2;
  }

  if (MotorPower >= 0)
  {
    direction = 1;
  }
  else
  {
    direction = -1;
  }
  
  ADJBasePower = MotorPower;
  GyroMotorBaseAngle = TargetAngle;
  sta_cyc(OFFSET_GYRO_CYC);
  sta_cyc(TRI_MOTOR_ON_CYC);
  while ((motorAngle - NowRotation) * direction >= 0)
  { 
    dly_tsk(1);
  }
  stp_cyc(TRI_MOTOR_ON_CYC);
  MotorStop(Stop);
}

//駆動用モーター制御(モーターエンコーダーによる同期制御)
//台形制御あり
//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void tADJMotor(int FrontBack, int MotorPower, int StartPower, int FinPower, int MotorAngle, int Stop, bool Reset)
{
  float accel = 0.3f;
  int startPower, finPower, accelTime, accelAngle, brakeTime, brakeAngle, startAngle, moveAngle;
  finPower = FinPower;

  if (Reset == true)
  {
    pup_motor_reset_count(motorL);
    pup_motor_reset_count(motorR);
  }

  startPower = StartPower;
  if (startPower < 20)
  {
    startPower = 20;
  }
  startAngle = pup_motor_get_count(motorR);
  accelTime = (MotorPower - startPower) / accel;
  brakeTime = (MotorPower - finPower) * 0.9f / accel;

  ADJBasePower = startPower * FrontBack;
  sta_cyc(ADJ_ON_CYC);
  for (int i = 0; i < accelTime; i++)
  {
    ADJBasePower = (startPower + (accel * i)) * FrontBack;
    dly_tsk(1000);
  }

  accelAngle = abs(pup_motor_get_count(motorR) - startAngle);
  brakeAngle = accelAngle * ((MotorPower + finPower) * brakeTime) / ((MotorPower + startPower) * accelTime);
  moveAngle = MotorAngle - brakeAngle - 50;

  ADJMotor(MotorPower * FrontBack, moveAngle * FrontBack, 3, false);

  ADJBasePower = MotorPower * FrontBack;
  sta_cyc(ADJ_ON_CYC);
  for (int i = 0; i < brakeTime; i++)
  {
    ADJBasePower = (MotorPower - (accel * i)) * FrontBack;
    dly_tsk(1000);
  }
  stp_cyc(ADJ_ON_CYC);

  ADJMotor(finPower * FrontBack, MotorAngle * FrontBack, Stop, false);
}

//駆動用モーター制御(ジャイロセンサーの角度による同期制御)
//台形制御あり
//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//TargetAngle = + : Left / - : Right
void tGyroMotor(int FrontBack, int MotorPower, int StartPower, int FinPower, int MotorAngle, int TargetAngle, int Stop, bool Reset)
{
  float accel = 0.3f;
  int startPower, finPower, accelTime, accelAngle, brakeTime, brakeAngle, startAngle, moveAngle;
  finPower = FinPower;

  if (Reset == true)
  {
    pup_motor_reset_count(motorL);
    pup_motor_reset_count(motorR);
  }
  startPower = StartPower;
  if (startPower < 20)
  {
    startPower = 20;
  }
  startAngle = pup_motor_get_count(motorR);
  accelTime = (MotorPower - startPower) / accel;
  brakeTime = (MotorPower - finPower) * 0.9f / accel;

  GyroMotorBaseAngle = TargetAngle;
  ADJBasePower = startPower * FrontBack;
  sta_cyc(OFFSET_GYRO_CYC);
  sta_cyc(GYRO_MOTOR_ON_CYC);
  for (int i = 0; i < accelTime; i++)
  {
    ADJBasePower = (startPower + (accel * i)) * FrontBack;
    dly_tsk(1000);
  }

  accelAngle = abs(pup_motor_get_count(motorR) - startAngle);
  brakeAngle = accelAngle * ((MotorPower + finPower) * brakeTime) / ((MotorPower + startPower) * accelTime);
  moveAngle = MotorAngle - brakeAngle - 50;

  GyroMotor(MotorPower * FrontBack, moveAngle * FrontBack, TargetAngle, 3, false);

  GyroMotorBaseAngle = TargetAngle;
  ADJBasePower = MotorPower * FrontBack;
  sta_cyc(GYRO_MOTOR_ON_CYC);
  for (int i = 0; i < brakeTime; i++)
  {
    ADJBasePower = (MotorPower - (accel * i)) * FrontBack;
    dly_tsk(1000);
  }
  stp_cyc(GYRO_MOTOR_ON_CYC);

  GyroMotor(finPower * FrontBack, MotorAngle * FrontBack, TargetAngle, Stop, false);
}

//未使用
//駆動用モーター制御(ジャイロセンサーの角度とモーターエンコーダーによる同期制御)
//台形制御あり
//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//TargetAngle = + : Left / - : Right
void tTriMotor(int FrontBack, int MotorPower, int StartPower, int FinPower, int MotorAngle, int TargetAngle, int Stop, bool Reset)
{
  float accel = 0.2f;
  int startPower, finPower, accelTime, accelAngle, brakeTime, brakeAngle, startAngle, moveAngle;
  finPower = FinPower;

  if (Reset == true)
  {
    pup_motor_reset_count(motorL);
    pup_motor_reset_count(motorR);
    TriMotorReset();
    BeforeRotation = 0;
  }
  else
  {
    BeforeRotation = (pup_motor_get_count(motorL) + pup_motor_get_count(motorR)) / 2;
  }
  startPower = StartPower;
  if (startPower < 20)
  {
    startPower = 20;
  }
  startAngle = (pup_motor_get_count(motorL) + pup_motor_get_count(motorR)) / 2;
  accelTime = (MotorPower - startPower) / accel;
  brakeTime = (MotorPower - finPower) * 1.1f / accel;

  GyroMotorBaseAngle = TargetAngle;
  ADJBasePower = startPower * FrontBack;
  sta_cyc(OFFSET_GYRO_CYC);
  sta_cyc(TRI_MOTOR_ON_CYC);
  for (int i = 0; i < accelTime; i++)
  {
    ADJBasePower = (startPower + (accel * i)) * FrontBack;
    dly_tsk(1000);
  }

  accelAngle = abs(NowRotation - startAngle);
  brakeAngle = accelAngle * ((MotorPower + finPower) * brakeTime) / ((MotorPower + startPower) * accelTime);
  moveAngle = MotorAngle - brakeAngle - 50;

  ADJBasePower = MotorPower * FrontBack;
  while ((moveAngle * FrontBack - NowRotation) * FrontBack >= 0)
  { 
    dly_tsk(1);
  }
  
  //TriMotor(MotorPower * FrontBack, moveAngle * FrontBack, TargetAngle, 3, false);
  for (int i = 0; i < brakeTime; i++)
  {
    ADJBasePower = (MotorPower - (accel * i)) * FrontBack;
    dly_tsk(1000);
  }

  while ((MotorAngle * FrontBack - NowRotation) * FrontBack >= 0)
  { 
    dly_tsk(1);
  }
  stp_cyc(TRI_MOTOR_ON_CYC);
  MotorStop(Stop);

  //TriMotor(finPower * FrontBack, MotorAngle * FrontBack, TargetAngle, Stop, false);
}

//駆動用モーター制御(指定秒数まで制御)
//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void SecMotor(int MotorPower, float Sec, int stop)
{
  pup_motor_reset_count(motorL);
  pup_motor_reset_count(motorR);
  
  ADJBasePower = MotorPower;
  sta_cyc(ADJ_ON_CYC);
  dly_tsk(Sec * 1000000);
  stp_cyc(ADJ_ON_CYC);
  MotorStop(stop);
}
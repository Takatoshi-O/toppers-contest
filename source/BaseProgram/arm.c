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

#include "../general_var.h"
#include "../Include/arm.h"

#include <pbio/color.h>

//アームを上下
//Updown = 1 : floor1 Up / 2 ; floor2 Up / 3 : AllUp
//Updown = -1 : floor1 Down / -2 : floor2 Down / 0 : AllDown 
void ArmUpdown(int Updown)
{
  if (Updown == 0)
  {
    while (pup_motor_get_count(armB) >= 80)
    {
      pup_motor_set_power(armB, -20);
    }
    pup_motor_set_power(armB, -10);
    while (pup_motor_get_count(armB) >= 10){}
    dly_tsk(100000);
  }
  else if (Updown == 1)
  {
    pup_motor_set_power(armB, 70);
    while (pup_motor_get_count(armB) <= 35){}
    pup_motor_set_power(armB, 10);
    dly_tsk(1000);
    pup_motor_set_speed(armB, 0);
  }
  else if (Updown == 2)
  {
    pup_motor_set_power(armB, 80);
    while (pup_motor_get_count(armB) <= 65){}
    pup_motor_set_power(armB, 10);
    dly_tsk(1000);
    pup_motor_set_speed(armB, 0);
  }
  else if (Updown == 3)
  {
    pup_motor_set_power(armB, 80);
    while (pup_motor_get_count(armB) <= 85){}
    pup_motor_set_power(armB, 60);
    dly_tsk(300000);
    pup_motor_set_power(armB, 30);
    dly_tsk(100000);
  }
  else if (Updown == -1)
  {
    while (pup_motor_get_count(armB) >= 80)
    {
      pup_motor_set_power(armB, -20);
    }
    pup_motor_set_power(armB, -5);
    pup_motor_set_power(armB, 0);
    while (pup_motor_get_count(armB) >= 30){}
    pup_motor_hold(armB);
  }
  else if (Updown == -2)
  {
    while (pup_motor_get_count(armB) >= 80)
    {
      pup_motor_set_power(armB, -20);
    }
    pup_motor_set_power(armB, -5);
    pup_motor_set_power(armB, 0);
    while (pup_motor_get_count(armB) >= 55){}
    pup_motor_hold(armB);
  }
  
}

//オブジェクトを取得
//FrontArm reset
void ArmClose()
{
  pup_motor_set_power(armF, -70);
  dly_tsk(100000);
  while (pup_motor_get_speed(armF) < -1){}
  dly_tsk(100000);
  pup_motor_stop(armF);
}

//アーム開閉
//FrontArm Open / Close
void ArmOpen(int Angle)
{
  int NowAngle;
  NowAngle = pup_motor_get_count(armF);

  if (NowAngle > Angle)
  {
    pup_motor_set_power(armF, -60);
    while (pup_motor_get_count(armF) > Angle){}
    pup_motor_hold(armF);
  }
  else if (NowAngle < Angle)
  {
    pup_motor_set_power(armF, 60);
    while (pup_motor_get_count(armF) < Angle){}
    pup_motor_hold(armF);
  }
  else if (NowAngle == Angle)
  {
    /* code */
  }
  
}

//周期ハンドラ用タスク
void ArmLockTsk(intptr_t exinf)
{
  pup_motor_set_speed(armF, -180);
}

//周期ハンドラ "ARM_LOCK_CYC" の起動 / 停止用
//Start "ARM_LOCK_CYC"
void ArmLock(bool TaskStart)
{
  if (TaskStart)
  {
    sta_cyc(ARM_LOCK_CYC);
  }
  else
  {
    stp_cyc(ARM_LOCK_CYC);
    pup_motor_set_speed(armF, 0);
  }
  
}
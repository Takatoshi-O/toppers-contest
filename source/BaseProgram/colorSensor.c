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
#include "../Include/colorSensor.h"
#include "../Include/motor.h"

#include <pbio/color.h>

#define ADJMOTOR_GAIN 3.0f
#define REF_BLACK 8
#define REF_WHITE 97

//色を読み取る
//n = none / w = white / r = red / y = yellow / b = blue / g = green
char GetColor(pup_device_t *pdev)
{
  pup_color_hsv_t hsv;
  char colorName;
  hsv = pup_color_sensor_color(pdev, true);
  if (hsv.v == 0)
  {
    colorName = 'n';
  }
  else if (hsv.s == 0)
  {
    colorName = 'w';
  }
  else if (hsv.h == PBIO_COLOR_HUE_RED)
  {
    colorName = 'r';
  }
  else if (hsv.h == PBIO_COLOR_HUE_YELLOW)
  {
    colorName = 'y';
  }
  else if (hsv.h == PBIO_COLOR_HUE_GREEN)
  {
    colorName = 'g';
  }
  else if (hsv.h == PBIO_COLOR_HUE_BLUE)
  {
    colorName = 'b';
  }

  return colorName;
}

//ここ以下は未使用

/*
void RefSetting()
{
  int ref;
  while (true)
  {
    ref = pup_color_sensor_reflection(ColorSensor);
    hub_display_number(ref);
  }
  
}


int ADJRef(float UnADJref)
{
  int Reref;
  Reref = (UnADJref - REF_BLACK + 5) * 100 / (REF_WHITE - REF_BLACK);
  return Reref;
}


void AngleLinetrace (int MotorPower, int Ref, float Gain, int Side, int MotorAngle, int Stop)
{
  int ref;
  int beforeRef;
  int diff;
  int motorDig_L, motorDig_R;
    
  pup_motor_reset_count(motorL);
  pup_motor_reset_count(motorR);

  while (true)
  {
    ref = ADJRef(pup_color_sensor_reflection(ColorSensor));
    diff = ((Ref - ref) * Gain + (((Ref - ref) - beforeRef) ^ 2)) * Side; 
    beforeRef = Ref - ref;
    pup_motor_set_power(motorL, MotorPower - (diff / 2));
    pup_motor_set_power(motorR, MotorPower + (diff / 2));

    motorDig_L = pup_motor_get_count(motorL);
    motorDig_R = pup_motor_get_count(motorR);
    if (abs(motorDig_L) >= MotorAngle && abs(motorDig_R) >= MotorAngle)
    {
      break;
    }

  }

  MotorStop(Stop);
}


void RefMotor(int LeftPower, int RightPower, int Ref, bool BigSmall, int Stop)
{
  int ref;

  pup_motor_set_power(motorL, LeftPower);
  pup_motor_set_power(motorR, RightPower);

  while (true)
  {
    ref = ADJRef(pup_color_sensor_reflection(ColorSensor));
    
    if (BigSmall == true)
    {
      if (ref >= Ref)
      {
        break;
      }
    }
    else
    {
      if (ref <= Ref)
      {
        break;
      }
      
    }
    
  }
  
  MotorStop(Stop);
}


void RefADJMotor(int MotorPower, int Ref, bool BigSmall, int Stop, bool Reset)
{
  int ref;

  if (Reset)
  {
    pup_motor_reset_count(motorL);
    pup_motor_reset_count(motorR);
  }
  ADJBasePower = MotorPower;
  sta_cyc(ADJ_ON_CYC);
  while (true)
  {
    ref = ADJRef(pup_color_sensor_reflection(ColorSensor));
    
    if (BigSmall == true)
    {
      if (ref >= Ref)
      {
        break;
      }
    }
    else
    {
      if (ref <= Ref)
      {
        break;
      }
      
    }
  }
  stp_cyc(ADJ_ON_CYC);
  MotorStop(Stop);
}
*/
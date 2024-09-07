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
#include "../Include/system.h"

#include <pbio/color.h>

//Hubの左ボタンが押されるまで待機
void ButtonStart()
{
  hub_button_t pressed;
  while (!(pressed&HUB_BUTTON_LEFT))
  {
    hub_button_is_pressed(&pressed);
  }
}

//モーターを温める用
//warmup
void WarmUp()
{
  pup_motor_set_power(motorR, 100);
  pup_motor_set_power(motorL, 100);
  dly_tsk(50000000);
  pup_motor_hold(motorL);
  pup_motor_hold(motorR);

  exit(0);
}
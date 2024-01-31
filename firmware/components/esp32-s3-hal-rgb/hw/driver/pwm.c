#include "pwm.h"



static bool is_init = false;



bool pwmInit(void)
{
  is_init = true;

  logPrintf("[OK] pwmInit()\n");   
  return true;  
}

bool pwmIsInit(void)
{
  return is_init;
}

void pwmWrite(uint8_t ch, uint16_t pwm_data)
{
  if (ch >= HW_PWM_MAX_CH || is_init != true) 
    return;

  // i2cExpWrite(I2C_REG_LCD_BL_PWM, (uint8_t *)&pwm_data, 1);
}
 
uint16_t pwmRead(uint8_t ch)
{
  uint8_t ret = 0;

  if (ch >= HW_PWM_MAX_CH || is_init != true) 
    return 0;

  // i2cExpRead(I2C_REG_LCD_BL_PWM, (uint8_t *)&ret, 1);

  return (uint16_t)ret;
}

uint16_t pwmGetMax(uint8_t ch)
{
  return 255;
}
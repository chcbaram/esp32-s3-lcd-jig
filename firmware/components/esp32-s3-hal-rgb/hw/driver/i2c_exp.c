/*
 * i2c.c
 *
 *  Created on: 2020. 12. 30.
 *      Author: baram
 */


#include "i2c_exp.h"
#include "i2c.h"
#include "cli.h"





#ifdef _USE_HW_I2C_EXP

#ifdef _USE_HW_CLI
static void cliCmd(cli_args_t *args);
#endif


typedef struct
{
  I2cExpAddr_t addr;
  const char *p_name;
} i2c_exp_info_t;


#define I2C_EXP_NAME(name) #name

const i2c_exp_info_t i2c_exp_info[] = 
{
  {I2C_REG_VERSION,     I2C_EXP_NAME(I2C_REG_VERSION)},
  {I2C_REG_POWER_BTN,   I2C_EXP_NAME(I2C_REG_POWER_BTN)},
  {I2C_REG_CHARGE_FLAG, I2C_EXP_NAME(I2C_REG_CHARGE_FLAG)},
  {I2C_REG_TOUCH_OUT,   I2C_EXP_NAME(I2C_REG_TOUCH_OUT)},
  {I2C_REG_I2S_SD_MODE, I2C_EXP_NAME(I2C_REG_I2S_SD_MODE)},
  {I2C_REG_LCD_TS_RST,  I2C_EXP_NAME(I2C_REG_LCD_TS_RST)},
  {I2C_REG_LCD_BL,      I2C_EXP_NAME(I2C_REG_LCD_BL)},
  {I2C_REG_LCD_WR_CMD,  I2C_EXP_NAME(I2C_REG_LCD_WR_CMD)},
  {I2C_REG_LCD_WR_DAT,  I2C_EXP_NAME(I2C_REG_LCD_WR_DAT)},
  {I2C_REG_LCD_BL_PWM,  I2C_EXP_NAME(I2C_REG_LCD_BL_PWM)},

  {I2C_REG_BAT_ADC_UP,  I2C_EXP_NAME(I2C_REG_BAT_ADC_UP)},
  {I2C_REG_BAT_ADC_L,   I2C_EXP_NAME(I2C_REG_BAT_ADC_L)},
  {I2C_REG_BAT_ADC_H,   I2C_EXP_NAME(I2C_REG_BAT_ADC_H)},
};


static bool is_init = false;
static uint8_t i2c_ch = _DEF_I2C1;
static uint8_t i2c_addr = 0x0B;




bool i2cExpInit(void)
{
  bool ret = false;


  if (i2cIsBegin(i2c_ch) == true)
    ret = true;
  else
    ret = i2cBegin(i2c_ch, 400);
    
  if (i2cIsDeviceReady(i2c_ch, i2c_addr))
  {
    ret = true;    
  }

  logPrintf("[%s] i2cExpInit()\n", ret ? "OK":"NG");

#ifdef _USE_HW_CLI
  cliAdd("i2c-exp", cliCmd);
#endif

  is_init = true;
  return true;
}

bool i2cExpIsInit(void)
{
  return is_init;
}

bool i2cExpRead(I2cExpAddr_t addr, uint8_t *p_data, uint8_t length)
{
  bool ret;

  ret = i2cReadBytes(i2c_ch, i2c_addr, addr, p_data, length, 50);

  return ret;
}

bool i2cExpWrite(I2cExpAddr_t addr, uint8_t *p_data, uint8_t length)
{
  bool ret = false;

  for (int i=0; i<length; i++)
  {
    ret = i2cWriteByte(i2c_ch, i2c_addr, addr + i, p_data[i], 10);
    if (ret != true)
      break;
  }

  return ret;
}

#ifdef _USE_HW_CLI
void cliCmd(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    cliPrintf("i2c-exp addr : 0x%02X\n", i2c_addr);
    cliPrintf("i2c-exp init : %d\n", is_init);

    uint8_t reg_cnt;
    uint8_t reg_data;

    reg_cnt = sizeof(i2c_exp_info) / sizeof(i2c_exp_info_t);
    for (int i=0; i<reg_cnt; i++)
    {
      i2cExpRead(i2c_exp_info[i].addr, &reg_data, 1);

      cliPrintf("        %-20s 0x%02X - 0x%02X\n", i2c_exp_info[i].p_name, i2c_exp_info[i].addr, reg_data);
    }
    
    ret = true;  
  }

  if (args->argc == 3 && args->isStr(0, "write") == true)
  {
    uint8_t reg_cnt;
    uint8_t reg_addr;
    uint8_t reg_data;

    reg_addr = args->getData(1);
    reg_data = args->getData(2);

    reg_cnt = sizeof(i2c_exp_info) / sizeof(i2c_exp_info_t);

    if (reg_addr < reg_cnt)
    {
      if (i2cExpWrite(reg_addr, &reg_data, 1) == true)
        cliPrintf("i2cExpWrite 0x%02X : 0x%02X OK\n", reg_addr, reg_data);
      else
        cliPrintf("i2cExpWrite 0x%02X : 0x%02X Fail\n", reg_addr, reg_data);
    }
    else
    {
      cliPrintf("addr error\n");
    }
    
    ret = true;  
  }

  if (ret == false)
  {
    cliPrintf("i2c-exp info\n");
    cliPrintf("i2c-exp write addr data\n");
  }
}

#endif

#endif
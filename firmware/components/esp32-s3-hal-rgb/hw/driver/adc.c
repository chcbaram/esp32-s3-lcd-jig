/*
 * adc.c
 *
 *  Created on: 2022. 9. 18.
 *      Author: baram
 */


#include "adc.h"
#include "cli.h"


#ifdef _USE_HW_ADC
#include "i2c_exp.h"


#define lock()      xSemaphoreTake(mutex_lock, portMAX_DELAY);
#define unLock()    xSemaphoreGive(mutex_lock);


#ifdef _USE_HW_CLI
static void cliAdc(cli_args_t *args);
#endif


static SemaphoreHandle_t mutex_lock;
static bool is_init = false;





bool adcInit(void)
{

  mutex_lock = xSemaphoreCreateMutex();

  if (i2cExpIsInit() != true)
    return false;
    
  is_init = true;

  logPrintf("[OK] adcInit()\n");  

#ifdef _USE_HW_CLI
  cliAdd("adc", cliAdc);
#endif
  return true;
}

int32_t adcRead(uint8_t ch)
{
  int32_t adc_ret = 0;

  if (is_init == false) return 0;


  lock();
  uint8_t data[2];

  data[0] = 1;
  i2cExpWrite(I2C_REG_BAT_ADC_UP, data, 1);
  i2cExpRead(I2C_REG_BAT_ADC_L, data, 2);
  
  adc_ret = (data[1]<<8)|(data[0]<<0);
  unLock();

  return adc_ret;
}

int32_t adcRead8(uint8_t ch)
{
  return adcRead(ch)>>4;
}

int32_t adcRead10(uint8_t ch)
{
  return adcRead(ch)>>2;
}

int32_t adcRead12(uint8_t ch)
{
  return adcRead(ch)>>0;
}

int32_t adcRead16(uint8_t ch)
{
  return adcRead(ch)<<4;  
}

uint8_t adcGetRes(uint8_t ch)
{
  return 12;
}



#ifdef _USE_HW_CLI
void cliAdc(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    cliPrintf("adc init : %d\n", is_init);
    cliPrintf("adc res  : %d\n", adcGetRes(0));
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "show") == true)
  {
    while(cliKeepLoop())
    {
      for (int i=0; i<ADC_MAX_CH; i++)
      {
        cliPrintf("%04d ", adcRead(i));
      }
      cliPrintf("\n");
      delay(50);
    }
    
    ret = true;
  }


  if (ret != true)
  {
    cliPrintf("adc info\n");
    cliPrintf("adc show\n");
  }
}
#endif

#endif
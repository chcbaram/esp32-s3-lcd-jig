#include "gpio.h"

#ifdef _USE_HW_GPIO
#include "cli.h"
#include "driver/gpio.h"


typedef enum
{
  GPIO_HW,
  GPIO_I2C_EXP,
} GpioType_t;


typedef struct
{
  GpioType_t    type;
  uint32_t      pin;
  uint8_t       mode;
  bool          init_value;
  const char   *p_name;
} gpio_tbl_t;


static const gpio_tbl_t gpio_tbl[GPIO_MAX_CH] =
    {
      {GPIO_HW, (uint32_t)GPIO_NUM_1,     _DEF_INPUT,   _DEF_HIGH, "LCD_TP_INT"  },   // 0. LCD_TP_INT
      {GPIO_HW, (uint32_t)GPIO_NUM_38,    _DEF_OUTPUT,  _DEF_HIGH, "LCD_TP_RESET"},   // 1. LCD_TP_RESET      
      {GPIO_HW, (uint32_t)GPIO_NUM_4,     _DEF_OUTPUT,  _DEF_HIGH, "LCD_BLK"     },   // 2. LCD_BLK  
      {GPIO_HW, (uint32_t)GPIO_NUM_5,     _DEF_OUTPUT,  _DEF_HIGH, "LCD_RESET"   },   // 3. LCD_RESET  
    };

static uint8_t gpio_data[GPIO_MAX_CH];
static bool is_init = false;

#ifdef _USE_HW_CLI
static void cliGpio(cli_args_t *args);
#endif



bool gpioInit(void)
{
  bool ret = true;


  is_init = ret;

  for (int i=0; i<GPIO_MAX_CH; i++)
  {
    gpioPinMode(i, gpio_tbl[i].mode);
    gpioPinWrite(i, gpio_tbl[i].init_value);
  }

#ifdef _USE_HW_CLI
  cliAdd("gpio", cliGpio);
#endif

  return ret;
}

bool gpioPinMode(uint8_t ch, uint8_t mode)
{
  bool ret = true;


  if (ch >= GPIO_MAX_CH)
  {
    return false;
  }

  gpio_reset_pin(gpio_tbl[ch].pin);
  
  switch(mode)
  {
    case _DEF_INPUT:
      gpio_pullup_dis(gpio_tbl[ch].pin);
      gpio_pulldown_dis(gpio_tbl[ch].pin);
      gpio_set_direction(gpio_tbl[ch].pin, GPIO_MODE_INPUT);
      break;

    case _DEF_INPUT_PULLUP:
      gpio_pullup_en(gpio_tbl[ch].pin);
      gpio_pulldown_dis(gpio_tbl[ch].pin);
      gpio_set_direction(gpio_tbl[ch].pin, GPIO_MODE_INPUT);
      break;

    case _DEF_INPUT_PULLDOWN:
      gpio_pullup_dis(gpio_tbl[ch].pin);
      gpio_pulldown_en(gpio_tbl[ch].pin);
      gpio_set_direction(gpio_tbl[ch].pin, GPIO_MODE_INPUT);
      break;

    case _DEF_OUTPUT:
      gpio_pullup_dis(gpio_tbl[ch].pin);
      gpio_pulldown_dis(gpio_tbl[ch].pin);
      gpio_set_direction(gpio_tbl[ch].pin, GPIO_MODE_INPUT_OUTPUT);
      break;

    case _DEF_OUTPUT_PULLUP:
      gpio_pullup_en(gpio_tbl[ch].pin);
      gpio_pulldown_dis(gpio_tbl[ch].pin);
      gpio_set_direction(gpio_tbl[ch].pin, GPIO_MODE_OUTPUT);
      break;

    case _DEF_OUTPUT_PULLDOWN:
      gpio_pullup_dis(gpio_tbl[ch].pin);
      gpio_pulldown_en(gpio_tbl[ch].pin);
      gpio_set_direction(gpio_tbl[ch].pin, GPIO_MODE_OUTPUT);
      break;
  }

  return ret;
}

void gpioPinWrite(uint8_t ch, uint8_t value)
{
  if (ch >= GPIO_MAX_CH || is_init != true)
  {
    return;
  }

  gpio_set_level(gpio_tbl[ch].pin, value);
  gpio_data[ch] = value;
}

uint8_t gpioPinRead(uint8_t ch)
{
  uint8_t ret;

  if (ch >= GPIO_MAX_CH || is_init != true)
  {
    return false;
  }

  ret = gpio_get_level(gpio_tbl[ch].pin);
  gpio_data[ch] = ret;
  return ret;
}

void gpioPinToggle(uint8_t ch)
{
  if (ch >= GPIO_MAX_CH || is_init != true)
  {
    return;
  }

  gpio_data[ch] = !gpio_data[ch];
  gpioPinWrite(gpio_tbl[ch].pin, gpio_data[ch]);
}





#ifdef _USE_HW_CLI
void cliGpio(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    cliPrintf("is init : %d\n", is_init);
    for (int i=0; i<GPIO_MAX_CH; i++)
    {
      cliPrintf("%d %-16s - %d\n", i, gpio_tbl[i].p_name, gpioPinRead(i));
    }
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "show") == true)
  {
    while(cliKeepLoop())
    {
      for (int i=0; i<GPIO_MAX_CH; i++)
      {
        cliPrintf("%d", gpioPinRead(i));
      }
      cliPrintf("\n");
      delay(100);
    }
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "read") == true)
  {
    uint8_t ch;

    ch = (uint8_t)args->getData(1);

    while(cliKeepLoop())
    {
      cliPrintf("gpio read %d : %d\n", ch, gpioPinRead(ch));
      delay(100);
    }

    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "write") == true)
  {
    uint8_t ch;
    uint8_t data;

    ch   = (uint8_t)args->getData(1);
    data = (uint8_t)args->getData(2);

    gpioPinWrite(ch, data);

    cliPrintf("gpio write %d : %d\n", ch, data);
    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("gpio info\n");
    cliPrintf("gpio show\n");
    cliPrintf("gpio read ch[0~%d]\n", GPIO_MAX_CH-1);
    cliPrintf("gpio write ch[0~%d] 0:1\n", GPIO_MAX_CH-1);
  }
}
#endif


#endif
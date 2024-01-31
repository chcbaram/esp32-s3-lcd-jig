#ifndef MAIN_HW_HW_H_
#define MAIN_HW_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#include "uart.h"
#include "gpio.h"
#include "cli.h"
#include "log.h"
#include "cdc.h"
#include "pwm.h"
#include "nvs.h"
#include "button.h"
#include "sd.h"
#include "i2c.h"
#include "touch.h"
#include "lcdc.h"
#include "mixer.h"
#include "i2s.h"
#include "audio.h"
#include "buzzer.h"
#include "lcd.h"
#include "lvgl.h"
#include "jpegd.h"
#include "i2c_exp.h"
#include "adc.h"
#include "battery.h"


bool hwInit(void);


#ifdef __cplusplus
}
#endif

#endif /* MAIN_HW_HW_H_ */

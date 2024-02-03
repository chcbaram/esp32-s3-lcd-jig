#include "lcdc.h"
#include "lcd.h"
#include "cli.h"

#include "driver/gpio.h"
#include "hal/gpio_hal.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"



#define GPIO_LCD_DE    (GPIO_NUM_17)
#define GPIO_LCD_VSYNC (GPIO_NUM_18)
#define GPIO_LCD_HSYNC (GPIO_NUM_8)
#define GPIO_LCD_PCLK  (GPIO_NUM_16)

#define GPIO_LCD_R0    (GPIO_NUM_45)
#define GPIO_LCD_R1    (GPIO_NUM_0)
#define GPIO_LCD_R2    (GPIO_NUM_39)
#define GPIO_LCD_R3    (GPIO_NUM_40)
#define GPIO_LCD_R4    (GPIO_NUM_2)

#define GPIO_LCD_G0    (GPIO_NUM_12)
#define GPIO_LCD_G1    (GPIO_NUM_13)
#define GPIO_LCD_G2    (GPIO_NUM_14)
#define GPIO_LCD_G3    (GPIO_NUM_21)
#define GPIO_LCD_G4    (GPIO_NUM_47)
#define GPIO_LCD_G5    (GPIO_NUM_48)

#define GPIO_LCD_B0    (GPIO_NUM_3)
#define GPIO_LCD_B1    (GPIO_NUM_46)
#define GPIO_LCD_B2    (GPIO_NUM_9)
#define GPIO_LCD_B3    (GPIO_NUM_10)
#define GPIO_LCD_B4    (GPIO_NUM_11)


static bool lcdcISR(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx);


static esp_lcd_panel_handle_t h_panel = NULL;
static bool is_init = false;
static bool is_begin = false;
static bool (*p_call_func)(void) = NULL;


static uint32_t lcdc_width  = LCD_WIDTH;
static uint32_t lcdc_height = LCD_HEIGHT;




bool lcdcInit(void)
{
  is_init = true;

  return true;
}

bool lcdcBegin(uint16_t width, uint16_t height, uint8_t bus_width, uint32_t freq_mhz)
{
  esp_lcd_rgb_panel_config_t panel_config = 
  {
      .data_width             = 16,    
      .bits_per_pixel         = 16,
      .psram_trans_align      = 64,
      // .bounce_buffer_size_px  = height * 20,
      .num_fbs                = HW_LCDC_FB_CNT,
      .clk_src                = LCD_CLK_SRC_PLL160M,
      .disp_gpio_num          = GPIO_NUM_NC,
      .pclk_gpio_num          = GPIO_LCD_PCLK,
      .vsync_gpio_num         = GPIO_LCD_VSYNC,
      .hsync_gpio_num         = GPIO_LCD_HSYNC,
      .de_gpio_num            = GPIO_LCD_DE,
      .data_gpio_nums = 
      {
          GPIO_LCD_B0, GPIO_LCD_B1, GPIO_LCD_B2, GPIO_LCD_B3, GPIO_LCD_B4,         
          GPIO_LCD_G0, GPIO_LCD_G1, GPIO_LCD_G2, GPIO_LCD_G3, GPIO_LCD_G4, GPIO_LCD_G5,
          GPIO_LCD_R0, GPIO_LCD_R1, GPIO_LCD_R2, GPIO_LCD_R3, GPIO_LCD_R4,
      },
      .timings = {
          .pclk_hz = freq_mhz * 1000 * 1000,
          #if HW_LCD_ROTATE == 1
          .h_res = height,
          .v_res = width,
          #else
          .h_res = width,
          .v_res = height,
          #endif
          // The following parameters should refer to LCD spec
          .hsync_back_porch   = 50, // 50
          .hsync_front_porch  = 10, // 10
          .hsync_pulse_width  = 58,
          .vsync_back_porch   = 20, // 20
          .vsync_front_porch  = 10, // 10
          .vsync_pulse_width  = 58,      
      },
      .flags.fb_in_psram       = true, // allocate frame buffer in PSRAM
      .flags.refresh_on_demand = false,
      .flags.bb_invalidate_cache = 0,
  };
  
  if (esp_lcd_new_rgb_panel(&panel_config, &h_panel) != ESP_OK)
  {
    return false;
  }

  esp_lcd_rgb_panel_event_callbacks_t cbs = 
  {
    .on_vsync = lcdcISR,
  };
  if (esp_lcd_rgb_panel_register_event_callbacks(h_panel, &cbs, NULL) != ESP_OK)
  {
    return false;
  }
  if (esp_lcd_panel_reset(h_panel) != ESP_OK)
  {
    return false;
  }
  
  if (esp_lcd_panel_init(h_panel) != ESP_OK)
  {
    return false;
  }

  #if HW_LCD_ROTATE == 1
  esp_lcd_panel_swap_xy(h_panel, true);
  esp_lcd_panel_mirror(h_panel, true, false);
  #endif

  lcdc_width  = width;
  lcdc_height = height;

  is_begin = true;
  
  return true;
}

bool lcdcSetCallBack(bool (*p_func)(void))
{
  p_call_func = p_func;
  return true;
}

void *lcdcGetFrameBuffer(uint8_t index)
{
  void *buf[3] = {NULL, NULL};

  if (is_begin != true)
    return NULL;
  if (index >= 3)
    return NULL;  

  esp_lcd_rgb_panel_get_frame_buffer(h_panel, HW_LCD_FB_CNT, &buf[0], &buf[1], &buf[2]);  

  return buf[index];
}

bool lcdcRefreshFrameBuffer(void *buf)
{
  if (is_begin != true) return false;

  esp_lcd_panel_draw_bitmap(h_panel, 0, 0, lcdc_width, lcdc_height, buf);
  // esp_lcd_panel_draw_bitmap(h_panel, 0, 0, lcdc_height, lcdc_width, buf);
  return true;
}

IRAM_ATTR bool lcdcISR(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx)
{
  bool ret = false;

  if (p_call_func != NULL)
  {
    ret = p_call_func();
  }

  return ret;
}
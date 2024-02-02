#include "lcd/st7701.h"


#ifdef _USE_HW_ST7701
#include "gpio.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

static bool st7701Reset(void);
static bool st7701InitRegs(void);
static bool st7701WriteCmd(uint8_t data);
static bool st7701WriteData(uint8_t data);
static bool st7701SpiInit(void);


static bool is_init = false;
static spi_device_handle_t spi_ch;



bool st7701Init(void)
{
  bool ret;

  st7701SpiInit();
  ret = st7701Reset();

  is_init = ret;
  return ret;
}

static void IRAM_ATTR preCB(spi_transaction_t *t)
{
}

static void IRAM_ATTR postCB(spi_transaction_t *t)
{
}

bool st7701SpiInit(void)
{
  esp_err_t ret;

  spi_bus_config_t buscfg = 
  {
    .miso_io_num = -1,
    .mosi_io_num = GPIO_NUM_6,
    .sclk_io_num = GPIO_NUM_7,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 64*1024
  };
  spi_device_interface_config_t devcfg = 
  {
    .command_bits = 9,
    .clock_speed_hz = 10*1000*1000,          // Clock out at 10 MHz
    .mode = 0,                               // SPI mode 0
    .spics_io_num = GPIO_NUM_15,             // CS pin
    .queue_size = 16,                        // We want to be able to queue 8 transactions at a time
    .flags = SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_NO_RETURN_RESULT, 
    .pre_cb = preCB,           
    .post_cb = postCB,
  };

  //Initialize the SPI bus
  ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
  if (ret != ESP_OK)
  {
    return false;
  }

  //Attach the LCD to the SPI bus
  ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spi_ch);
  if (ret != ESP_OK)
  {
    return false;
  }  

  return true;
}

bool st7701Reset(void)
{
  st7701InitRegs();
  return true;
}

bool st7701InitRegs(void)
{
  gpioPinWrite(3, 0);
  delay(10);
  gpioPinWrite(3, 1);

  delay(100);
 
#if 1
  st7701WriteCmd(0xF0);
  st7701WriteData(0xC3);  // C3h enable command 2 part I

  st7701WriteCmd(0xF0);
  st7701WriteData(0x96);  // 96h enable command 2 part II

  st7701WriteCmd(0x36);   // MADCTL (36h): Memory Data Access Control
  uint8_t reg = 0;
  reg |= (0<<7);  // MY
  reg |= (0<<6);  // MX
  reg |= (0<<5);  // MV
  reg |= (0<<4);  // ML
  #if HW_LCD_ROTATE == 1
  reg |= (1<<2);  // MH
  #endif
  reg |= (1<<3);  // RGB
  st7701WriteData(reg);

  st7701WriteCmd(0x21);   // Display Inversion On

  
  st7701WriteCmd(0xB4);   // Display Inversion Control
  st7701WriteData(0x01);

  st7701WriteCmd(0xB0);   
  st7701WriteData(0x00);
  
  st7701WriteCmd(0xB1);  // Frame Rate Control (In Normal Mode/Full Colors)
  st7701WriteData(0x80); // A0);
  st7701WriteData(0x10); // 42

  st7701WriteCmd(0xB5);  // Blanking Porch Control
  st7701WriteData(0x1F); // A0);
  st7701WriteData(0x50); // 42
  st7701WriteData(0x00);
  st7701WriteData(0x20);

  st7701WriteCmd(0xB6);  // Display Function Control
  st7701WriteData(0xA0); // A0, 20
  st7701WriteData(0x47); // 07
  st7701WriteData(0x3B);

  st7701WriteCmd(0xC0);
  st7701WriteData(0x80);

  st7701WriteData(0x64); // 42

  st7701WriteCmd(0xC1);
  st7701WriteData(0x13); // A0);
  st7701WriteCmd(0xC2);
  st7701WriteData(0xA7); // A0);

  st7701WriteCmd(0xC5);
  st7701WriteData(0x09); // A0);

  st7701WriteCmd(0xE8);
  st7701WriteData(0x40);
  st7701WriteData(0x8a);
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0x29);
  st7701WriteData(0x19);
  st7701WriteData(0xa5);
  st7701WriteData(0x33);

  st7701WriteCmd(0xE0); // Positive Voltage Gamma Control
  st7701WriteData(0xf0);
  st7701WriteData(0x06);
  st7701WriteData(0x0b);
  st7701WriteData(0x07);
  st7701WriteData(0x06);
  st7701WriteData(0x05);
  st7701WriteData(0x2e);
  st7701WriteData(0x33);
  st7701WriteData(0x47);
  st7701WriteData(0x3a);
  st7701WriteData(0x17);
  st7701WriteData(0x16);
  st7701WriteData(0x2e);
  st7701WriteData(0x31);

  st7701WriteCmd(0xE1); // Negative Voltage Gamma Control
  st7701WriteData(0xf0);
  st7701WriteData(0x09);
  st7701WriteData(0x0d);
  st7701WriteData(0x09);
  st7701WriteData(0x08);
  st7701WriteData(0x23);
  st7701WriteData(0x2e);
  st7701WriteData(0x33);
  st7701WriteData(0x46);
  st7701WriteData(0x38);
  st7701WriteData(0x13);
  st7701WriteData(0x13);
  st7701WriteData(0x2c);
  st7701WriteData(0x32);



  st7701WriteCmd(0xF0);
  st7701WriteData(0x3C);

  st7701WriteCmd(0xF0);
  st7701WriteData(0x69);

  st7701WriteCmd(0x3A);   // Interface Pixel Format
  st7701WriteData(0x55);

  st7701WriteCmd(0x2A);
  st7701WriteData(0x00); // A0);
  st7701WriteData(0x00); // 42
  st7701WriteData(0x01);
  st7701WriteData(0x3F);

  st7701WriteCmd(0x2B);
  st7701WriteData(0x00); // A0);
  st7701WriteData(0x00); // 42
  st7701WriteData(0x01);
  st7701WriteData(0xDF);
  

  st7701WriteCmd(0x11);   // Sleep Out
  delay(120);             // Delay 120ms
#else
  //PAGE1
  st7701WriteCmd(0xFF);    
  st7701WriteData(0x77);
  st7701WriteData(0x01);
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0x10);

  st7701WriteCmd(0xC0);    
  st7701WriteData(0x3B);
  st7701WriteData(0x00);

  st7701WriteCmd(0xC1);    
  st7701WriteData(0x0D);
  st7701WriteData(0x02);

  st7701WriteCmd(0xC2);    
  st7701WriteData(0x31);
  st7701WriteData(0x05);

  st7701WriteCmd(0xCD);
  st7701WriteData(0x08);

  st7701WriteCmd(0xB0);    
  st7701WriteData(0x00); //Positive Voltage Gamma Control
  st7701WriteData(0x11);
  st7701WriteData(0x18);
  st7701WriteData(0x0E);
  st7701WriteData(0x11);
  st7701WriteData(0x06);
  st7701WriteData(0x07);
  st7701WriteData(0x08);
  st7701WriteData(0x07);
  st7701WriteData(0x22);
  st7701WriteData(0x04);
  st7701WriteData(0x12);
  st7701WriteData(0x0F);
  st7701WriteData(0xAA);
  st7701WriteData(0x31);
  st7701WriteData(0x18);

  st7701WriteCmd(0xB1);    
  st7701WriteData(0x00); //Negative Voltage Gamma Control
  st7701WriteData(0x11);
  st7701WriteData(0x19);
  st7701WriteData(0x0E);
  st7701WriteData(0x12);
  st7701WriteData(0x07);
  st7701WriteData(0x08);
  st7701WriteData(0x08);
  st7701WriteData(0x08);
  st7701WriteData(0x22);
  st7701WriteData(0x04);
  st7701WriteData(0x11);
  st7701WriteData(0x11);
  st7701WriteData(0xA9);
  st7701WriteData(0x32);
  st7701WriteData(0x18);

  //PAGE1
  st7701WriteCmd(0xFF);    
  st7701WriteData(0x77);
  st7701WriteData(0x01);
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0x11);

  st7701WriteCmd(0xB0);    st7701WriteData(0x60); //Vop=4.7375v
  st7701WriteCmd(0xB1);    st7701WriteData(0x32); //VCOM=32
  st7701WriteCmd(0xB2);    st7701WriteData(0x07); //VGH=15v
  st7701WriteCmd(0xB3);    st7701WriteData(0x80);
  st7701WriteCmd(0xB5);    st7701WriteData(0x49); //VGL=-10.17v
  st7701WriteCmd(0xB7);    st7701WriteData(0x85);
  st7701WriteCmd(0xB8);    st7701WriteData(0x21); //AVDD=6.6 & AVCL=-4.6
  st7701WriteCmd(0xC1);    st7701WriteData(0x78);
  st7701WriteCmd(0xC2);    st7701WriteData(0x78);

  st7701WriteCmd(0xE0);    
  st7701WriteData(0x00);
  st7701WriteData(0x1B);
  st7701WriteData(0x02);

  st7701WriteCmd(0xE1);   
  st7701WriteData(0x08);
  st7701WriteData(0xA0);
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0x07);
  st7701WriteData(0xA0);
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0x44);
  st7701WriteData(0x44);

  st7701WriteCmd(0xE2);    
  st7701WriteData(0x11);
  st7701WriteData(0x11);
  st7701WriteData(0x44);
  st7701WriteData(0x44);
  st7701WriteData(0xED);
  st7701WriteData(0xA0);
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0xEC);
  st7701WriteData(0xA0);
  st7701WriteData(0x00);
  st7701WriteData(0x00);

  st7701WriteCmd(0xE3);    
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0x11);
  st7701WriteData(0x11);

  st7701WriteCmd(0xE4);    
  st7701WriteData(0x44);
  st7701WriteData(0x44);

  st7701WriteCmd(0xE5);    
  st7701WriteData(0x0A);
  st7701WriteData(0xE9);
  st7701WriteData(0xD8);
  st7701WriteData(0xA0);
  st7701WriteData(0x0C);
  st7701WriteData(0xEB);
  st7701WriteData(0xD8);
  st7701WriteData(0xA0);
  st7701WriteData(0x0E);
  st7701WriteData(0xED);
  st7701WriteData(0xD8);
  st7701WriteData(0xA0);
  st7701WriteData(0x10);
  st7701WriteData(0xEF);
  st7701WriteData(0xD8);
  st7701WriteData(0xA0);

  st7701WriteCmd(0xE6);   
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0x11);
  st7701WriteData(0x11);

  st7701WriteCmd(0xE7);    
  st7701WriteData(0x44);
  st7701WriteData(0x44);

  st7701WriteCmd(0xE8);    
  st7701WriteData(0x09);
  st7701WriteData(0xE8);
  st7701WriteData(0xD8);
  st7701WriteData(0xA0);
  st7701WriteData(0x0B);
  st7701WriteData(0xEA);
  st7701WriteData(0xD8);
  st7701WriteData(0xA0);
  st7701WriteData(0x0D);
  st7701WriteData(0xEC);
  st7701WriteData(0xD8);
  st7701WriteData(0xA0);
  st7701WriteData(0x0F);
  st7701WriteData(0xEE);
  st7701WriteData(0xD8);
  st7701WriteData(0xA0);

  st7701WriteCmd(0xEB);    
  st7701WriteData(0x02);
  st7701WriteData(0x00);
  st7701WriteData(0xE4);
  st7701WriteData(0xE4);
  st7701WriteData(0x88);
  st7701WriteData(0x00);
  st7701WriteData(0x40);

  st7701WriteCmd(0xEC);    
  st7701WriteData(0x3C);
  st7701WriteData(0x00);

  st7701WriteCmd(0xED);    
  st7701WriteData(0xAB);
  st7701WriteData(0x89);
  st7701WriteData(0x76);
  st7701WriteData(0x54);
  st7701WriteData(0x02);
  st7701WriteData(0xFF);
  st7701WriteData(0xFF);
  st7701WriteData(0xFF);
  st7701WriteData(0xFF);
  st7701WriteData(0xFF);
  st7701WriteData(0xFF);
  st7701WriteData(0x20);
  st7701WriteData(0x45);
  st7701WriteData(0x67);
  st7701WriteData(0x98);
  st7701WriteData(0xBA);

  st7701WriteCmd(0x36);    MADCTL (36h): Memory Data Access Control
  st7701WriteData(0x00);

  //-----------VAP & VAN---------------
  st7701WriteCmd(0xFF);    
  st7701WriteData(0x77);
  st7701WriteData(0x01);
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0x13);

  st7701WriteCmd(0xE5);    
  st7701WriteData(0xE4);

  st7701WriteCmd(0xFF);
  st7701WriteData(0x77);
  st7701WriteData(0x01);
  st7701WriteData(0x00);
  st7701WriteData(0x00);
  st7701WriteData(0x00);


  st7701WriteCmd(0x3A);   //0x70 RGB888, 0x60 RGB666, 0x50 RGB565
  st7701WriteData(0x60);

  st7701WriteCmd(0x21);   //Display Inversion On


  st7701WriteCmd(0x11);   //Sleep Out
  delay(100);
#endif

  bool ret;
  ret = st7701WriteCmd(0x29);   //Display On
  delay(50);

  return ret;
}

bool st7701WriteCmd(uint8_t data)
{
  bool ret = true;
  esp_err_t esp_ret;
  spi_transaction_t t;
  uint16_t tx_data;


  tx_data = (0<<8) | (data);
  memset(&t, 0, sizeof(t));     // Zero out the transaction
  t.cmd = tx_data;
  
  esp_ret = spi_device_polling_transmit(spi_ch, &t);  //Transmit!
  if (esp_ret != ESP_OK)
  {
    logPrintf("st7701WriteCmd fail\n");
    ret = false;
  }
  return ret;
}

bool st7701WriteData(uint8_t data)
{
  bool ret = true;
  esp_err_t esp_ret;
  spi_transaction_t t;
  uint16_t tx_data;

  tx_data = (1<<8) | (data);
  memset(&t, 0, sizeof(t));     // Zero out the transaction
  t.cmd = tx_data;

  esp_ret = spi_device_polling_transmit(spi_ch, &t);  //Transmit!
  if (esp_ret != ESP_OK)
  {
    logPrintf("st7701WriteData fail\n");
    ret = false;
  }
  return ret;
}
#endif
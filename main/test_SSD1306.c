#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
#include <u8g2.h>

#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"

//vcc gnd din clk cs dc rst
//       d23 d22 d21 d19 d18
// CLK - GPIO14
#define PIN_CLK 22

// MOSI - GPIO 13
#define PIN_MOSI 23

// RESET - GPIO 26
#define PIN_RESET 18

// DC - GPIO 27
#define PIN_DC 19

// CS - GPIO 15
#define PIN_CS 21
static char tag[] = "test_SSD1306spi";

void draw(void);
u8g2_t u8g2; // a structure which will contain all the data for one display
void task_test_SSD1306(void *ignore) {
	u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.clk   = PIN_CLK;
	u8g2_esp32_hal.mosi  = PIN_MOSI;
	u8g2_esp32_hal.cs    = PIN_CS;
	u8g2_esp32_hal.dc    = PIN_DC;
	u8g2_esp32_hal.reset = PIN_RESET;
	u8g2_esp32_hal_init(u8g2_esp32_hal);


	//u8g2_Setup_ssd1327_i2c_midas_128x128_f(
	//u8g2_Setup_ssd1327_seeed_96x96_f(
	u8g2_Setup_ssd1327_midas_128x128_f(
		&u8g2,
		U8G2_R0,
		u8g2_esp32_spi_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure
	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
	u8g2_SetPowerSave(&u8g2, 0); // wake up display
	//u8g2_ClearBuffer(&u8g2);
	u8g2_DrawFrame(&u8g2, 5,20, 100, 80);
	u8g2_DrawCircle(&u8g2, 20, 25, 10, U8G2_DRAW_ALL);
	u8g2_SetFont(&u8g2, u8g2_font_robot_de_niro_tf );
	u8g2_SendBuffer(&u8g2);
	ESP_LOGI(tag, "All done!");

int x= 0, y = 0;



for (int var = 0; var < 50; ++var) {
	u8g2_DrawFrame(&u8g2, x,y, 6, 6);
	//u8g2_DrawStr(&u8g2, x+5,y+15,"Hello World!");
	u8g2_DrawCircle(&u8g2, x+20, y+25, 10, U8G2_DRAW_ALL);
		u8g2_SendBuffer(&u8g2);
			vTaskDelay(100 / portTICK_PERIOD_MS);
	x=x+5;
	y=y+10;
	ESP_LOGI(tag, "afterDelay");
	//u8g2_ClearBuffer(&u8g2);
	if (y>100) {
	x=0; y=0; }
}//end for



		




/*
	int x= 0, y = 0;

	u8g2_FirstPage(&u8g2);
	  do {

		  draw();


	  } while ( u8g2_NextPage(&u8g2) );
*/


	vTaskDelete(NULL);
}
void draw(void) {
// graphic commands to redraw the complete screen should be placed here
	u8g2_SetFont(&u8g2, u8g2_font_robot_de_niro_tf );
			  u8g2_DrawStr(&u8g2, 0,0,"Hello World!");
}

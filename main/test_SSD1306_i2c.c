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

#include "test_SSD1306i2c.h"

// SDA - GPIO21
#define PIN_SDA 23

// SCL - GPIO22
#define PIN_SCL 22

static const char *TAG = "ssd1306i2c";

void task_test_SSD1306i2c(void *ignore) {
	u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda   = PIN_SDA;
	u8g2_esp32_hal.scl  = PIN_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);


	u8g2_t u8g2; // a structure which will contain all the data for one display
	//u8g2_Setup_ssd1306_i2c_128x32_univision_f(
	u8g2_Setup_ssd1327_i2c_midas_128x128_f(
		&u8g2,
		U8G2_R0,
		//u8x8_byte_sw_i2c,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure
	u8g2_SetI2CAddress(&u8g2,0x78);


	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
	u8g2_SetPowerSave(&u8g2, 0); // wake up display
	u8g2_ClearBuffer(&u8g2);
	//u8g2_DrawBox(&u8g2, 0, 26, 0,6);
	//u8g2_DrawFrame(&u8g2, 0,26,100,100);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
   u8g2_DrawStr(&u8g2, 2,17,"distance!");
   u8g2_DrawStr(&u8g2, 2,30,"temperature");
	u8g2_SendBuffer(&u8g2);



	struct us100_device pxRxedMessage;
	char string1[10], string2[10];

	 while( 1 )
	 {
	     // Receive a message on the created queue.  Block for 10 ticks if a
	     // message is not immediately available.
	     if( xQueueReceive( xQueue1, &pxRxedMessage , ( TickType_t ) 0 ) )
	     {

	         sprintf(string1, "%dmm",pxRxedMessage.distance);
	         sprintf(string2, "%dC",pxRxedMessage.tempC);
	         u8g2_ClearBuffer(&u8g2);
	         	u8g2_DrawStr(&u8g2, 2,17,string1);
	         	   u8g2_DrawStr(&u8g2, 2,40,string2);
	         		u8g2_SendBuffer(&u8g2);
	         		//printf("in queue %d\n", uxQueueMessagesWaiting(xQueue1 ));

	         // by vATask.
	     }
	 }

	/*for (int var = 0; var < 110; var=var+10) {
		u8g2_DrawBox(&u8g2, 0, 26, var,6);
		printf("another box end %d\n", var);
		u8g2_SendBuffer(&u8g2);
	}
printf("finished box end loop\n");*/


	vTaskDelete(NULL);
}


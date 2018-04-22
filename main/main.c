#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
//#include "test_SSD1306.h"
#include "test_SSD1306i2c.h"
#include "driver/uart.h"
#include "sdkconfig.h"

#define ECHO_TEST_TXD  (GPIO_NUM_17)
#define ECHO_TEST_RXD  (GPIO_NUM_16)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)
#define BUF_SIZE (256)
#define BLINK_GPIO 2


static void init_uart()
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, BUF_SIZE, 0, NULL, 0);



} //close init uart


    void distance_task(void *pvParameter) {
    	uint8_t *data =  (uint8_t *) calloc(BUF_SIZE, sizeof(uint8_t));
    	struct us100_device *px_us100_device;
    	xQueue1 = xQueueCreate( 10, sizeof( struct us100_device ) );

        // Read distance data from the UART
    	const char temp2[] = { 0x55, 0x50 };
    while(1) {
    	uart_write_bytes(UART_NUM_1, temp2, 1);    //position
    	//uart_write_bytes(UART_NUM_1, 0x55, 1);
    	int len = uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 50 / portTICK_RATE_MS);
        // Write data back to the UART
    	int  distanceMM;
    	distanceMM = (data[0] << 8) | (data[1]);
    	//printf("data come in %dmm \n", distanceMM);
    	data[0] = 0; data[1] = 0;


       // Read temperature data from the UART
    	uart_write_bytes(UART_NUM_1, temp2+1, 1);
    	len = uart_read_bytes(UART_NUM_1, data+2, BUF_SIZE, 20 / portTICK_RATE_MS);
        // Write data back to the UART
    	int  tempC;
    	tempC = data[2] - 45;
    	data[2]=0;
    	//printf("temperature come in %ddegrees C \n", tempC);
    	//printf("--------------------------------------------------\n");
    	//load structure for queue
    	struct_us100_device.distance = distanceMM;
    	struct_us100_device.tempC = tempC;
    	px_us100_device = & struct_us100_device;
    	xQueueSend(xQueue1,px_us100_device, ( TickType_t )2000/ portTICK_PERIOD_MS );
    	vTaskDelay(350 / portTICK_PERIOD_MS);
    }// close while
    }//close distance task







void blink_task(void *pvParameter) {
	/* Configure the IOMUX register for pad BLINK_GPIO (some pads are
	 muxed to GPIO on reset already, but some default to other
	 functions and need to be switched to GPIO. Consult the
	 Technical Reference for a list of pads and their default
	 functions.)
	 */
	gpio_pad_select_gpio(BLINK_GPIO);
	/* Set the GPIO as a push/pull output */
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
	while (1) {
		/* Blink off (output low) */

		gpio_set_level(BLINK_GPIO, 0);
		vTaskDelay(250 / portTICK_PERIOD_MS);
		/* Blink on (output high) */
		gpio_set_level(BLINK_GPIO, 1);
		vTaskDelay(10 / portTICK_PERIOD_MS);

	}
}

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
    printf("did something happen here");
}

void app_main(void)
{
    nvs_flash_init();
  /*  tcpip_adapter_init();

    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
        .sta = {
            .ssid = "fatherJack",
            .password = "batmanN9",
            .bssid_set = false
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() ); */

    init_uart();
    xQueueCreate( 10, sizeof( struct_us100_device) );
  xTaskCreate(&blink_task, "blink_task", 512, NULL, 5, NULL);

	//xTaskCreate(&task_test_SSD1306, "task_test_SSD1306", 2048, NULL, 4, NULL);
  xTaskCreate(&task_test_SSD1306i2c, "task_test_SSD1306i2c", 4096, NULL, 4, NULL);
    xTaskCreate(distance_task, "distance_task", 4096, NULL, 10, NULL);

}


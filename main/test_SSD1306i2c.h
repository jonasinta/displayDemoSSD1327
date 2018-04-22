/*
 * test_SSD1306i2c.h
 *
 *  Created on: 19/04/2018
 *      Author: jonas
 */

#ifndef TEST_SSD1306i2c_H_
#define TEST_SSD1306i2c_H_

QueueHandle_t xQueue1;
struct us100_device{
	int distance;
	int tempC;
}struct_us100_device;

void task_test_SSD1306i2c(void *);


#endif /* TEST_SSD1306i2c_H_ */

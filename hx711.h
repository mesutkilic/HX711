#ifndef HX711_H_
#define HX711_H_

#include "stm32f3xx_hal.h"

struct _hx711_t
{
	GPIO_TypeDef* gpioSck;
	GPIO_TypeDef* gpioData;
	uint16_t pinSck;
	uint16_t pinData;
	int offset;
	int gain;
	// 1: channel A, gain factor 128
	// 2: channel B, gain factor 32
    // 3: channel A, gain factor 64
};

typedef struct _hx711_t hx711_t;

void HX711_Init(hx711_t data);
HX711 HX711_Tare(hx711_t data, uint8_t times);
int HX711_Value(hx711_t data);
int HX711_AverageValue(hx711_t data, uint8_t times);

#endif /* HX711_H_ */

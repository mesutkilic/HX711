#ifndef HX711_H_
#define HX711_H_

#include "stm32f0xx_hal.h"
#include "main.h"

#define HX711_SCK_PORT clk_GPIO_Port
#define HX711_SCK_PIN1 clk_Pin

#define HX711_DATA_PORT data_GPIO_Port
#define HX711_DATA_PIN1 data_Pin


/**
 * @brief hx711 struct
 *
 */
struct _hx711_t
{
	GPIO_TypeDef* portSck;  /**< port name for sck pin */
	GPIO_TypeDef* portData; /**< port name for data pin */
	uint16_t pinSck; 		/**< pin name for sck pin */
	uint16_t pinData; 		/**< pin name for data pin */
	int offset; 			/**< offset for tare operation */
	uint8_t gain; 			/**< gain selection <br>  
							* 1: channel A, gain factor 128 <br> 
							* 2: channel B, gain factor 32 <br> 
							* 3: channel A, gain factor 64 */
	int buffer; 			/**< buffer for save data */
};

typedef struct _hx711_t hx711_t;

//these functions from freakone/HX711 library not used in here
void HX711_Init(hx711_t data);
int HX711_Value2(hx711_t data);

//HX711 value functions

void HX711_Tare(hx711_t *data, uint8_t times);
int HX711_Value(hx711_t *data);
int HX711_AverageValue(hx711_t *data, uint8_t times);

//init functions
void init_hx711_port(hx711_t *data, GPIO_TypeDef* portScki, uint16_t pinScki,
		GPIO_TypeDef* portDatai, uint16_t pinDatai );
void init_hx711(hx711_t *data);

#endif /* HX711_H_ */

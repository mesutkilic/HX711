#include "hx711.h"

void HX711_Init(hx711_t data)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = data.pinSck;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(data.gpioSck, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = data.pinData;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(data.gpioData, &GPIO_InitStruct);

	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(data.gpioData, data.pinSck, GPIO_PIN_RESET);

}


int HX711_Value2(hx711_t data)
{
    int buffer;
    buffer = 0;

    while (HAL_GPIO_ReadPin(data.gpioData, data.pinData)==1)
    ;

    for (uint8_t i = 0; i < 24; i++)
    {
    	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);

        buffer = buffer << 1 ;

        if (HAL_GPIO_ReadPin(data.gpioData, data.pinData))
        {
            buffer ++;
        }

        HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);
    }

    for (int i = 0; i < data.gain; i++)
    {
    	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);
    }

    buffer = buffer ^ 0x800000;

    return buffer;
}

/**
 * @brief		gets sensor value for @ref times and gets average
 * @param data	hx711 struct
 * @param times	how many time read data and get average
 * @return 		average value
 */
int HX711_Average_Value(hx711_t *data, uint8_t times)
{
    int sum = 0;
    for (int i = 0; i < times; i++)
    {
        sum += HX711_Value(data);
    }

    return sum / times;
}

/**
 * @param data	hx711 struct
 * @return 		24 bit sensor value
 */
int HX711_Value(hx711_t *data)
{
	int count=0;
    while (HAL_GPIO_ReadPin(data->portData, data->pinData)==1); // wait for data pin 0

    for (uint8_t i = 0; i < 24; i++)
    {
    	HAL_GPIO_WritePin(data->portSck, data->pinSck, GPIO_PIN_SET); //set clk 1
        count = count << 1 ; //increase buffer value
        HAL_GPIO_WritePin(data->portSck, data->pinSck, GPIO_PIN_RESET); //set clk 0
        if(HAL_GPIO_ReadPin(data->portData, data->pinData) == 1){count++;} //read and update data buffer
    }

    //25. clk to set next conversation CH.A, Gain:128
    HAL_GPIO_WritePin(data->portSck, data->pinSck, GPIO_PIN_SET);
    count = count ^ 0x800000; //get 2's complement of data
    HAL_GPIO_WritePin(data->portSck, data->pinSck, GPIO_PIN_RESET);
	
	data->buffer=count;
    return count;
}


/**
 * @brief 		gets sensor data @ref times and gets average value, sets offset.
 * @param data	hx711 struct
 * @param times	how many time read data and get average
 */
void HX711_Tare(hx711_t *data, uint8_t times)
{
    int sum = HX711_Average_Value(data, times);
    data->offset = sum;
}

/**
 * @brief			inits hx711 struct for given parameters
 * @param data 		hx711 struct
 * @param portScki	portname for sck pin : GPIOA
 * @param pinScki	pinname for sck pin  : GPIO_PIN_0
 * @param portDatai portname for data pin : GPIOA
 * @param pinDatai	pinname for data pin  : GPIO_PIN_0
 */
void init_hx711_port(hx711_t *data, GPIO_TypeDef* portScki, uint16_t pinScki,
		GPIO_TypeDef* portDatai, uint16_t pinDatai )
{
	data->gain=1;
	data->portData=portDatai;
	data->portSck=portScki;
	data->offset=0;
	data->pinData=pinDatai;
	data->pinSck=pinScki;
	data->buffer=0;

	HAL_GPIO_WritePin(data->portSck, data->pinSck, GPIO_PIN_RESET);
	HAL_Delay(50);
}

/**
 * @brief		inits hx711 struct for defined parameters
 * @param data	hx711 struct
 */
void init_hx711(hx711_t *data)
{
	data->gain=1;
	data->portData=HX711_DATA_PORT;
	data->portSck=HX711_SCK_PORT;
	data->offset=0;
	data->pinData=HX711_DATA_PIN1;
	data->pinSck=HX711_SCK_PIN1;
	data->buffer=0;

	HAL_GPIO_WritePin(data->portSck, data->pinSck, GPIO_PIN_RESET);
	HAL_Delay(50);
}

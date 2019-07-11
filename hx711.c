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

int HX711_Average_Value(hx711_t data, uint8_t times)
{
    int sum = 0;
    for (int i = 0; i < times; i++)
    {
        sum += HX711_Value(data);
    }

    return sum / times;
}

int HX711_Value(hx711_t data)
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

hx711_t HX711_Tare(hx711_t data, uint8_t times)
{
    int sum = HX711_Average_Value(data, times);
    data.offset = sum;
    return data;
}


void init_hx711_port(hx711_t *data, GPIO_TypeDef* portScki, uint16_t pinScki,
		GPIO_TypeDef* portDatai, uint16_t pinDatai )
{
	data->gain=1;
	data->portData=portDatai;
	data->portSck=portScki;
	data->offset=0;
	data->pinData=pinDatai;
	data->pinSck=pinScki;

	HAL_GPIO_WritePin(data->portSck, data->pinSck, GPIO_PIN_RESET);
	HAL_Delay(50);
}

# HX711
Procedural implementation of HX711 library with HAL for STM32

This library is tested on STM32 NUCLEO-F030R8 and working.

# Usage `Init`

If using one sensor define SCK and DATA pins. 
Use `init_hx711(hx711_t *data);` function

If using more than one sensor use tis fuction : 
`init_hx711_port(hx711_t *data, GPIO_TypeDef* portScki, uint16_t pinScki, GPIO_TypeDef* portDatai, uint16_t pinDatai );`

Mesut KILIÇ

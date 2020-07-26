
/** \file   wifi_task.h
*   \author Alex
*   \date   22.07.2020
*
* Задача обслуживания модуля ESP8266
*/

#pragma once

#include "main.h"

#define WIFI_UART                 USART6

/** Инициализация задачи обслуживания модуля ESP8266
*/
void init_wifi_task( void );

/** Задача обслуживания модуля ESP8266
*/
int wifi_task( void );

/** Прерыввание UART-а ESP8266
*/
void wifi_uart_irq( void );
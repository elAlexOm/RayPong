
/** \file   ws2812_task.h
*   \author Alex
*   \date   22.07.2020
*
* Задача обслуживания ленты WS2812
*/

#pragma once

#include "draw.h"

/** Инициализация задачи обслуживания ленты
*/
void init_ws2812_task( void );

/** Задача обслуживания ленты
*/
int ws2812_task( void );

/** Запуск обновления экрана
*/
void ws_start_update( void );

/** Прерывание таймера светодиодов
*/
void ws_timer_irq( void );

extern ws_led_s ws_string[];
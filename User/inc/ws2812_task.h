
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

void ws_stop_update( void );

/** Прерывание таймера светодиодов
*/
void ws_timer_irq( void );

extern pixel_t ws_string[];

/** \file   cli.h
*   \author Alex
*   \date   12.08.2020
* 
* Реализация CLI для работы с ESP на STM32
*/

#pragma once

#include <stdint.h>

/** Инициализация задачи обработки сообщений
*/
void init_cli_task( void );

/** Задача обработки сообщений
* \return     - статус процесса
*/
int cli_task( void );

void token_handler( void* str, uint32_t len );

/** Запуск задачи обработки сообщений
* \param  count   - количество символов в буффере
*/
void cli_start( uint16_t count );

extern char get_next_char( void );

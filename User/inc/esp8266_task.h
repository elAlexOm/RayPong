
/** \file   esp8266_task.h
*   \author Alex
*   \date   11.08.2020
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>

/** Инициализация задачи обслуживания модуля ESP8266
*/
void init_esp_task( void );

/** Задача обслуживания модуля ESP8266
*/
int esp_task( void );

void esp_half_rx_buffer_full_event( void* data, uint32_t len );

void esp_full_rx_buffer_full_event( void* data, uint32_t len );

void esp_ev_input_data( char* data );

/** Обработчик события по отдаче буффера
*/
void esp_tx_end_handler( void );

void init_ESP_UART( void );

/** Обработчик прерывания USART
*/
void ESP_UART_irq( void );

void start_tx_data( uint8_t* data, uint32_t size );

/** Возвращает адрес и размер буффера на отправку
*/
uint8_t* get_tx_buffer( uint16_t* size );

bool is_tranmite_complete( void );
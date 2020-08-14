
/** \file 	ring_buffer.h
*		\author Alex
*		\date		1.2.2019
*
* Реализация циклического буфера.
* Память под буффер может быть выделена как статически, так и динамически в куче
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>

/** Результаты операций
*/
typedef enum {
	rbOk 				= 0,
	rbFault 		= -1,
	rbMemFault 	= -2,
	rbFree			= -3,
	rbFull			= -4
} rbResult_t;

/** Структура буфера
*/
typedef __packed struct {
	uint16_t	head;
	uint16_t	tile;
	uint16_t 	count;
	uint16_t	size;
	uint16_t 	*buffer;
} ring_buffer_t;

/** Инициализация буфера
*
* В случае если buf равен NULL пытается выделить память в куче.
* При нехватке возвращает false
*
*	\param	buffer 	- указатель на буфер
*	\param	buf			-	указатель на статический буффер
*	\param	length	- размер буфера
*	\return 				-	статус операции
*/
rbResult_t ring_buffer_init( ring_buffer_t* buffer, void* buf, uint16_t length );

/** Свободное место в буфере
*	\param	buffer 	- указатель на буфер
*	\return					- cвободное место
*/
uint8_t buffer_get_free( ring_buffer_t* buffer );

/** Количество элементов в буфере
*	\param	buffer 	- указатель на буфер
*	\return					- занятое место
*/
uint8_t buffer_get_count( ring_buffer_t* buffer );

/** Загрузка данных в буффер
*	\param	buffer 	- указатель на буфер
*	\param	data		- сохраняемые данные
*	\return 				-	статус операции
*/
rbResult_t ring_buffer_set( ring_buffer_t* buffer, uint8_t data );

/** Выгрузка байта данных из буффера
*	\param	buffer 	- указатель на буфер
*	\param	data		- указатель на данные
*	\return 				-	количество отданных байт
*/
rbResult_t ring_buffer_get_byte( ring_buffer_t* buffer, uint8_t *data );

/** Выгрузка данных из буффера
*	\param	buffer 	- указатель на буфер
*	\param	data		- указатель на данные
*	\return 				-	количество отданных байт
*/
uint16_t ring_buffer_get_data( ring_buffer_t* buffer, uint8_t *data, uint16_t len );

/** Очистка буффера
*	\param	buffer 	- указатель на буфер
*	\return 				-	статус операции
*/
rbResult_t ring_buffer_clear( ring_buffer_t* buffer );

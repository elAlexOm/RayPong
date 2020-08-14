
/** \file 	ring_buffer.c
*		\author Alex
*		\date		1.2.2019
*/

#include "ring_buffer.h"

#include <stdlib.h>

#ifdef __ICCARM__
#include <cmsis_iar.h>
#endif

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
rbResult_t ring_buffer_init( ring_buffer_t* buffer, void* buf, uint16_t length ) {
	buffer->buffer = buf;
	if( buffer->buffer == NULL ) {
		buffer->buffer = malloc( length );
		if( buffer->buffer == NULL ) return rbMemFault;
	}
	buffer->size 	= length;
	buffer->head 	= 0;
	buffer->tile 	= 0;
	buffer->count	= 0;
	return rbOk;
}

/** Свободное место в буфере
*	\param	buffer 	- указатель на буфер
*	\return					- cвободное место
*/
uint8_t buffer_get_free( ring_buffer_t* buffer ) {
	return buffer->size - buffer->count;
}

/** Количество элементов в буфере
*	\param	buffer 	- указатель на буфер
*	\return					- занятое место
*/
uint8_t buffer_get_count( ring_buffer_t* buffer ) {
  return buffer->count;
}

/** Загрузка данных в буффер
*	\param	buffer 	- указатель на буфер
*	\param	data		- сохраняемые данные
*	\return 				-	статус операции
*/
rbResult_t ring_buffer_set( ring_buffer_t* buffer, uint8_t data ) {
	rbResult_t result = rbFull;

	int i =  __get_PRIMASK();
  __disable_irq();

	if( buffer->count < buffer->size ) {
		buffer->buffer[buffer->head] = data;
		if( ++buffer->head >= buffer->size ) buffer->head = 0;
		buffer->count++;
		result = rbOk;
	}

  __set_PRIMASK( i );
	return result;
}

/** Выгрузка байта данных из буффера
*	\param	buffer 	- указатель на буфер
*	\param	data		- указатель на данные
*	\return 				-	количество отданных байт
*/
uint16_t ring_buffer_get_data( ring_buffer_t* buffer, uint8_t *data, uint16_t len ) {
	rbResult_t result = rbFree;

	int i =  __get_PRIMASK();
  __disable_irq();

	if( buffer->count < len ) {
    len = buffer->count;
  }
  
  for( uint16_t i = 0; i < len; i++ ) {
		*data = buffer->buffer[buffer->tile++];
		if( ++buffer->tile >= buffer->size ) buffer->tile = 0;
		buffer->count--;
  }

  result = rbOk;

  __set_PRIMASK( i );
	return result;
}

/** Выгрузка байта данных из буффера
*	\param	buffer 	- указатель на буфер
*	\param	data		- указатель на данные
*	\return 				-	статус операции
*/
rbResult_t ring_buffer_get_byte( ring_buffer_t* buffer, uint8_t *data ) {
	rbResult_t result = rbFree;

	int i =  __get_PRIMASK();
  __disable_irq();

	if( buffer->count > 0 ) {
		*data = buffer->buffer[buffer->tile];
		if( ++buffer->tile >= buffer->size ) buffer->tile = 0;
		buffer->count--;
		result = rbOk;
	}

  __set_PRIMASK( i );
	return result;
}

/** Очистка буффера
*	\param	buffer 	- указатель на буфер
*	\return 				-	статус операции
*/
rbResult_t ring_buffer_clear( ring_buffer_t* buffer ) {
	int i =  __get_PRIMASK();
  __disable_irq();
  
  buffer->count = 0;
  buffer->tile = buffer->head;
  
  __set_PRIMASK( i );  
  return rbOk;
}  




/** \file   esp8266_io.h
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define DEFAULT_TIME_OUT                 5000 /* in ms */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int8_t ESP8266_IO_Init( void );
void ESP8266_IO_DeInit( void );

int8_t ESP8266_IO_Send( uint8_t* Buffer, uint32_t Length );
int32_t ESP8266_IO_Receive( uint8_t* Buffer, uint32_t Length );

void USART6_IRQ_Handler( void );

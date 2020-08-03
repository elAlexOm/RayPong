
/** \file   wifi_task.c
*   \author Alex
*   \date   22.07.2020
*
* Задача обслуживания модуля ESP8266
*/

#include "wifi_task.h"

#include <stdint.h>
#include <string.h>

#include "main.h"
#include "pt.h"
#include "bsp.h"
#include "slre.h"
#include "usart.h"

#define WIFI_UART_RX_BUFFER_SIZE            64
#define WIFI_RESTART_COUNT                  50
#define COMMAND_REPEAT_COUNT                30

static struct pt pt_wifi;

static uint32_t time;

static uint8_t wifi_restart_counter = WIFI_RESTART_COUNT;                 //!< Количество попыток перезапуска ESP8266
static uint8_t repeat_counter = COMMAND_REPEAT_COUNT;

//static struct {
//  char      buffer[WIFI_UART_RX_BUFFER_SIZE];
//  uint16_t  index;
//  char      eol;
//} rx_data = {
//  .index  = 0,
//  .eol    = 0
//};

uint16_t  rx_index = 0;
char      rx_buffer[WIFI_UART_RX_BUFFER_SIZE];
char      rx_eol = 0;

static struct {
  char*   buffer;
  uint8_t index;
  uint8_t count;
} tx_data = {
  .buffer  = NULL,
  .index   = 0,
  .count   = 0
};

/** Запись буфера в WIFI UART
* \param[in]  str     - указатель на буфер
* \param[in]  len     - длина буфера
*/
static void wifi_uart_tx( char* buffer, uint8_t len ) {
  tx_data.buffer = buffer;
  tx_data.index  = 0;
  tx_data.count  = len;

  WIFI_UART->CR1 |= USART_CR1_TXEIE;  
}

/** Запись строки в WIFI UART
* \param[in]  str     - указатель на записываемую строку
*/
static void wifi_uart_tx_string( char* str ) {
  tx_data.buffer = str;
  tx_data.index  = 0;  
  tx_data.count  = strlen( str );
  WIFI_UART->CR1 |= USART_CR1_TXEIE;  
}

/** Инициализация задачи обслуживания модуля ESP8266
*/
void init_wifi_task( void ) {
  PT_INIT( &pt_wifi );
}

/** Задача обслуживания модуля ESP8266
*/
int wifi_task( void ) {
  PT_BEGIN( &pt_wifi );
  
  if( wifi_restart_counter == 0 ) {
    PT_YIELD( &pt_wifi );
  }
  
  wifi_restart_counter--;
  
  ESP_RESET_LOW();
  ESP_PD_LOW();
  
  time = HAL_GetTick();  
  PT_WAIT_WHILE( &pt_wifi, ( HAL_GetTick() - time ) < 100 );  
  
  ESP_PD_HI();
  ESP_RESET_HI();  
  
  time = HAL_GetTick();   
  PT_WAIT_WHILE( &pt_wifi, ( HAL_GetTick() - time ) < 2000 );
    
// Команда AT  
  rx_eol = 0;
  repeat_counter = COMMAND_REPEAT_COUNT;
  do {
    wifi_uart_tx_string( "AT\r\n" );
    PT_WAIT_WHILE( &pt_wifi, tx_data.buffer != NULL );
    time = HAL_GetTick();
//    while(( HAL_GetTick() - time ) < 1000 ) {
//      if( rx_eol == '\n' ) {
//        if( strstr( rx_buffer, "OK" ) != NULL ) {
//          break;
//        }
//      }
//      PT_YIELD( &pt_wifi );
//    }
  } while( 1 );// --repeat_counter || ( rx_eol == 0 ));
  
  if( repeat_counter == 0 ) {
    PT_RESTART( &pt_wifi );
  }
  
  while( 1 ) {
    PT_YIELD( &pt_wifi );
  }
  PT_END( &pt_wifi );
}

/** Прерыввание UART-а ESP8266
*/
void wifi_uart_irq( void ) {
  if( WIFI_UART->ISR & USART_ISR_RXNE ) {         // Обработчик RX
    char data = WIFI_UART->RDR;
    
    rx_buffer[rx_index++] = data;
    if( rx_index >= sizeof( rx_buffer )) {
      rx_index = 0;
    }
    
//    if( data == '\r' ) {
//      rx_data.eol = '\n';      
//    }
//    else {
//      if( rx_data.index < sizeof( rx_data.buffer )) {
//        rx_data.buffer[rx_data.index++];
//      }
//    }
  }
  
  if( WIFI_UART->ISR & USART_ISR_TXE ) {          // Обработчик TX
    WIFI_UART->TDR = *tx_data.buffer++;
    if( ++tx_data.index >= tx_data.count ) {
      WIFI_UART->CR1 &= ~USART_CR1_TXEIE;
      tx_data.buffer = NULL;
    }    
  }
//  if( WIFI_UART->ISR & USART_ISR_PE ) {
//    WIFI_UART->ICR = USART_ICR_PECF;
//    rx_index = 0;
//  }
//  if( WIFI_UART->ISR & USART_ISR_FE ) {
//    WIFI_UART->ICR = USART_ICR_FECF;
//    rx_index = 0;    
//  }
//  if( WIFI_UART->ISR & USART_ISR_ORE ) {
//    WIFI_UART->ICR = USART_ICR_ORECF; 
////    rx_data.index = 0;
//  }  
}
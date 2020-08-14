
/** \file   esp8266_task.c
*   \author Alex
*   \date   11.08.2020
*/

#include "esp8266_task.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pt.h"
#include "usart.h"

#include "cli.h"

#define RX_DEBUG                        1

#define ESP_UART                        USART6
#define ENABLE_ESP_UART_CLK()           { RCC->APB2ENR |= RCC_APB2ENR_USART6EN; }

#define ESP_RX_DMA
#define ESP_TX_DMA

#define RX_BUFFER_SIZE                  256
#define TX_BUFFER_SIZE                  256

#define UART_ENABLE_RXNE()              { ESP_UART->CR1 |=  USART_CR1_RXNEIE; }
#define UART_DISABLE_RXNE()             { ESP_UART->CR1 &= ~USART_CR1_RXNEIE; }
#define IS_UART_ENABLE_RXNE()           ( ESP_UART->CR1 &   USART_CR1_RXNEIE )
#define IS_UART_RXNE_IRQ()              ( ESP_UART->ISR &   USART_ISR_RXNE )

#define UART_ENABLE_TXE()               { ESP_UART->CR1 |=  USART_CR1_TXEIE; }
#define UART_DISABLE_TXE()              { ESP_UART->CR1 &= ~USART_CR1_TXEIE; }
#define IS_UART_ENABLE_TXE()            ( ESP_UART->CR1 &   USART_CR1_TXEIE )
#define IS_UART_TXE_IRQ()               ( ESP_UART->ISR &   USART_ISR_TXE )

#define UART_ENABLE_IDLE()              { ESP_UART->CR1 |=  USART_CR1_IDLEIE; }
#define UART_DISABLE_IDLE()             { ESP_UART->CR1 &= ~USART_CR1_IDLEIE; }
#define IS_UART_ENABLE_IDLE()           ( ESP_UART->CR1 &   USART_CR1_IDLEIE )
#define IS_UART_IDLE_IRQ()              ( ESP_UART->ISR &   USART_ISR_IDLE )
#define UART_CLEAR_IDLE_IRQ()           { ESP_UART->ICR =   USART_ICR_IDLECF; }

#define UART_ENABLE_TC()               { ESP_UART->CR1 |= USART_CR1_TCIE; }
#define UART_DISABLE_TC()              { ESP_UART->CR1 &= ~USART_CR1_TCIE; ESP_UART->ICR = USART_ICR_TCCF; }

static uint8_t uart_rx_data[RX_BUFFER_SIZE];                  //!< Кольцевой буффер
static uint8_t uart_tx_buffer[TX_BUFFER_SIZE];

static struct {
  uint8_t*  data;
  uint16_t  in;
  uint16_t  out;
  uint16_t  count;
  uint16_t  size;
} uart_rx_buffer = { uart_rx_data, 0, 0, 0, sizeof( uart_rx_data ) };

static struct {
  uint8_t*  pdata;
  uint32_t  count;
} tx_data = { 0 };

void flush_rx_buffer( void ) {
  uart_rx_buffer.in     = 0;
  uart_rx_buffer.out    = 0;
  uart_rx_buffer.count  = 0;
#if RX_DEBUG == 1 
  for( uint16_t i = 0; i < uart_rx_buffer.size; i++ ) 
    uart_rx_buffer.data[i] = 0;
#endif  
}

void init_ESP_UART( void ) {
  ENABLE_ESP_UART_CLK();

  UART_CLEAR_IDLE_IRQ();
  UART_ENABLE_IDLE();
  UART_ENABLE_RXNE();
}

uint8_t* get_tx_buffer( uint16_t* size ) {
  *size = sizeof( uart_tx_buffer );
  return uart_tx_buffer;
}

void start_tx_data( uint8_t* data, uint32_t size ) {
  
  if( NULL == data ) return;
  if( 0 == size ) return;
  
  ESP_UART->ICR = USART_ICR_IDLECF; 
  tx_data.pdata = data;
  tx_data.count  = size;
  UART_ENABLE_TXE();  
}

bool is_tranmite_complete( void ) {
  return ( IS_UART_ENABLE_TXE() ? false : true );
}

void ESP_UART_irq( void ) {
  if( IS_UART_RXNE_IRQ() && IS_UART_ENABLE_RXNE()){   // Прерывание по приходу символа
    uart_rx_buffer.data[uart_rx_buffer.in] = ESP_UART->RDR;
    if( uart_rx_buffer.count < uart_rx_buffer.size ) {
      if( ++uart_rx_buffer.in >= uart_rx_buffer.size ) {
        uart_rx_buffer.in = 0;
      }
      uart_rx_buffer.count++;
      if( uart_rx_buffer.count == ( uart_rx_buffer.size / 2 )) {
        esp_half_rx_buffer_full_event( uart_rx_buffer.data, uart_rx_buffer.size / 2 );
      }
      if( uart_rx_buffer.count == uart_rx_buffer.size ) {
        esp_full_rx_buffer_full_event( uart_rx_buffer.data, uart_rx_buffer.size );
      }      
    }
  }
  if( IS_UART_TXE_IRQ() && IS_UART_ENABLE_TXE()) {
    ESP_UART->TDR = *tx_data.pdata++;
    if( --tx_data.count == 0 ) {
      UART_DISABLE_TXE();
      esp_tx_end_handler();
    }
  }
  if( ESP_UART->ISR & USART_ISR_CMF ) {
    ESP_UART->ICR = USART_ICR_CMCF; 
  }
  if( IS_UART_IDLE_IRQ() && IS_UART_ENABLE_IDLE()) {
    UART_CLEAR_IDLE_IRQ(); 
    cli_start( uart_rx_buffer.count );
  }  
  if( ESP_UART->ISR & USART_ISR_NE ) {
    ESP_UART->ICR = USART_ICR_NCF;
  }  
  if( ESP_UART->ISR & USART_ISR_FE ) {
    ESP_UART->ICR = USART_ICR_FECF;
//    flush_rx_buffer();
  }
  if( ESP_UART->ISR & USART_ISR_ORE ) {
    ESP_UART->ICR = USART_ICR_ORECF;
  }
  if( ESP_UART->ISR & USART_ISR_PE ) {
    ESP_UART->ICR = USART_ICR_PECF;
  }   
}

char get_next_char( void ) {
  char result = 0;
  if( uart_rx_buffer.count ) {
    result = uart_rx_buffer.data[uart_rx_buffer.out];
    if( ++uart_rx_buffer.out >= uart_rx_buffer.size ) {
      uart_rx_buffer.out = 0;
    }
    uart_rx_buffer.count--;
  }
  return result;
}

__weak void esp_half_rx_buffer_full_event( void* data, uint32_t len ) {
  cli_start( uart_rx_buffer.count );
}

__weak void esp_full_rx_buffer_full_event( void* data, uint32_t len ) {
  flush_rx_buffer();
}

__weak void esp_ev_input_data( char* data ) {
}

/** Обработчик события по отдаче буффера
*/
__weak void esp_tx_end_handler( void ) {
}

#undef RX_BUFFER_SIZE
#undef RX_DEBUG
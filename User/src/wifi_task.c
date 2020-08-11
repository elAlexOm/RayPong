
/** \file   wifi_task.c
*   \author Alex
*   \date   22.07.2020
*
* Задача обслуживания модуля ESP8266
*/

#include "wifi_task.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "main.h"
#include "pt.h"
#include "bsp.h"
#include "usart.h"


#include "esp8266.h"

#define MAX_NUM_TRIAL                       10

#define HOST_ADDRESS                        "192.168.0.14"
#define HOST_PORT                           52222

static struct pt pt_wifi;
static uint32_t time;
uint8_t IpAddress[15];

ESP8266_ConnectionInfoTypeDef ConnectionInfo;
ESP8266_StatusTypeDef Result = ESP8266_OK;

char mess[32];

/** Инициализация задачи обслуживания модуля ESP8266
*/
void init_wifi_task( void ) {
  PT_INIT( &pt_wifi );
}

/** Задача обслуживания модуля ESP8266
*/
int wifi_task( void ) {
  PT_BEGIN( &pt_wifi );

  ESP_RESET_LOW();  
  ESP_PD_LOW();
  
  time = HAL_GetTick();
  PT_YIELD_UNTIL( &pt_wifi, ( HAL_GetTick() - time ) > 100 );    

  ESP_RESET_HI();  
  ESP_PD_HI();
  
  time = HAL_GetTick();
  PT_YIELD_UNTIL( &pt_wifi, ( HAL_GetTick() - time ) > 1000 );   
  
  if( ESP8266_OK != ESP8266_Init()) {
    PT_YIELD( &pt_wifi );
  }

  uint32_t Trial = 0;
  while( ESP8266_JoinAccessPoint( ( uint8_t * )WIFI_SSID, ( uint8_t * )WIFI_PASSWORD ) != ESP8266_OK ) {
    if ( Trial == MAX_NUM_TRIAL ) break;
  }
  
//  /* Reset the IP address field to 0 */
//  memset( IpAddress, '\0', 15 );
//  /* Access point joined: start getting IP address */
//  ESP8266_GetIPAddress( ESP8266_STATION_MODE, IpAddress );
//  HAL_Delay( 1000 );

  Trial = 0;
  memset( &ConnectionInfo, '\0', sizeof ( ESP8266_ConnectionInfoTypeDef ) );

  ConnectionInfo.connectionType = ESP8266_TCP_CONNECTION;
  ConnectionInfo.ipAddress = ( uint8_t * )HOST_ADDRESS;
  ConnectionInfo.isServer = ESP8266_FALSE;
  ConnectionInfo.port = HOST_PORT;

  /* Wait for communication establishment */
  while ( ESP8266_EstablishConnection( &ConnectionInfo ) != ESP8266_OK ) {
#ifdef USE_LCD
    LCD_UsrLog( "Retrying( %d ) to connect to %s:%d \n", ( int )++Trial, HOST_ADDRESS, HOST_PORT );
    HAL_Delay( 1000 );
#endif /* USE_LCD */

    if ( Trial == MAX_NUM_TRIAL ) {
      break;
    }
  }
  
  sprintf( mess, "Hello XEON2678\r\n" );
  Result = ESP8266_SendData( mess, strlen(( char * )mess ) );

  /* In case of error, quit the Access point */
  if ( Result != ESP8266_OK ) {
    /* Deinitialize the WiFi module */
    ESP8266_DeInit();
    /* Call the error Handler */
//    ErrorHandler();
  }
 
//  char* mess = "I am work\r\n";
  sprintf( mess, "I am work\r\n" );
  Result = ESP8266_SendData( mess, strlen(( char * )mess ) );

  /* In case of error, quit the Access point */
  if ( Result != ESP8266_OK ) {
    /* Deinitialize the WiFi module */
    ESP8266_DeInit();
    /* Call the error Handler */
//    ErrorHandler();
  }  
  
  while( 1 ) {
    PT_YIELD( &pt_wifi );
  }
  PT_END( &pt_wifi );
}

/** Прерыввание UART-а ESP8266
*/
//void wifi_uart_irq( void ) {
//  if( WIFI_UART->ISR & USART_ISR_RXNE ) {         // Обработчик RX
//    char data = WIFI_UART->RDR;
//    
//    if( data == '\n' ) {
//      rx_eol = '\n';
//      data = 0;
//    }
//    rx_buffer[rx_head] = data;
//    if( ++rx_head == sizeof( rx_buffer )) {
//      rx_head = 0;
//    }
//    if( ++rx_count == sizeof( rx_buffer )) {
//      rx_tile = rx_head;
//    }
//  }
//  
//  if( WIFI_UART->ISR & USART_ISR_TXE ) {          // Обработчик TX
//    WIFI_UART->TDR = *tx_data.buffer++;
//    if( ++tx_data.index >= tx_data.count ) {
//      WIFI_UART->CR1 &= ~USART_CR1_TXEIE;
//      tx_data.buffer = NULL;
//    }    
//  }
//  if( WIFI_UART->ISR & USART_ISR_PE ) {
//    WIFI_UART->ICR = USART_ICR_PECF;
////    rx_index = 0;
//  }
//  if( WIFI_UART->ISR & USART_ISR_FE ) {
//    WIFI_UART->ICR = USART_ICR_FECF;
////    rx_index = 0;    
//  }
//  if( WIFI_UART->ISR & USART_ISR_ORE ) {
//    WIFI_UART->ICR = USART_ICR_ORECF; 
////    rx_data.index = 0;
//  }  
//}

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

#include "esp8266_task.h"
#include "cli.h"
#include "esp_at.h"

#define MAX_NUM_TRIAL                       10

#define SSID                                "elAlex"
#define PASSWORD                            "K8ei5a5ejh"

#define HOST_ADDRESS                        "192.168.0.14"
#define HOST_PORT                           52222

static struct pt pt_wifi, pt_at;
static uint32_t time;
static int16_t at_status;
static uint8_t repeat_counter;

uint8_t IpAddress[15];

ESP8266_ConnectionInfoTypeDef ConnectionInfo;
ESP8266_StatusTypeDef Result = ESP8266_OK;

char mess[32];

/** Инициализация задачи обслуживания модуля ESP8266
*/
void init_wifi_task( void ) {
  init_cli_task();
  PT_INIT( &pt_wifi );
}

/** Задача обслуживания модуля ESP8266
*/
int wifi_task( void ) {
  
  cli_task();
  
  PT_BEGIN( &pt_wifi );

  PT_INIT( &pt_at );
  
  ESP_RESET_LOW();  
  ESP_PD_LOW();
  
  time = HAL_GetTick();
  PT_YIELD_UNTIL( &pt_wifi, ( HAL_GetTick() - time ) > 100 );    

  init_ESP_UART();  
  
  ESP_RESET_HI();  
  ESP_PD_HI();
  
  time = HAL_GetTick();
  PT_YIELD_UNTIL( &pt_wifi, ( HAL_GetTick() - time ) > 100 ); 

  PT_WAIT_THREAD( &pt_wifi, at_wait( &pt_at, "ready", 5000, &at_status ));
  if( at_status != AT_OK ) {
    PT_RESTART( &pt_wifi );
  }   
  
  repeat_counter = 3;
  while( repeat_counter-- ) {
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "ATE0\r\n", "OK", 1000, &at_status ));
    if( at_status == AT_OK ) break;
    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
  }
 
  repeat_counter = 3;
  while( repeat_counter-- ) {
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "AT+CWMODE=1\r\n", "OK", 1000, &at_status ));
    if( at_status == AT_OK ) break;
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "AT+CWMODE=1\r\n", "no change", 1000, &at_status )); 
    if( at_status == AT_OK ) break;    
    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
  }  
 
  repeat_counter = 3;
  while( repeat_counter-- ) {
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "AT+CIPMUX=0\r\n", "OK", 1000, &at_status ));
    if( at_status == AT_OK ) break;
    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
  }  

  repeat_counter = 3;
  while( repeat_counter-- ) {
    char at_cmd[64];
    sprintf( at_cmd, "AT+CWJAP=\"%s\",\"%s\"%c%c", SSID, PASSWORD, '\r', '\n' );
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, at_cmd, "OK", 10000, &at_status ));
    if( at_status == AT_OK ) break;
    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
  }   
  
//  /* Reset the IP address field to 0 */
//  memset( IpAddress, '\0', 15 );
//  /* Access point joined: start getting IP address */
//  ESP8266_GetIPAddress( ESP8266_STATION_MODE, IpAddress );
//  HAL_Delay( 1000 );

  repeat_counter = 3;
  while( repeat_counter-- ) {
    char at_cmd[64];
    
    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
    sprintf( at_cmd, "AT+CIPSTART=\"TCP\",\"%s\",%lu\r\n", HOST_ADDRESS, HOST_PORT );
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, at_cmd, "Linked", 10000, &at_status ));
    if( at_status == AT_OK ) break;    
  }
  
//  repeat_counter = 3;
//  while( repeat_counter-- ) {
//    char* mess = "Hello friend\r\n";
//    PT_WAIT_THREAD( &pt_wifi, at_send_data( &pt_at, ( uint8_t* )mess, strlen( mess ), 10000, &at_status ));
//    if( at_status == AT_OK ) break;
//    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
//  }
 
  repeat_counter = 3;
  while( repeat_counter-- ) {
    char at_cmd[64];
    
    if( repeat_counter == 0 ) {
      PT_RESTART( &pt_wifi );
    }
    
    sprintf( at_cmd, "AT+CIPSEND=%lu\r\n", strlen( mess ) );
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, at_cmd, AT_SEND_PROMPT_STRING, 1000, &at_status ));
    if( at_status != AT_OK ) continue;
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "Hello friend\r\n", "SEND OK", 1000, &at_status ));
    if( at_status == AT_OK ) break;
  }  
  
//    memset( AtCmd, '\0', MAX_AT_CMD_SIZE );
//    sprintf( ( char * )AtCmd, "AT+CIPSEND=%lu\r\n", Length );
//
//    /* The CIPSEND command doesn't have a return command
//       until the data is actually sent. Thus we check here whether
//       we got the '>' prompt or not. */
//    Ret = runAtCmd( AtCmd, strlen( ( char * )AtCmd ), ( uint8_t* )AT_SEND_PROMPT_STRING );
//
//    /* Return Error */
//    if ( Ret != ESP8266_OK ) {
//      return ESP8266_ERROR;
//    }
//
//    /* Wait before sending data. */
//    tickStart = HAL_GetTick();
//    while ( HAL_GetTick() - tickStart < 500 ) {
//    }
//
//    /* Send the data */
//    Ret = runAtCmd( Buffer, Length, ( uint8_t* )AT_SEND_OK_STRING );
//  }  
  
//  memset( AtCmd, '\0', MAX_AT_CMD_SIZE );
//  sprintf( ( char * )AtCmd, "AT+CIPSTART=\"TCP\",\"%s\",%lu\r\n", ( char * )connection_info->ipAddress, connection_info->port );
//
//  /* Send the CIPSTART command */
//  Ret = runAtCmd( AtCmd, strlen( ( char * )AtCmd ), ( uint8_t* )AT_OK_STRING );  
  
//  Trial = 0;
//  memset( &ConnectionInfo, '\0', sizeof ( ESP8266_ConnectionInfoTypeDef ) );
//
//  ConnectionInfo.connectionType = ESP8266_TCP_CONNECTION;
//  ConnectionInfo.ipAddress = ( uint8_t * )HOST_ADDRESS;
//  ConnectionInfo.isServer = ESP8266_FALSE;
//  ConnectionInfo.port = HOST_PORT;
//
//  /* Wait for communication establishment */
//  while ( ESP8266_EstablishConnection( &ConnectionInfo ) != ESP8266_OK ) {
//
//    if ( Trial == MAX_NUM_TRIAL ) {
//      break;
//    }
//  }
//  
//  sprintf( mess, "Hello XEON2678\r\n" );
//  Result = ESP8266_SendData(( uint8_t* )mess, strlen(( char * )mess ) );
//
//  /* In case of error, quit the Access point */
//  if ( Result != ESP8266_OK ) {
//    /* Deinitialize the WiFi module */
//    ESP8266_DeInit();
//    /* Call the error Handler */
////    ErrorHandler();
//  }
// 
////  char* mess = "I am work\r\n";
//  sprintf( mess, "I am work\r\n" );
//  Result = ESP8266_SendData(( uint8_t* )mess, strlen(( char * )mess ) );
//
//  /* In case of error, quit the Access point */
//  if ( Result != ESP8266_OK ) {
//    /* Deinitialize the WiFi module */
//    ESP8266_DeInit();
//    /* Call the error Handler */
////    ErrorHandler();
//  }  
  
  while( 1 ) {
    PT_YIELD( &pt_wifi );
  }
  PT_END( &pt_wifi );
}



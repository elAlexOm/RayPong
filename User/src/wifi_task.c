
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
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "AT+CWSAP=\"RayPongWiFi\",\"45931111\",2,2\r\n", "OK", 10000, &at_status ));
    if( at_status == AT_OK ) break;
    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
  }  
  
//  repeat_counter = 3;
//  while( repeat_counter-- ) {
//    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "AT+CIPAP_DEF=\"192.168.4.1\",\"192.168.4.1\",\"255.255.255.0\"\r\n", "OK", 10000, &at_status ));
//    if( at_status == AT_OK ) break;
//    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
//  }
//  
  repeat_counter = 3;
  while( repeat_counter-- ) {
//    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "AT+CWMODE=2\r\n", "OK", 1000, &at_status ));
//    if( at_status == AT_OK ) break;
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "AT+CWMODE=2\r\n", "no change", 1000, &at_status )); 
    if( at_status == AT_OK ) break;    
    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
  }  
 
  repeat_counter = 3;
  while( repeat_counter-- ) {
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "AT+CIPMUX=1\r\n", "OK", 1000, &at_status ));
    if( at_status == AT_OK ) break;
    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
  }  

  repeat_counter = 3;
  while( repeat_counter-- ) {
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "AT+CIPSERVER=1\r\n", "OK", 1000, &at_status ));
    if( at_status == AT_OK ) break;
    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
  }   

  repeat_counter = 3;
  while( repeat_counter-- ) {
    PT_WAIT_THREAD( &pt_wifi, at_send_command( &pt_at, "AT+CIPAP=?\r\n", "OK", 10000, &at_status ));
    if( at_status == AT_OK ) break;
    if( repeat_counter == 0 ) PT_RESTART( &pt_wifi );
  }
  
  while( 1 ) {
    PT_YIELD( &pt_wifi );
  }
  PT_END( &pt_wifi );
}



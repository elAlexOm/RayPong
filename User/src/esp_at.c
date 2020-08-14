
/** \file   esp_at.c
*   \author Alex
*   \date   12.08.2020
*/

#include "esp_at.h"


#include "main.h"
#include "cli.h"
#include "esp8266_task.h"

#include <stdio.h>
#include <string.h>

static uint32_t at_delay;
static char* ret;

void at_cb( void* str, uint32_t len ) {
  ret = str;  
}

void cli_ev_input_data_handler( void* str, uint32_t len ) {
}

int at_send_command( struct pt* pt, char* at_command, char* answer, uint32_t timeout, int16_t* result ) {
  PT_BEGIN( pt );
  
  ret = NULL;
  cli_register_answer( answer, at_cb );
  
  if( NULL != at_command ) {
    uint16_t size;
    char* buf = ( char* )get_tx_buffer( &size );
    sprintf( buf, at_command );
    
    start_tx_data(( uint8_t* )buf, strlen( buf ));
  }
  
  at_delay = HAL_GetTick();
  while(( HAL_GetTick() - at_delay ) < timeout ) {
    if( NULL != ret ) {
      *result = AT_OK;
      PT_EXIT( pt );      
    }
    PT_YIELD( pt );
  }
  *result = AT_TIMEOUT_RX;
  PT_EXIT( pt );
  PT_END( pt );
}

int at_wait( struct pt* pt, char* answer, uint32_t timeout, int16_t* result ) {
  return at_send_command( pt, NULL, answer, timeout, result );
}

int at_send_data( struct pt* pt, uint8_t* data, uint32_t length, uint32_t timeout, int16_t* result ) {
  PT_BEGIN( pt );
  
  ret = NULL;
  cli_register_answer( AT_SEND_PROMPT_STRING, at_cb );  
  
  *result = AT_TIMEOUT_RX;
  PT_EXIT( pt );
  PT_END( pt );  
}

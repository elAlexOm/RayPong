
/** \file   wifi_task.c
*   \author Alex
*   \date   22.07.2020
*
* Задача обслуживания модуля ESP8266
*/

#include "wifi_task.h"

#include "pt.h"

static struct pt pt_wifi;

void init_wifi_task( void ) {
  PT_INIT( &pt_wifi );
}

int wifi_task( void ) {
  PT_BEGIN( &pt_wifi );
  
  while( 1 ) {
    PT_YIELD( &pt_wifi );
  }
  PT_END( &pt_wifi );
}
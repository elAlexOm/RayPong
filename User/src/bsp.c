
/** \file   bsp.c
*   \author Alex
*   \date   22.07.2020
*/

#include "bsp.h"

#include "stm32f0xx_hal.h"
#include "main.h"

/** Подхват кнопки включения питания
*/
void bsp_power_lock( void ) { 
//  if( !IS_POWER_ON()) {
//    while( 1 );
//  }
//  V33_ENABLE();
  V50_ENABLE();
}

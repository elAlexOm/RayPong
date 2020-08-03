
/** \file   control_task.c
*   \author Alex
*   \date   22.07.2020
*
* Задача обслуживания кнопок/светодиодов
*/

#include "control_task.h"
#include "bsp.h"

#include <stdint.h>
#include "tim.h"

#include "pt.h"

static struct pt pt_control;

typedef struct {
  uint8_t led_group1;
  uint8_t led_group2;
  uint8_t buttons;
} controls_s;

controls_s controls = {
  .led_group1 = 0xff,
  .led_group2 = 0x55
};

controls_s control_temp;
static uint32_t time;

/** Инициализация задачи чтения/записи кнопок/светодиодов
*/
void init_control_task( void ) {
  PT_INIT( &pt_control );
}

/** Задача чтения/записи кнопок/светодиодов
*/
int control_task( void ) {
  PT_BEGIN( &pt_control );
 
  IND_RESET_HI();
  
  time = HAL_GetTick();
  while( 1 ) {
    PT_YIELD_UNTIL( &pt_control, ( HAL_GetTick() - time ) > 20 );
    time = HAL_GetTick();
    REG_LATCH_OFF();    
    uint8_t leds[2] = { controls.led_group1, controls.led_group2 };
    uint8_t keys = 0xff;
    REG_LATCH_ON();
    for( uint8_t i = 0; i < 8; i++ ) {

      keys <<= 1;         
      keys |= GET_SDATA3(); 
      
      CLK_LOW();
      SET_SDATA1( leds[0] & 0x01 );
      SET_SDATA2( leds[1] & 0x01 );
      
      CLK_HI();
      leds[0] >>= 1;
      leds[1] >>= 1;
      
    }
    keys = ~keys;
    if( controls.buttons != keys ) {
      controls.buttons = keys;      
    }    
  }
  PT_END( &pt_control );
}

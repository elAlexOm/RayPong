
/** \file   control_task.c
*   \author Alex
*   \date   22.07.2020
*
* Задача обслуживания кнопок/светодиодов
*/

#include "control_task.h"
#include "bsp.h"

#include <stdint.h>

#include "pt.h"

static struct pt pt_control;
static uint8_t run;

typedef struct {
  uint8_t led_group1;
  uint8_t led_group2;
  uint8_t buttons;
} controls_s;

static controls_s control;
static controls_s control_temp;

/** Инициализация задачи чтения/записи кнопок/светодиодов
*/
void init_control_task( void ) {
  PT_INIT( &pt_control );
}

/** Задача чтения/записи кнопок/светодиодов
*/
int control_task( void ) {
  PT_BEGIN( &pt_control );
  
  while( 1 ) {
    PT_YIELD_UNTIL( &pt_control, run != 0 );
    control_temp = control;
    control_temp.buttons = 0;
    REG_LATCH_ON();
    while( run-- ) {
      
      REG_CLOCK_OFF();      
      SET_SDATA1( control_temp.led_group1 & 0x01 );
      SET_SDATA2( control_temp.led_group2 & 0x01 );
      control_temp.led_group1 >>= 1;
      control_temp.led_group2 >>= 1;      
      REG_CLOCK_ON();
      control_temp.buttons <<= 1;
      control_temp.buttons |= GET_SDATA3(); 
    }
  }
  PT_END( &pt_control );
}

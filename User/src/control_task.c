
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

uint8_t ind_leds[2];

controls_s controls = { 0 };

static uint32_t time;
static uint32_t power_off_timer = 0;

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
    uint8_t leds[2] = { ind_leds[0], ind_leds[1] };
    uint8_t buttons = 0xff;
    REG_LATCH_ON();
    for( uint8_t i = 0; i < 8; i++ ) {

      buttons <<= 1;         
      buttons |= GET_SDATA3(); 
      
      CLK_LOW();
      SET_SDATA1( leds[0] & 0x01 );
      SET_SDATA2( leds[1] & 0x01 );
      
      CLK_HI();
      leds[0] >>= 1;
      leds[1] >>= 1;
      
    }
    buttons = ~buttons;
    if( controls.buttons != buttons ) {
      uint8_t change_button = buttons ^ controls.buttons;
      controls.button_on    =  buttons & change_button;
      controls.button_off   = controls.buttons & change_button; 
      
      if( change_button & BTN_RUN ) {
        btn_run_handler( &controls );
      }
 
      if( change_button & BTN_SOUND ) {
        btn_sound_handler( &controls );
      }      
      
      if( change_button & BTN_ONE_PLAYER ) {
        btn_one_player_handler( &controls );
      }
 
      if( change_button & BTN_MULTY_PLAYER ) {
        btn_multy_player_handler( &controls );
      }
      
      if( change_button & BTN_5 ) {
        btn_btn5_handler( &controls );
      }
 
      if( change_button & BTN_6 ) {
        btn_btn6_handler( &controls );
      }
      
      if( change_button & BTN_MINUS ) {
        btn_minus_player_handler( &controls );
      }
 
      if( change_button & BTN_PLUS ) {
        btn_plus_player_handler( &controls );
      }      
      controls.buttons      = buttons;      
    }    
  }
  PT_END( &pt_control );
}

void debonce_timer( void ) {
  
}

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin ) {
  if( GPIO_Pin == PWR_ON_Pin ) {    
//    if( IS_POWER_ON()) {    // Rising - кнопка нажата
//      LED_POWER_OFF();
//      V50_DISABLE();
//      V33_DISABLE();
//    }
//    else {                  // Falling - кнопка отпущена
//      
//    }
  }
  if( GPIO_Pin == PLEER_1_Pin ) {
    btn_player1_handler();
  }
  if( GPIO_Pin == PLEER_2_Pin ) {
    btn_player2_handler();
  }  
}

__weak void btn_run_handler( controls_s* pbutton ) {
}

__weak void btn_sound_handler( controls_s* pbutton ) {
}

__weak void btn_one_player_handler( controls_s* pbutton ) {
}

__weak void btn_multy_player_handler( controls_s* pbutton ) {
}

__weak void btn_btn5_handler( controls_s* pbutton ) {
}

__weak void btn_btn6_handler( controls_s* pbutton ) {
}

__weak void btn_minus_player_handler( controls_s* pbutton ) {
}

__weak void btn_plus_player_handler( controls_s* pbutton ) {
}

__weak void btn_player1_handler( void ) {
}

__weak void btn_player2_handler( void ) {
}

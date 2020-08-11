
/** \file   control_task.h
*   \author Alex
*   \date   22.07.2020
*
* Задача обслуживания кнопок/светодиодов
*/

#pragma once

#include <stdint.h>

#define BTN_RUN                     0x01
#define BTN_SOUND                   0x02
#define BTN_ONE_PLAYER              0x04
#define BTN_MULTY_PLAYER            0x08
#define BTN_5                       0x10
#define BTN_6                       0x20
#define BTN_MINUS                   0x40
#define BTN_PLUS                    0x80

typedef struct {
  uint8_t buttons;
  uint8_t button_on;
  uint8_t button_off;
} controls_s;

/** Инициализация задачи чтения/записи кнопок/светодиодов
*/
void init_control_task( void );

/** Задача чтения/записи кнопок/светодиодов
*/
int control_task( void );

void timControlCompareIrq( void );

void timControlCompleteIrq( void );

void btn_run_handler( controls_s* pbutton );
void btn_sound_handler( controls_s* pbutton );
void btn_one_player_handler( controls_s* pbutton );
void btn_multy_player_handler( controls_s* pbutton );
void btn_btn5_handler( controls_s* pbutton );
void btn_btn6_handler( controls_s* pbutton );
void btn_minus_player_handler( controls_s* pbutton );
void btn_plus_player_handler( controls_s* pbutton );

void btn_player1_handler( void );
void btn_player2_handler( void );

void debonce_timer( void );

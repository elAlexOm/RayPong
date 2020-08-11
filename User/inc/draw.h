
/** \file   draw.h
*   \author Alex
*   \date   25.07.2020
*
* Различные функции для работы с графикой
*/

#pragma once

#include "stdint.h"

#define WS_LED_COUNT              85                //!< Количество светодиодов в ленте

typedef enum {
  mvStay = 0,
  mvLeft = 1,
  mvRight = 2
} move_e;

typedef union {
  struct {
    uint8_t blue;
    uint8_t red;
    uint8_t green;
  };
  uint32_t  bits;
} pixel_t;

typedef struct {
  uint8_t  length;
  pixel_t* buffer;
  pixel_t  background;
  uint8_t  flags;
} screen_t;

//typedef struct line_t;

typedef struct line_t {
  uint16_t  start;
  uint16_t  end;
  uint16_t  position;
  uint16_t  length;
  pixel_t*  buffer;
  move_e    move;
  void ( *init )( void );
  void ( *draw )( screen_t*, struct line_t* );
} line_t;

/** Обработчик события обновления экрана
*/
void update_screen( void );

/** Инициализация задачи обслуживания экрана
*/
void init_draw_task( void );

/** Задача обслуживания экрана
*/
int draw_task( void );

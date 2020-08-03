
/** \file   draw.c
*   \author Alex
*   \date   25.07.2020
*
* Различные функции для работы с графикой
*/

#include "draw.h"

#include "ws2812_task.h"

typedef struct {
  uint8_t   length;
  pixel_t* buffer;
  
} screen_t;

screen_t screen = {
  .length = 85,
  .buffer = ws_string,  
};

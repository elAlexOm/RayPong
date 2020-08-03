
/** \file   draw.h
*   \author Alex
*   \date   25.07.2020
*
* Различные функции для работы с графикой
*/

#pragma once

#include "stdint.h"

#define WS_LED_COUNT              85                //!< Количество светодиодов в ленте

typedef union {
  struct {
    uint8_t blue;
    uint8_t red;
    uint8_t green;
  };
  uint32_t  bits;
} pixel_t;

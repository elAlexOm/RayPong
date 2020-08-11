
/** \file   bsp.h
*   \author Alex
*   \date   22.07.2020
*/

#pragma once

#include <stdint.h>

#include "main.h"

#define IS_POWER_ON()                     (( PWR_ON_GPIO_Port->IDR & PWR_ON_Pin ) ? 1 : 0 )

#define V33_ENABLE()                      { EN_3V3_GPIO_Port->BSRR  = EN_3V3_Pin; }
#define V33_DISABLE()                     { EN_3V3_GPIO_Port->BRR   = EN_3V3_Pin; }

#define V50_ENABLE()                      { EN_5V0_GPIO_Port->BSRR  = EN_5V0_Pin; }
#define V50_DISABLE()                     { EN_5V0_GPIO_Port->BRR   = EN_5V0_Pin; }

#define REG_CLOCK_ON()                    { CLK_GPIO_Port->BSRR     = CLK_Pin; }
#define REG_CLOCK_OFF()                   { CLK_GPIO_Port->BRR      = CLK_Pin; }

#define LED_POWER_ON()                    { LED_PWR_GPIO_Port->BSRR = LED_PWR_Pin; }
#define LED_POWER_OFF()                   { LED_PWR_GPIO_Port->BRR  = LED_PWR_Pin; }

#define SET_SDATA1( X )                   { if( X ) S_DATA1_GPIO_Port->BSRR = S_DATA1_Pin; else S_DATA1_GPIO_Port->BRR = S_DATA1_Pin; }
#define SET_SDATA2( X )                   { if( X ) S_DATA2_GPIO_Port->BSRR = S_DATA2_Pin; else S_DATA2_GPIO_Port->BRR = S_DATA2_Pin; }
#define GET_SDATA3()                      (( S_DATA3_GPIO_Port->IDR & S_DATA3_Pin ) ? 1 : 0 )
#define CLK_LOW()                         { CLK_GPIO_Port->BSRR     = CLK_Pin; }
#define CLK_HI()                          { CLK_GPIO_Port->BRR      = CLK_Pin; }
#define REG_LATCH_ON()                    { LATCH_GPIO_Port->BSRR   = LATCH_Pin; }
#define REG_LATCH_OFF()                   { LATCH_GPIO_Port->BRR    = LATCH_Pin; }
#define REG_LATCH_TOOGLE()                { LATCH_GPIO_Port->ODR   ^= LATCH_Pin; }

#define IND_RESET_LOW()                   { CLEAR_GPIO_Port->BRR    = CLEAR_Pin; }
#define IND_RESET_HI()                    { CLEAR_GPIO_Port->BSRR   = CLEAR_Pin; }

#define ESP_RESET_LOW()                   { ESP_RST_GPIO_Port->BRR  = ESP_RST_Pin; }
#define ESP_RESET_HI()                    { ESP_RST_GPIO_Port->BSRR = ESP_RST_Pin; }

#define ESP_PD_LOW()                      { ESP_PD_GPIO_Port->BRR   = ESP_PD_Pin; }
#define ESP_PD_HI()                       { ESP_PD_GPIO_Port->BSRR  = ESP_PD_Pin; }

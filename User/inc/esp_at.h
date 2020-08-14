
/** \file   esp_at.h
*   \author Alex
*   \date   12.08.2020
*/

#pragma once

#include "stdint.h"
#include "pt.h"

#define AT_SEND_PROMPT_STRING   ">"                     //"OK\r\n>"

#define AT_OK                 0
#define AT_TIMEOUT_TX         1
#define AT_TIMEOUT_RX         2

int at_send_command( struct pt* pt, char* at_command, char* answer, uint32_t timeout, int16_t* result );

int at_wait( struct pt* pt, char* answer, uint32_t timeout, int16_t* result );

int at_send_data( struct pt* pt, uint8_t* data, uint32_t length, uint32_t timeout, int16_t* result );

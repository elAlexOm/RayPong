
/** \file   cli.h
*   \author Alex
*   \date   12.08.2020
* 
* Реализация CLI для работы с ESP на STM32
* Модуль разбивает поток на лексемы и отдаёт их обработчику - token_handler
* Используется библиотека protothreads Adam Dunkels
* tab - 2
*/

#include "cli.h"

#include "pt.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MACH_SYMBOL                 '\n'
#define MAX_TOKEN_SIZE              64

//typedef struct {
//  char* at;
//  bool  enable;  
//  void ( *callback )( void* str, uint32_t len );
//} at_answer_s;

//at_answer_s at_answer[] = {
//  { "OK",     true,   NULL },
//  { "IPD+",   true,   cli_ev_input_data_handler },
//  { "",       true,   NULL } //cli_ev_default_handler }
//};

static uint16_t size = 0;

static char token[MAX_TOKEN_SIZE];

static struct pt pt_cli;

/** Запуск задачи обработки сообщений
* \param  count   - количество символов в буффере
*/
void cli_start( uint16_t count ) {
  if( size == 0 ) {
    size = count;
  }
}

/** Инициализация задачи обработки сообщений
*/
void init_cli_task( void ) {
  PT_INIT( &pt_cli );
}

/** Задача разбора сообщений на лексемы
* \return     - статус процесса
*/
int cli_task( void ) {
  PT_BEGIN( &pt_cli );
  
  while( 1 ) {
    PT_YIELD_UNTIL( &pt_cli, size != 0 );
    uint16_t k = 0;
    do {
      size--;
      token[k] = get_next_char();
      if(( token[k] == MACH_SYMBOL ) || ( size == 0 )) {
        token[k] = 0;
        k = 0;
        token_handler( token, strlen( token ));
      }
      else if( k == ( sizeof( token ) - 1 )) {
        token[k+1] = 0;
        k = 0;
        token_handler( token, strlen( token ));
      }
      else {
        k++;
      }
    } while( size );
  }  
  PT_END( &pt_cli );
}

__weak void token_handler( void* str, uint32_t len ) {
}

#undef MACH_SYMBOL
#undef MAX_TOKEN_SIZE


/** \file   draw.c
*   \author Alex
*   \date   25.07.2020
*
* Различные функции для работы с графикой
*/

#include "draw.h"

#include "ws2812_task.h"
#include "control_task.h"

#include "pt.h"

void init_line1( void );
void draw_line( screen_t* pscreen, line_t* pline );
void draw_background( screen_t* pscreen );

static struct pt pt_draw;

uint8_t start_update_screen = 0;

screen_t screen = {
  .length     = 85,
  .buffer     = ws_string,
  .background = { .blue = 0, .red = 0, .green = 0 },
  .flags      = 0,
};

pixel_t line1_buffer[5];
line_t  line1 = {
  .start      = 0,
  .end       = 34,
  .position   = 0,
  .length     = sizeof( line1_buffer ) / sizeof( pixel_t ),
  .buffer     = line1_buffer,
  .move       = mvRight,
  .init       = init_line1,
  .draw       = draw_line
};

void init_line1( void ) {
  for( uint8_t i = 0; i < line1.length; i++ ) {
    line1.buffer[i].red = 255 - i * 10;;
    line1.buffer[i].green = 0;
    line1.buffer[i].blue = 160 + i * 10;
  }
}

void draw_line( screen_t* pscreen, line_t* pline ) {
  switch( line1.move ) {
    case mvLeft : {
      if( pline->position > pline->start ) {
        pline->position--;
      }
      else {
        pline->move = mvRight;
        pline->position++;
      }
      break;
    }
    case mvRight : {
      if(( pline->position + pline->length ) < pline->end ) {
        pline->position++;
      }
      else {
        pline->move = mvLeft;
        pline->position--;
      }      
      break;      
    }  
  }
  
  pixel_t* p = &pscreen->buffer[pline->position];

  for( uint8_t i = 0; i < pline->length; i++, p++ ) {
    p->bits = pline->buffer[i].bits;
  }  
}

void draw_background( screen_t* pscreen ) {
  static uint8_t counter = 10;
  
  pixel_t* p = pscreen->buffer;
  for( uint16_t i = 0; i < pscreen->length; i++,p++ ) {
    p->bits = pscreen->background.bits;
  }
  if( !--counter ) {
    counter = 10;
    switch( pscreen->flags ) {
      case 0 : {
        if( pscreen->background.green++ >= 5 ) {
          pscreen->flags = 1;
        }
        break;
      }
      case 1 : {
        if( --pscreen->background.green == 0 ) {
          pscreen->flags = 2;
        }
        break;
      }
      case 2 : {
        if( pscreen->background.blue++ >= 5 ) {
          pscreen->flags = 3;
        }
        break;
      }
      case 3 : {
        if( --pscreen->background.blue == 0 ) {
          pscreen->flags = 0;
        }
        break;
      }      
    }
  }
}

void update_screen( void ) {
  start_update_screen = 1;
}

/** Инициализация задачи обслуживания экрана
*/
void init_draw_task( void ) {
  PT_INIT( &pt_draw );
}

/** Задача обслуживания экрана
*/
int draw_task( void ) {
  PT_BEGIN( &pt_draw ); 
  
  line1.init();
  
  while( 1 ) {
    PT_WAIT_WHILE( &pt_draw, start_update_screen == 0 ); 
    start_update_screen = 0;
    
    draw_background( &screen );
    draw_line( &screen, &line1 );
    
    
  }
  PT_END( &pt_draw );
}

void btn_minus_player_hundler( controls_s* pbutton ) {
  if( pbutton->button_on & BTN_MINUS ) {
    for( uint8_t i = 0; i < line1.length; i++ ) {
      line1.buffer[i].blue  += 5;
      line1.buffer[i].red   += 5;
      line1.buffer[i].green -= 5;
    }
  }
}

void btn_plus_player_hundler( controls_s* pbutton ) {
  if( pbutton->button_on & BTN_PLUS ) {
    for( uint8_t i = 0; i < line1.length; i++ ) {
      line1.buffer[i].blue  -= 5;
      line1.buffer[i].red   -= 5;
      line1.buffer[i].green += 5;
    }    
  }  
}

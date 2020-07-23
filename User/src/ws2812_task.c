
/** \file   ws2812_task.c
*   \author Alex
*   \date   22.07.2020
*
* Задача обслуживания ленты WS2812
*/

#include "ws2812_task.h"

#include "pt.h"

#include "main.h"
#include "tim.h"
#include "bsp.h"

#define PWM_BUFFER_SIZE           256

#define WS_LED_COUNT              45
#define WS_PWM_SIZE               24

#define LED_TIMER_PERIOD          69
#define PWM_1                     ( LED_TIMER_PERIOD / 3 )
#define PWM_0                     ( 2 * LED_TIMER_PERIOD / 3 )

#define LED_Timer                 TIM16

typedef union {
  struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
  };
  uint32_t  bits;
} ws_led_s;

static uint8_t  led_run = 0;                                        //!< Флаг запуска обновления строки

static ws_led_s ws_string[WS_LED_COUNT];                            //!< Буфер строки в цветах светодиодов
static uint8_t pwm_buffer[WS_LED_COUNT * WS_PWM_SIZE] = { PWM_1 };      //!< Буфер строки в параметрах таймера
static uint32_t pwm_index = 0;                                      //!< Индекс записываемого светодиода

static struct pt pt_ws2812;

/** Запуск обновления экрана
*/
void ws_start_update( void ) {
  led_run = 1;
}

/** Преобразование из параметров цвета в параметры таймера
* \param[in]  led     - структура содержащая цвета светодиода
* \param[out] buffer  - указатель для буфера шим таймера
*/
void ws_led_2_pwm( ws_led_s led, uint8_t* buffer ) {
  uint32_t pwm = led.bits;
  
  for( uint8_t i = 0; i < 24; i++ ) {
    buffer[i] = PWM_0;
    if( pwm & 0x00000001 ) buffer[i] = PWM_1;
  }
}

/** Инициализация задачи обслуживания ленты
*/
void init_ws2812_task( void ) {
  PT_INIT( &pt_ws2812 );
}

/** Задача обслуживания ленты
*/
int ws2812_task( void ) {
  PT_BEGIN( &pt_ws2812 ); 
  
  MX_TIM16_Init();
  
  for( uint8_t i = 0; i < WS_LED_COUNT; i++ ) {
    ws_string[i].blue   = 255 - 5*i;
    ws_string[i].green  = 255 - 5*i;
    ws_string[i].red    = 255 - 5*i;    
  }
  
  for( uint32_t i = 0; i < sizeof( pwm_buffer ); i++ ) {
    pwm_buffer[i] = i;
  }
  
  while( 1 ) {
    PT_WAIT_WHILE( &pt_ws2812, led_run == 0 ); 
    led_run = 0;
//    for( uint8_t i = 0; i < WS_LED_COUNT; i++ ) {
//      ws_led_2_pwm( ws_string[i], &pwm_buffer[i*3] );
//      
//    }
    pwm_index = 0;
    LED_Timer->ARR    = LED_TIMER_PERIOD;
    LED_Timer->BDTR   = TIM_BDTR_MOE;
    LED_Timer->CCER   = TIM_CCER_CC1E;
    LED_Timer->SR     = 0;
    LED_Timer->DIER   = 1;
    LED_Timer->CR1    = 1;
  }
  PT_END( &pt_ws2812 );
}

/** Прерывание таймера светодиодов
*/
void ws_timer_irq( void ) {
  LED_Timer->SR = 0;
  
//  REG_LATCH_TOOGLE();
  
  if( ++pwm_index >= ( WS_LED_COUNT * WS_PWM_SIZE )) {
    LED_Timer->CR1 = 0;
  }
  else {  
    LED_Timer->CCR1 = pwm_buffer[pwm_index];
  }
}

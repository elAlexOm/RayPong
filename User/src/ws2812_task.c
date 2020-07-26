
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
#include "draw.h"


#define WS_PWM_SIZE               24

#define LED_TIMER_PERIOD          59
#define PWM_0                     ( LED_TIMER_PERIOD / 3 )
#define PWM_1                     ( 2 * LED_TIMER_PERIOD / 3 )

#define LED_Timer                 TIM16
#define hLED_Timer                htim16

static uint8_t  led_run = 0;                                        //!< Флаг запуска обновления строки

ws_led_s ws_string[WS_LED_COUNT];                                   //!< Буфер строки в цветах светодиодов
static uint8_t pwm_buffer[WS_LED_COUNT * WS_PWM_SIZE] = { PWM_1 };  //!< Буфер строки в параметрах таймера
static uint32_t pwm_index = 0;                                      //!< Индекс записываемого светодиода

static struct pt pt_ws2812;

static uint8_t led_pwm0;
static uint8_t led_pwm1;

static int32_t led_counter = 0;
static uint32_t direct = 1;
static uint32_t led_max = 80;

ws_led_s led_color = { .blue = 100, .red = 200, .green = 50 };

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
    buffer[i] = led_pwm0;
    if( pwm & 0x00100000 ) buffer[i] = led_pwm1;
    pwm <<= 1;
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
  
  led_pwm0 = hLED_Timer.Instance->ARR / 3;
  led_pwm1 = led_pwm0 * 2;
    
  for( uint8_t i = 0; i < WS_LED_COUNT; i++ ) {
    ws_string[i].bits = 0;   
  }
 
  while( 1 ) {
    PT_WAIT_WHILE( &pt_ws2812, led_run == 0 ); 
    led_run = 0;
    
    if( direct ) {
      ws_string[led_counter++].bits = 0;
      led_color.bits += 10;
      ws_string[led_counter].bits = led_color.bits;
      if( led_counter >= led_max ) direct = 0;
    }
    else {
      ws_string[led_counter--].bits = 0;
      led_color.bits += 10;
      ws_string[led_counter].bits = led_color.bits;
      if( led_counter == 0 ) direct = 1;      
    }
    
    for( uint8_t i = 0; i < WS_LED_COUNT; i++ ) {
      ws_led_2_pwm( ws_string[i], &pwm_buffer[i*24] );
      
    }
    HAL_TIM_PWM_Start_DMA( &hLED_Timer, TIM_CHANNEL_1, ( uint32_t* )pwm_buffer, sizeof( pwm_buffer ));
  }
  PT_END( &pt_ws2812 );
}

/** Прерывание таймера светодиодов
*/
void ws_timer_irq( void ) {
  LED_Timer->SR = 0;
  
  if( ++pwm_index >= ( WS_LED_COUNT * WS_PWM_SIZE )) {
    LED_Timer->CR1 = 0;
  }
  else {  
    LED_Timer->CCR1 = pwm_buffer[pwm_index];
  }
}

/** Прерывание DMA по кончанию буфера
*/
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  LED_Timer->CR1 = 0;
}

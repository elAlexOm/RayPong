
/** \file   control_task.h
*   \author Alex
*   \date   22.07.2020
*
* Задача обслуживания кнопок/светодиодов
*/

#pragma once

/** Инициализация задачи чтения/записи кнопок/светодиодов
*/
void init_control_task( void );

/** Задача чтения/записи кнопок/светодиодов
*/
int control_task( void );

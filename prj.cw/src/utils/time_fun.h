#pragma once
#include <iostream>
#include <ctime>

/**
 * @brief Функция для создания объекта типа time_t на основе переданных значений года, месяца, дня, часа, минут и секунд.
 * 
 * @param y Год.
 * @param m Месяц.
 * @param d День.
 * @param h Час.
 * @param n Минута.
 * @param s Секунда.
 * @return time_t Созданный объект типа time_t.
 */
time_t getTime_t(int y, int m, int d, int h, int n, int s);

/**
 * @brief Функция форматирует время, представленное объектом time_t, в строку в формате 'гггг-мм-дд_чч:мм:сс'.
 * 
 * @param t Время в формате time_t.
 * @return std::string Отформатированная строка.
 */
std::string printDateTimeCsv(const time_t &t);

/**
 * @brief Функция форматирует время, представленное объектом time_t, в строку в формате 'гггг-мм-дд чч:мм:сс'.
 * 
 * @param t Время в формате time_t.
 * @return std::string Отформатированная строка.
 */
std::string printDateTimeDb(const time_t &t);

/**
 * @brief Функция форматирует текущее время в строку без разделителей в формате 'ггггммдд_ччммсс'.
 * 
 * @return std::string Отформатированная строка.
 */
std::string printDateTimeFileName();

/**
 * @brief Функция форматирует время, представленное объектом time_t, в строку в формате 'мм/дд/гггг чч:мм:сс'.
 * 
 * @param t Время в формате time_t.
 * @return std::string Отформатированная строка.
 */
std::string printDateTime(time_t t);

/**
 * @brief Функция форматирует текущее время в строку в формате 'мм/дд/гггг чч:мм:сс'.
 * 
 * @return std::string Отформатированная строка.
 */
std::string printDateTime();

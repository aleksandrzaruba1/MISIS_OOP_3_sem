#pragma once

#include <string>

/**
 * @brief Перечисление для определения регистра символов в шестнадцатеричном представлении.
 */
enum {
  upperhex, /**< Верхний регистр (A-F) */
  lowerhex  /**< Нижний регистр (a-f) */
};

/**
 * @brief Функция для генерации строки с шестнадцатеричным представлением данных.
 * 
 * @tparam Caps Указывает, должны ли символы быть в верхнем регистре (upperhex) или нижнем (lowerhex). По умолчанию lowerhex.
 * @tparam FwdIt Тип итератора для перебора данных.
 * @param first Итератор, указывающий на начало диапазона данных.
 * @param last Итератор, указывающий на конец диапазона данных.
 * @return std::string Строка с шестнадцатеричным представлением данных.
 */
template <bool Caps = lowerhex, typename FwdIt>
std::string hex_str(FwdIt first, FwdIt last) {
  // Утверждение о том, что тип данных в итераторе является однобайтовым
  static_assert(sizeof(typename std::iterator_traits<FwdIt>::value_type) == 1,
                "value_type must be 1 byte.");

  // Массив символов, используемых для представления шестнадцатеричных цифр
  constexpr const char *bytemap = Caps ?
                                  "0123456789abcdef" :
                                  "0123456789ABCDEF";

  // Создание строки, в которой будет храниться шестнадцатеричное представление
  std::string result(std::distance(first, last) * 2, '0');

  // Итерация по данным и формирование шестнадцатеричного представления
  auto pos = begin(result);
  while (first != last) {
    *pos++ = bytemap[ *first >> 4 & 0xF ]; // Получение старшей тетрады и выбор соответствующего символа
    *pos++ = bytemap[ *first++ & 0xF ];    // Получение младшей тетрады и выбор соответствующего символа
  }

  return result; // Возврат сформированной строки
}

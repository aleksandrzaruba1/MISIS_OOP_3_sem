#pragma once
#include <utility>

/**
 * @brief Класс для представления котировки
 * 
 * Класс quote_t представляет собой абстракцию для хранения информации о котировке, 
 * включающей в себя цену предложения (bid) и цену спроса (ask).
 */
class quote_t {
private:
  typedef double bid_t; ///< Тип для цены предложения
  typedef double ask_t; ///< Тип для цены спроса
  std::pair<bid_t, ask_t> quote_pair; ///< Пара значений bid и ask

public:
  /**
   * @brief Конструктор класса
   * 
   * Создает объект котировки с заданными значениями bid и ask.
   * 
   * @param bid Цена предложения
   * @param ask Цена спроса
   */
  quote_t(bid_t bid, ask_t ask) : quote_pair(std::make_pair(bid, ask)) {}

  /**
   * @brief Конструктор класса
   * 
   * Создает объект котировки, перемещая данные из переданной пары значений.
   * 
   * @param quote Пара значений bid и ask
   */
  quote_t(std::pair<bid_t, ask_t> &&quote) : quote_pair(std::move(quote)) {}

  /**
   * @brief Получить цену предложения (bid)
   * 
   * @return Цена предложения
   */
  bid_t bid() const { return quote_pair.first; }

  /**
   * @brief Получить цену спроса (ask)
   * 
   * @return Цена спроса
   */
  ask_t ask() const { return quote_pair.second; }
};

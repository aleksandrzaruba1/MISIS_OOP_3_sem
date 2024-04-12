#pragma once

#include "quote_t.h"
#include <string>

/**
 * @brief Класс BasicCoin представляет базовую информацию о монете на бирже.
 */
class BasicCoin {

private:
  unsigned id; /**< Уникальный идентификатор монеты. */
  std::string exchName; /**< Название биржи. */
  std::string coinName; /**< Название монеты. */
  double fees; /**< Размер комиссии. */
  bool hasShort; /**< Имеет ли монета возможность продажи. */
  bool isImplemented; /**< Флаг указывающий, реализована ли монета. */
  double bid; /**< Цена покупки монеты. */
  double ask; /**< Цена продажи монеты. */

public:
  /**
   * @brief Конструктор класса BasicCoin.
   * @param _id Уникальный идентификатор монеты.
   * @param _coinName Название монеты.
   * @param _isImplemented Флаг, указывающий, реализована ли монета.
   */
  BasicCoin(unsigned _id, std::string _coinName, bool _isImplemented)
      : id(_id), coinName(_coinName), isImplemented(_isImplemented) {}

  /**
   * @brief Метод обновления данных о монете.
   * @param quote Объект quote_t с информацией о монете.
   */
  void updateData(quote_t quote) {
    bid = quote.bid(); /**< Установить цену покупки монеты. */
    ask = quote.ask(); /**< Установить цену продажи монеты. */
  }

  /**
   * @brief Метод получения уникального идентификатора монеты.
   * @return Уникальный идентификатор монеты.
   */
  unsigned getId() const { return id; }

  /**
   * @brief Метод получения названия биржи.
   * @return Название биржи.
   */
  std::string getExchName() const { return exchName; }

  /**
   * @brief Метод получения названия монеты.
   * @return Название монеты.
   */
  std::string getCoinName() const { return coinName; }

  /**
   * @brief Метод получения размера комиссии.
   * @return Размер комиссии.
   */
  double getFees() const { return fees; }

  /**
   * @brief Метод проверки наличия возможности продажи монеты.
   * @return true, если монета имеет возможность продажи, в противном случае - false.
   */
  bool getHasShort() const { return hasShort; };

  /**
   * @brief Метод проверки, реализована ли монета.
   * @return true, если монета реализована, в противном случае - false.
   */
  bool getIsImplemented() const { return isImplemented; }

  /**
   * @brief Метод получения цены покупки монеты.
   * @return Цена покупки монеты.
   */
  double getBid() const { return bid; }

  /**
   * @brief Метод получения цены продажи монеты.
   * @return Цена продажи монеты.
   */
  double getAsk() const { return ask; }

  /**
   * @brief Метод получения средней цены монеты.
   * @return Средняя цена монеты.
   */
  double getMidPrice() const {
    return (bid > 0.0 && ask > 0.0) ? (bid + ask) / 2.0 : 0.0; /**< Вернуть среднюю цену монеты. */
  }
};

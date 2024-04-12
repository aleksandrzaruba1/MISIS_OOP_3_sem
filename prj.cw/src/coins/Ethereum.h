#pragma once
#include "BasicCoin.h"

/**
 * @brief Класс Ethereum, наследующий от BasicCoin.
 * 
 * Этот класс представляет криптовалюту Ethereum и наследует основные свойства и функциональность от BasicCoin.
 */
class Ethereum : public BasicCoin {

public:
  /**
   * @brief Конструктор класса Ethereum.
   * 
   * @param _id Уникальный идентификатор Ethereum.
   * @param _isImplemented Флаг, указывающий на то, реализован ли класс Ethereum.
   */
  Ethereum(unsigned _id, bool _isImplemented)
      : BasicCoin(_id, "ETH", _isImplemented) {
  }
};

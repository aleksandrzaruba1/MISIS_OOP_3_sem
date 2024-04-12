#pragma once
#include "BasicCoin.h"

/**
 * @brief Класс Bitcoin является производным от BasicCoin и представляет криптовалюту Bitcoin.
 */
class Bitcoin : public BasicCoin {
public:
    /**
     * @brief Конструктор класса Bitcoin.
     * 
     * @param _id Идентификатор Bitcoin.
     * @param _isImplemented Флаг, указывающий, реализован ли класс Bitcoin.
     */
    Bitcoin(unsigned _id, bool _isImplemented)
        : BasicCoin(_id, "BTC", _isImplemented) {
    }
};

#pragma once

#include "BasicExchange.h"

struct Parameters;

/**
 * @brief Класс, представляющий биржу Binance.
 */
class Binance : public BasicExchange {
private:

public:
  /**
   * @brief Структура, представляющая баланс аккаунта на бирже.
   */
  struct acct_balance {
    std::string asset; /**< Название актива. */
    double free; /**< Свободный баланс. */
    double locked; /**< Заблокированный баланс. */

    /**
     * @brief Конструктор для инициализации баланса аккаунта.
     * @param a Название актива.
     * @param f Свободный баланс.
     * @param l Заблокированный баланс.
     */
    acct_balance(std::string a, double f, double l)
        : asset(a), free(f), locked(l) {}
  };

  std::vector<acct_balance> balances; /**< Список балансов аккаунта. */

  /**
   * @brief Конструктор по умолчанию.
   */
  Binance();

  /**
   * @brief Метод получения баланса валюты.
   * @param currency Название валюты.
   * @return Баланс валюты, -1 если валюта недействительна.
   */
  double getBalance(std::string &currency) const override;

  /**
   * @brief Метод получения котировки.
   * @param params Параметры запроса.
   * @param symbol Символ.
   * @return Котировка.
   */
  quote_t getQuote(Parameters &params, std::string symbol) override;

  /**
   * @brief Метод получения доступного баланса аккаунта.
   * @param params Параметры запроса.
   */
  void getAvailBalance(Parameters &params) override;

  /**
   * @brief Метод для аутентифицированного запроса.
   * @param params Параметры запроса.
   * @param method Метод запроса.
   * @param request Запрос.
   * @param options Опции запроса.
   * @return Ответ на запрос в формате JSON.
   */
  json_t *authRequest(Parameters &params, std::string method,
                      std::string request, std::string options) override;

  /**
   * @brief Метод для получения подписи.
   * @param params Параметры запроса.
   * @param payload Нагрузка.
   * @return Подпись.
   */
  std::string getSignature(Parameters &params, std::string payload) override;
};

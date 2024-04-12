#pragma once

#include "BasicExchange.h"

struct Parameters;

/**
 * @brief Класс Kraken, представляющий биржу Kraken.
 */
class Kraken : public BasicExchange {
private:

public:
  /**
   * @brief Структура acct_balance для хранения баланса аккаунта на бирже.
   */
  struct acct_balance {
    std::string asset; ///< Название актива.
    double free;       ///< Свободный баланс.
    double locked;     ///< Заблокированный баланс.
    
    /**
     * @brief Конструктор для создания объекта acct_balance.
     * @param a Название актива.
     * @param f Свободный баланс.
     * @param l Заблокированный баланс.
     */
    acct_balance(std::string a, double f, double l)
        : asset(a), free(f), locked(l) {}
  };

  /**
   * @brief Вектор, содержащий балансы аккаунта.
   */
  std::vector<acct_balance> balances;

  /**
   * @brief Конструктор класса Kraken.
   */
  Kraken();

  /**
   * @brief Метод для получения баланса валюты.
   * @param currency Валюта.
   * @return Баланс валюты, -1 если валюта недействительна.
   */
  double getBalance(std::string &currency) const override;

  /**
   * @brief Метод для получения котировки.
   * @param params Параметры.
   * @param symbol Символ.
   * @return Котировка.
   */
  quote_t getQuote(Parameters &params, std::string symbol) override;

  /**
   * @brief Метод для получения доступного баланса.
   * @param params Параметры.
   */
  void getAvailBalance(Parameters &params) override;

  /**
   * @brief Метод для аутентифицированного запроса.
   * @param params Параметры.
   * @param method Метод запроса.
   * @param request Запрос.
   * @param options Опции запроса.
   * @return json_t* Указатель на JSON объект.
   */
  json_t *authRequest(Parameters &params, std::string method,
                      std::string request, std::string options) override;

  /**
   * @brief Метод для получения подписи.
   * @param params Параметры.
   * @param payload Полезная нагрузка.
   * @return std::string Подпись.
   */
  std::string getSignature(Parameters &params, std::string payload) override;
};

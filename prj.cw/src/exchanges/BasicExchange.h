/**
 * @file basic_exchange.h
 * @brief Описание базового класса биржи.
 */

#pragma once

#include <string>
#include <vector>

#include "quote_t.h"
#include "openssl/hmac.h"
#include "openssl/sha.h"
#include "utils/hex_str.h"

struct Parameters;

#include "utils/unique_json.h"

/**
 * @brief Базовый класс для работы с биржей.
 */
class BasicExchange {
private:
public:
  /**
   * @brief Название биржи.
   */
  std::string exchName;

  /**
   * @brief Флаг, указывающий, реализованы ли методы класса.
   */
  bool isImplemented;

  /**
   * @brief Получить название биржи.
   * @return Название биржи.
   */
  std::string getExchName() const { return exchName; }

  /**
   * @brief Получить информацию о реализации методов класса.
   * @return true, если методы реализованы, иначе false.
   */
  bool getIsImplemented() const { return isImplemented; }

  /**
   * @brief Получить баланс по заданной валюте.
   * @param currency Валюта, для которой необходимо получить баланс.
   * @return Баланс в указанной валюте.
   */
  virtual double getBalance(std::string &currency) const = 0;

  /**
   * @brief Получить котировку по заданным параметрам и символу.
   * @param params Параметры для запроса котировки.
   * @param symbol Символ, для которого запрашивается котировка.
   * @return Котировка для указанного символа.
   */
  virtual quote_t getQuote(Parameters &params, std::string symbol) = 0;

  /**
   * @brief Получить доступный баланс для указанных параметров.
   * @param params Параметры для запроса доступного баланса.
   */
  virtual void getAvailBalance(Parameters &params) = 0;

  /**
   * @brief Выполнить аутентифицированный запрос.
   * @param params Параметры для выполнения запроса.
   * @param param1 Первый параметр запроса.
   * @param param2 Второй параметр запроса.
   * @param param3 Третий параметр запроса.
   * @return JSON-объект с результатом запроса.
   */
  virtual json_t *authRequest(Parameters &params, std::string param1, std::string param2, std::string param3) = 0;

  /**
   * @brief Получить подпись для указанных параметров и данных.
   * @param params Параметры для создания подписи.
   * @param data Данные, для которых создается подпись.
   * @return Строка с подписью.
   */
  virtual std::string getSignature(Parameters &params, std::string data) = 0;
};

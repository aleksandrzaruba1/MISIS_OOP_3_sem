/**
 * @file kraken.h
 * @brief Содержит объявление класса Kraken, представляющего биржу Kraken.
 */

#include "exchanges/kraken.h"
#include "parameters.h"
#include "utils/restapi.h"

/**
 * @brief Конструктор по умолчанию класса Kraken.
 */
Kraken::Kraken() {
  exchName = "Kraken";
  isImplemented = true;
}

/**
 * @brief Получает баланс указанной валюты.
 * @param currency Валюта, баланс которой требуется получить.
 * @return Баланс указанной валюты.
 */
double Kraken::getBalance(std::string &currency) const {
  auto it = std::find_if(
      balances.cbegin(), balances.cend(),
      [&c = currency](const acct_balance &x) -> bool { return c == x.asset; });

  return it == std::end(balances) ? -1.0 : it->free;
}

/**
 * @brief Получает котировку указанного символа.
 * @param params Параметры для выполнения запроса.
 * @param symbol Символ, котировку которого требуется получить.
 * @return Пара bid и ask значений котировки.
 */
quote_t Kraken::getQuote(Parameters &params, std::string symbol) {
  RestApi api{"https://api.binance.us", params.cacert.c_str(), *params.logFile};
  std::string x{"/api/v3/ticker/bookTicker?symbol=BTCUSDT"};

  unique_json root{api.getRequest(x)};

  double quote =
      atof(json_string_value(json_object_get(root.get(), "bidPrice")));

  auto bidValue = quote ? quote : 0.0;

  quote = atof(json_string_value(json_object_get(root.get(), "askPrice")));

  auto askValue = quote ? quote : 0.0;

  return std::make_pair(bidValue, askValue);
}

/**
 * @brief Получает доступные балансы.
 * @param params Параметры для выполнения запроса.
 */
void Kraken::getAvailBalance(Parameters &params) {
  unique_json root{authRequest(params, "GET", "/api/v3/account", "")};

  // в настоящее время 170 различных активов
  size_t arraySize = json_array_size(json_object_get(root.get(), "balances"));

  // массив размером 170
  auto raw_balances = json_object_get(root.get(), "balances");

  // balances - член класса

  for (size_t i = 0; i < arraySize; ++i) {
    std::string asset = json_string_value(
        json_object_get(json_array_get(raw_balances, i), "asset"));
    double free = atof(json_string_value(
        json_object_get(json_array_get(raw_balances, i), "free")));
    double locked = atof(json_string_value(
        json_object_get(json_array_get(raw_balances, i), "locked")));
    balances.emplace_back(asset, free, locked);
  }

  return;
}
/**
 * @brief Выполняет аутентифицированный запрос к API.
 * @param params Параметры для выполнения запроса.
 * @param method Метод запроса (POST или GET).
 * @param request Запрос к API.
 * @param options Опции запроса.
 * @return Указатель на JSON-объект с ответом от API.
 */
json_t *Kraken::authRequest(Parameters &params, std::string method,
                             std::string request, std::string options) {
  RestApi api{"https://api.binance.us", params.cacert.c_str(), *params.logFile};
  std::string x{"/api/v1/time"};
  unique_json root{api.getRequest(x)};

  std::string timestamp = std::to_string(
      json_integer_value(json_object_get(root.get(), "serverTime")));

  std::string payload, uri, sig;

  std::array<std::string, 1> headers{"X-MBX-APIKEY:" + params.binanceApi};

  // сборка запроса
  if (method == "POST") {
    payload = options + "&timestamp=" + timestamp;
    sig = getSignature(params, payload);
    uri = request + "?" + options + "&timestamp=" + timestamp +
          "&signature=" + sig;
    return api.postRequest(uri,
                           make_slist(std::begin(headers), std::end(headers)));
  } else if (method == "GET") {
    payload = options + (options.empty() ? "" : "&") + "timestamp=" + timestamp;
    sig = getSignature(params, payload);
    uri = request + "?" + options + (options.empty() ? "" : "&") +
          "timestamp=" + timestamp + "&signature=" + sig;
    return api.getRequest(uri,
                          make_slist(std::begin(headers), std::end(headers)));
  } else {
    std::cerr << "Метод запроса должен быть либо POST, либо GET" << std::endl;
    exit(1);
  }
}

/**
 * @brief Генерирует подпись для аутентификации запроса к API.
 * @param params Параметры для выполнения запроса.
 * @param payload Данные запроса.
 * @return Строка с сгенерированной подписью.
 */
std::string Kraken::getSignature(Parameters &params, std::string payload) {
  uint8_t *hmac_digest = HMAC(EVP_sha256(), params.binanceSecret.c_str(),
                              params.binanceSecret.size(),
                              reinterpret_cast<const uint8_t *>(payload.data()),
                              payload.size(), NULL, NULL);
  std::string api_sign_header =
      hex_str(hmac_digest, hmac_digest + SHA256_DIGEST_LENGTH);
  return api_sign_header;
}

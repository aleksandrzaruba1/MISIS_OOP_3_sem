#pragma once

// #include "utils/unique_sqlite.h"

#include <fstream> ///< Для работы с файлами.
#include <string>  ///< Для работы с строками.
#include <vector>  ///< Для работы с векторами.
#include <curl/curl.h> ///< Для работы с HTTP.

/**
 * @brief Структура, хранящая все параметры, определенные в файле конфигурации.
 */
struct Parameters {
  
  std::vector<std::string> exchName;    ///< Имена бирж.
  std::vector<double> fees;             ///< Комиссии бирж.
  std::vector<bool> canShort;           ///< Возможность шортить.
  std::vector<bool> isImplemented;      ///< Флаги реализации.
  std::vector<std::string> tickerUrl;   ///< URL адреса для тикеров.

  CURL* curl;                           ///< Указатель на CURL обработчик.
  double spreadEntry;                   ///< Входной спред.
  double spreadTarget;                  ///< Целевой спред.
  unsigned maxLength;                   ///< Максимальная длина.
  double priceDeltaLim;                 ///< Лимит изменения цены.
  double trailingLim;                   ///< Значение trailing.
  unsigned trailingCount;               ///< Счетчик trailing.
  double orderBookFactor;               ///< Фактор стакана заявок.
  bool isDemoMode;                      ///< Режим демонстрации.
  std::string pair1;                    ///< Первая пара.
  std::string pair2;                    ///< Вторая пара.
  bool verbose;                         ///< Флаг вывода подробной информации.
  std::ofstream* logFile;               ///< Указатель на файл для логирования.
  unsigned interval;                    ///< Интервал.
  unsigned debugMaxIteration;           ///< Максимальное количество итераций в режиме отладки.
  bool useFullExposure;                 ///< Использование полного экспозиционного позиционирования.
  double testedExposure;                ///< Проверенное экспозиционное позиционирование.
  double maxExposure;                   ///< Максимальное экспозиционное позиционирование.
  bool useVolatility;                   ///< Использование волатильности.
  unsigned volatilityPeriod;            ///< Период волатильности.
  std::string cacert;                   ///< Путь к файлу сертификата.

  std::string okcoinApi;                ///< API ключ Okcoin.
  std::string okcoinSecret;             ///< Секретный ключ Okcoin.
  double okcoinFees;                    ///< Комиссии Okcoin.
  bool okcoinEnable;                    ///< Включение Okcoin.

  std::string bitstampClientId;         ///< Идентификатор клиента Bitstamp.
  std::string bitstampApi;              ///< API ключ Bitstamp.
  std::string bitstampSecret;           ///< Секретный ключ Bitstamp.
  double bitstampFees;                  ///< Комиссии Bitstamp.
  bool bitstampEnable;                  ///< Включение Bitstamp.

  std::string geminiApi;                ///< API ключ Gemini.
  std::string geminiSecret;             ///< Секретный ключ Gemini.
  double geminiFees;                    ///< Комиссии Gemini.
  bool geminiEnable;                    ///< Включение Gemini.

  std::string krakenApi;                ///< API ключ Kraken.
  std::string krakenSecret;             ///< Секретный ключ Kraken.
  double krakenFees;                    ///< Комиссии Kraken.
  bool krakenEnable;                    ///< Включение Kraken.

  std::string binanceApi;               ///< API ключ Binance.
  std::string binanceSecret;            ///< Секретный ключ Binance.
  double binanceFees;                   ///< Комиссии Binance.
  bool binanceEnable;                   ///< Включение Binance.

  bool sendEmail;                       ///< Отправка электронной почты.
  std::string senderAddress;            ///< Адрес отправителя.
  std::string senderUsername;           ///< Имя пользователя отправителя.
  std::string senderPassword;           ///< Пароль отправителя.
  std::string smtpServerAddress;        ///< Адрес SMTP сервера.
  std::string receiverAddress;          ///< Адрес получателя.

  std::string dbFile;                   ///< Файл базы данных.
  // unique_sqlite dbConn;

  /**
   * @brief Конструктор с параметром.
   * @param fileName Имя файла конфигурации.
   */
  Parameters(std::string fileName);

  /**
   * @brief Добавляет информацию о бирже в параметры.
   * @param n Имя биржи.
   * @param f Комиссия биржи.
   * @param h Возможность шортить.
   * @param m Флаг реализации.
   */
  void addExchange(std::string n, double f, bool h, bool m);

  /**
   * @brief Возвращает количество бирж.
   * @return Количество бирж.
   */
  int nbExch() const;
};

/**
 * @brief Получает значение параметра из файла конфигурации.
 * @param parameter Имя параметра.
 * @param configFile Файл конфигурации.
 * @return Значение параметра.
 */
std::string getParameter(std::string parameter, std::ifstream& configFile);

/**
 * @brief Преобразует строку в логическое значение.
 * @param value Строка для преобразования.
 * @return Логическое значение.
 */
bool getBool(std::string value);

/**
 * @brief Преобразует строку в числовое значение с плавающей точкой.
 * @param value Строка для преобразования.
 * @return Числовое значение с плавающей точкой.
 */
double getDouble(std::string value);

/**
 * @brief Преобразует строку в беззнаковое целое число.
 * @param value Строка для преобразования.
 * @return Беззнаковое целое число.
 */
unsigned getUnsigned(std::string value);

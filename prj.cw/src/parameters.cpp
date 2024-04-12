#include "parameters.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>

/**
 * @brief Находит конфигурационный файл по имени.
 * 
 * @param fileName Имя файла.
 * @return std::string Полный путь к найденному конфигурационному файлу или пустая строка, если файл не найден.
 */
static std::string findConfigFile(std::string fileName) {
  // Локальная директория
  {
    std::ifstream configFile(fileName);

    // Сохраняем первое совпадение
    if (configFile.good()) {
      return fileName;
    }
  }

  // Директория настроек Unix пользователя
  {
    char *home = getenv("HOME");

    if (home) {
      std::string prefix = std::string(home) + "/.config";
      std::string fullpath = prefix + "/" + fileName;
      std::ifstream configFile(fullpath);

      // Сохраняем первое совпадение
      if (configFile.good()) {
        return fullpath;
      }
    }
  }

  // Директория настроек пользователя Windows
  {
    char *appdata = getenv("APPDATA");

    if (appdata) {
      std::string prefix = std::string(appdata);
      std::string fullpath = prefix + "/" + fileName;
      std::ifstream configFile(fullpath);

      // Сохраняем первое совпадение
      if (configFile.good()) {
        return fullpath;
      }
    }
  }
}
// Директория настроек системы Unix
{
  std::string fullpath = "/etc/" + fileName;
  std::ifstream configFile(fullpath);

  // Сохраняем первое совпадение
  if (configFile.good()) {
    return fullpath;
  }
}

// Мы должны вернуть что-то, даже если знаем, что это не сработает
return fileName;
/**
 * @brief Конструктор класса Parameters.
 * 
 * @param fileName Имя файла конфигурации.
 */
Parameters::Parameters(std::string fileName) {
  std::ifstream configFile(findConfigFile(fileName));
  if (!configFile.is_open()) {
    std::cout << "ERROR: " << fileName << " не может быть открыт.\n";
    exit(EXIT_FAILURE);
  }

  // Инициализация параметров из файла конфигурации
  spreadEntry = getDouble(getParameter("SpreadEntry", configFile));
  spreadTarget = getDouble(getParameter("SpreadTarget", configFile));
  maxLength = getUnsigned(getParameter("MaxLength", configFile));
  priceDeltaLim = getDouble(getParameter("PriceDeltaLimit", configFile));
  trailingLim = getDouble(getParameter("TrailingSpreadLim", configFile));
  trailingCount = getUnsigned(getParameter("TrailingSpreadCount", configFile));
  orderBookFactor = getDouble(getParameter("OrderBookFactor", configFile));
  isDemoMode = getBool(getParameter("DemoMode", configFile));
  leg1 = getParameter("Leg1", configFile);
  leg2 = getParameter("Leg2", configFile);
  verbose = getBool(getParameter("Verbose", configFile));
  interval = getUnsigned(getParameter("Interval", configFile));
  debugMaxIteration =
      getUnsigned(getParameter("DebugMaxIteration", configFile));
  useFullExposure = getBool(getParameter("UseFullExposure", configFile));
  testedExposure = getDouble(getParameter("TestedExposure", configFile));
  maxExposure = getDouble(getParameter("MaxExposure", configFile));
  useVolatility = getBool(getParameter("UseVolatility", configFile));
  volatilityPeriod = getUnsigned(getParameter("VolatilityPeriod", configFile));
  cacert = getParameter("CACert", configFile);

  // Инициализация параметров для API различных бирж
  okcoinApi = getParameter("OkCoinApiKey", configFile);
  okcoinSecret = getParameter("OkCoinSecretKey", configFile);
  okcoinFees = getDouble(getParameter("OkCoinFees", configFile));
  okcoinEnable = getBool(getParameter("OkCoinEnable", configFile));

  bitstampClientId = getParameter("BitstampClientId", configFile);
  bitstampApi = getParameter("BitstampApiKey", configFile);
  bitstampSecret = getParameter("BitstampSecretKey", configFile);
  bitstampFees = getDouble(getParameter("BitstampFees", configFile));
  bitstampEnable = getBool(getParameter("BitstampEnable", configFile));

  geminiApi = getParameter("GeminiApiKey", configFile);
  geminiSecret = getParameter("GeminiSecretKey", configFile);
  geminiFees = getDouble(getParameter("GeminiFees", configFile));
  geminiEnable = getBool(getParameter("GeminiEnable", configFile));

  krakenApi = getParameter("KrakenApiKey", configFile);
  krakenSecret = getParameter("KrakenSecretKey", configFile);
  krakenFees = getDouble(getParameter("KrakenFees", configFile));
  krakenEnable = getBool(getParameter("KrakenEnable", configFile));

  binanceApi = getParameter("BinanceApiKey", configFile);
  binanceSecret = getParameter("BinanceSecretKey", configFile);
  binanceFees = getDouble(getParameter("BinanceFees", configFile));
  binanceEnable = getBool(getParameter("BinanceEnable", configFile));
}
// sendEmail = getBool(getParameter("SendEmail", configFile));
// senderAddress = getParameter("SenderAddress", configFile);
// senderUsername = getParameter("SenderUsername", configFile);
// senderPassword = getParameter("SenderPassword", configFile);
// smtpServerAddress = getParameter("SmtpServerAddress", configFile);
// receiverAddress = getParameter("ReceiverAddress", configFile);

// Получение имени файла базы данных
dbFile = getParameter("DBFile", configFile);
/**
 * @brief Добавляет информацию о бирже.
 * 
 * @param n Название биржи.
 * @param f Сборы сделки.
 * @param h Возможность продажи на короткую позицию.
 * @param m Реализована ли биржа.
 */
void Parameters::addExchange(std::string n, double f, bool h, bool m) {
  exchName.push_back(n);
  fees.push_back(f);
  canShort.push_back(h);
  isImplemented.push_back(m);
}

/**
 * @brief Возвращает количество бирж.
 * 
 * @return int Количество бирж.
 */
int Parameters::nbExch() const { return exchName.size(); }

/**
 * @brief Получает значение параметра из файла конфигурации.
 * 
 * @param parameter Имя параметра.
 * @param configFile Файл конфигурации.
 * @return std::string Значение параметра.
 */
std::string getParameter(std::string parameter, std::ifstream &configFile) {
  assert(configFile);
  std::string line;
  configFile.clear();
  configFile.seekg(0);

  while (getline(configFile, line)) {
    if (line.length() > 0 && line.at(0) != '#') {
      std::string key = line.substr(0, line.find('='));
      std::string value = line.substr(line.find('=') + 1, line.length());
      if (key == parameter) {
        return value;
      }
    }
  }
  std::cout << "ERROR: parameter '" << parameter
            << "' not found. Ваш файл конфигурации может быть слишком старым.\n"
            << std::endl;
  exit(EXIT_FAILURE);
}

/**
 * @brief Преобразует строку в булево значение.
 * 
 * @param value Строка со значением.
 * @return bool Булево значение.
 */
bool getBool(std::string value) { return value == "true"; }

/**
 * @brief Преобразует строку в значение типа double.
 * 
 * @param value Строка со значением.
 * @return double Значение типа double.
 */
double getDouble(std::string value) { return atof(value.c_str()); }

/**
 * @brief Преобразует строку в беззнаковое целое число.
 * 
 * @param value Строка со значением.
 * @return unsigned Беззнаковое целое число.
 */
unsigned getUnsigned(std::string value) { return atoi(value.c_str()); }

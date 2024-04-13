/** 
 * @brief Основной файл программы CryptoArb, который содержит реализацию
 *        программы для арбитража криптовалют.
 */
#include "parameters.h"
#include "utils/db_fun.h"
#include "utils/time_fun.h"

//#include "coins/Bitcoin.h"
//#include "coins/Ethereum.h"

//#include "exchanges/binance.h"
//#include "exchanges/kraken.h"

//#include <gtest/gtest.h>

#include <iostream>
#include <vector>

/**
 * @brief Структура, представляющая баланс.
 */
struct Balance {
  double leg1;        /**< Баланс первой валюты */
  double leg2;        /**< Баланс второй валюты */
  double leg1After;   /**< Баланс первой валюты после операций */
  double leg2After;   /**< Баланс второй валюты после операций */
};
/**
 * @brief Главная функция программы.
 * @param argc Количество аргументов командной строки.
 * @param argv Массив аргументов командной строки.
 * @return Код возврата программы.
 */
int main(int argc, char **argv) {
  std::cout << " >>> CryptoArb Cryptocurrencies Arbitrage Bot <<<" << std::endl;


  // Чтение файла конфигурации
  Parameters params("CryptoArb.conf");

  //Создание имени лог-файла с текущей датой и временем
  std::string currDateTime = "today";
  std::string logFileName = "output/log/CryptoArb_log_" + currDateTime + ".log";
  std::cout << " >>> Log file generated at ./output/log/: " << logFileName << " <<<" << std::endl;

  // Создание файла для логирования событий
  std::ofstream logFile(logFileName, std::ofstream::trunc);
  // logFile << std::setprecision(3) << std::fixed;
  params.logFile = &logFile;

  logFile << "--------------------------\n";
  logFile << "|   CryptoArb Log File   |\n";
  logFile << "--------------------------\n";
  // logFile << "CryptoArb started time: " << printDateTime() << "\n\n";
  logFile << "CryptoArb started time: NULL" << "\n\n";

  logFile << "[ Information ]\n\t\t";

  // Проверка параметров
  if (params.leg1 == "BTC" and params.leg2 == "USD") {
    logFile << "trading pair: [ BTC, USD ]\n\t";
  } else {
    logFile << "ERROR: only support BTC / USD pair\n\t";
    exit(EXIT_FAILURE);
  }
/*
  // dababase connections
  int db_status = createDbConnection(params);
  if (db_status != 0) {
    std::cerr << "ERROR: db connection error" << std::endl;
    exit(EXIT_FAILURE);
  }

  // initialize exchange objects
  std::vector<BasicExchange *> ExchangeVec{};

  if (params.binanceEnable || params.isDemoMode) {
    ExchangeVec.push_back(new Binance());
    createTable("binance", params);
  }
  if (params.krakenEnable || params.isDemoMode) {
    ExchangeVec.push_back(new Kraken());
    createTable("kraken", params);
  }
  // TODO: add more exchagnes

  
  std::vector<BasicCoin *> CoinVec{
      new Bitcoin(0, true),
      new Ethereum(1, true),
  };
  // TODO: add more coin

  // create CSV files collecting trade results
  std::string currDateTime = printDateTimeFileName();
  std::string csvFileName =
      "output/result/CryptoArb_result_" + currDateTime + ".csv";
  std::ofstream csvFile(csvFileName, std::ofstream::trunc);
  csvFile << "TRADE_ID,"
          << "EXCHANGE_LONG,"
          << "EXHANGE_SHORT,"
          << "ENTRY_TIME,"
          << "EXIT_TIME,"
          << "DURATION,"
          << "TOTAL_EXPOSURE,"
          << "BALANCE_BEFORE,"
          << "BALANCE_AFTER,"
          << "RETURN" 
          << std::endl;

*/
  if (params.isDemoMode)
    logFile << "Demo mode: trades won't be generated\n\n";

  // logFile << "Pair traded: " << params.leg1 << "/" << params.leg2 << "\n\n";

  // *********************
  // * Log profit target *
  // *********************
  // Логирование целей прибыли (вход в спред и целевой показатель спреда должны быть равны +, иначе вы потеряете $)
  logFile << "[ Targets ]\n";
  logFile << "\tSpread Entry : " << params.spreadEntry * 100.0 << "%\n";
  logFile << "\tSpread Target: " << params.spreadTarget * 100.0 << "%\n\n";

  if (params.spreadEntry <= 0.0)
    logFile << "\t\t WARNING: Spread Entry should be positive.\n";

  if (params.spreadTarget <= 0.0)
    logFile << "\t\t WARNING: Spread Target should be positive.\n";

  logFile << std::endl;

  // *****************************
  // * Get balances of coins in  *
  // * exchanges and log results *
  // *****************************
// Логирование текущего баланса валют
  logFile << "[ Current Balance ]\n";
/*
  for (auto &exch : ExchangeVec) {

    // get balance in the exchange
    // store it in class member: balances
    exch->getAvailBalance(params);
    logFile << "\t" << exch->getExchName() << "\n";

    for (auto &coin : CoinVec) {
      std::string coinName = coin->getCoinName();
      auto bal = exch->getBalance(coinName);

      // TODO: implement demo mode
      if (params.isDemoMode and false) {
        logFile << "\t\t"
                << "N/A - demo mode\tTODO: Implement demo mode\n";
      } else if (exch->getIsImplemented()) {
        logFile << "\t\t" << coinName << ": " << bal << "\n";
      } else {
        logFile << "\t\t"
                << "not implemented";
      }
    }
  }*/
  logFile << std::endl;

  // ****************
  // * Log exposure *
  // ****************

  logFile << "[ Exposure ]\n";
  logFile << std::endl;

  // **********************
  // * main analysis loop *
  // **********************

  // Инициализация соединений cURL
  params.curl = curl_easy_init();

  // Информация о времени
  
  /**
   * @brief Получает текущее календарное время.
   * @return Возвращает текущее календарное время в формате `time_t`.
   */
  time_t rawtime = time(nullptr);
  /**
   * @brief Преобразует время из `time_t` в структуру `tm`, представляющую календарные данные.
   * @param rawtime Значение времени в формате `time_t` для преобразования.
   * @return Возвращает структуру `tm`, содержащую календарные данные.
   */
  tm timeinfo = *localtime(&rawtime);
  time_t currTime;
  int i = 3;
  bool running = true;
  while (running and i--) {

    /* currTime = std::mktime(&timeinfo);

    for (auto exchange : ExchangeVec) {
      std::string exchName = exchange->getExchName();
      auto quote = exchange->getQuote(params, "btc");
      double bid = quote.bid(), ask = quote.ask();

      // Saves bid/ask data into SQLite database
      addBidAskToDb(exchName, printDateTimeDb(currTime), bid, ask, params);

      if (params.verbose)
        logFile << "\t" << exchName << " (bid/ask): " << bid << " / " << ask
                << std::endl;

      curl_easy_reset(params.curl);
    }*/
  }
/*
  csvFile.close();
  curl_easy_cleanup(params.curl); */

  std::cout << " >>> CryptoArb has been correctly terminated. <<<" << std::endl;

  return 0;
}

#include "parameters.h"
#include "utils/unique_sqlite.h"

#include <iostream>
#include <string>
#include <format>

/**
 * @brief Пространство имён, определяющее вспомогательные перегрузки для упрощения управления ресурсами SQLite.
 */
namespace {
    /**
     * @brief Класс-прокси для управления ресурсом SQLite.
     * 
     * Этот класс помогает в управлении ресурсом SQLite через указатель, обернутый в std::unique_ptr.
     */
    template <typename UNIQUE_T>
    class sqlite_proxy {
        typename UNIQUE_T::pointer S; ///< Указатель на ресурс SQLite.
        UNIQUE_T &owner; ///< Владелец ресурса SQLite.

    public:
        /**
         * @brief Конструктор класса sqlite_proxy.
         * 
         * @param owner Владелец ресурса SQLite.
         */
        sqlite_proxy(UNIQUE_T &owner) : S(nullptr), owner(owner) {}

        /**
         * @brief Деструктор класса sqlite_proxy.
         * 
         * Освобождает ресурс SQLite при уничтожении объекта.
         */
        ~sqlite_proxy() { owner.reset(S); }

        /**
         * @brief Оператор приведения типа к указателю на указатель.
         * 
         * @return Указатель на указатель ресурса SQLite.
         */
        operator typename UNIQUE_T::pointer* () { return &S; }
    };

    /**
     * @brief Функция для получения объекта класса sqlite_proxy.
     * 
     * Эта функция используется для управления ресурсом SQLite через std::unique_ptr.
     */
    template <typename T, typename deleter>
    sqlite_proxy< std::unique_ptr<T, deleter> >
    acquire(std::unique_ptr<T, deleter> &owner) { return owner; }
}

/**
 * @brief Функция для создания соединения с базой данных.
 * 
 * @param params Параметры подключения к базе данных.
 * @return Код результата операции.
 */
int createDbConnection(Parameters& params) {
    int res = sqlite3_open(params.dbFile.c_str(), acquire(params.dbConn));

    if (res != SQLITE_OK)
        std::cerr << sqlite3_errmsg(params.dbConn.get()) << std::endl;

    return res;
}

/**
 * @brief Функция для создания таблицы в базе данных.
 * 
 * @param exchangeName Название биржи.
 * @param params Параметры подключения к базе данных.
 * @return Код результата операции.
 */
int createTable(std::string exchangeName, Parameters& params) {
    std::string query = "CREATE TABLE IF NOT EXISTS `" + exchangeName + "` (Datetime DATETIME NOT NULL, bid DECIMAL(8, 2), ask DECIMAL(8, 2));";

    unique_sqlerr errmsg;
    int res = sqlite3_exec(params.dbConn.get(), query.c_str(), nullptr, nullptr, acquire(errmsg));
    if (res != SQLITE_OK)
        std::cerr << errmsg.get() << std::endl;

    return res;
}

/**
 * @brief Функция для добавления записи bid/ask в базу данных.
 * 
 * @param exchangeName Название биржи.
 * @param datetime Дата и время.
 * @param bid Значение bid.
 * @param ask Значение ask.
 * @param params Параметры подключения к базе данных.
 * @return Код результата операции.
 */
int addBidAskToDb(std::string exchangeName, std::string datetime, double bid, double ask, Parameters& params) {
    std::string query = "INSERT INTO `" + exchangeName + "` VALUES ('"   + datetime + "'," + std::to_string(bid) + "," + std::to_string(ask) + ");";

    unique_sqlerr errmsg;
    int res = sqlite3_exec(params.dbConn.get(), query.c_str(), nullptr, nullptr, acquire(errmsg));
    if (res != SQLITE_OK)
        std::cerr << errmsg.get() << std::endl;

    return res;
}

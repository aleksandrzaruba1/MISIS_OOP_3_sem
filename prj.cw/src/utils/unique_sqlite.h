#pragma once

#include "sqlite3.h"
#include <memory>
/**
 * @brief Функтор для удаления ресурсов SQLite3.
 */
struct sqlite_deleter {
 /**
   * @brief Оператор удаления для объектов типа sqlite3.
   * @param[in] S Указатель на объект sqlite3 для закрытия.
   */
  void operator () (sqlite3 *S) {
    sqlite3_close(S);
  }
  void operator () (char *errmsg) {
    sqlite3_free(errmsg);
  }
};

using unique_sqlite = std::unique_ptr<sqlite3, sqlite_deleter>;
using unique_sqlerr = std::unique_ptr<char, sqlite_deleter>;


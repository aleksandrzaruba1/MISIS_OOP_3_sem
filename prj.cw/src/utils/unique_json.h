#pragma once

#include "jansson.h"
#include <memory>

/**
 * @brief Функтор для удаления объекта json_t.
 */
struct json_deleter {
  void operator () (json_t *J) { /**< Оператор для удаления объекта json_t. */
    json_decref(J); /**< Уменьшает счетчик ссылок на объект json_t. */
  }
};

/**
 * @brief Уникальный указатель на объект json_t с пользовательским удалением.
 */
using unique_json = std::unique_ptr<json_t, json_deleter>;

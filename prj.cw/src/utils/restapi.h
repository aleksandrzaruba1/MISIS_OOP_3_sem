#include "restapi.h"
#include <cassert>
#include <thread>

// Анонимное пространство имен для внутренних вспомогательных функций
namespace {

/**
 * @brief Callback-функция для приема данных через libcurl.
 * 
 * @param contents Указатель на полученные данные.
 * @param size Размер каждого элемента данных.
 * @param nmemb Количество элементов данных.
 * @param userp Указатель на пользовательские данные.
 * @return Возвращает общий размер полученных данных.
 */
size_t recvCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  auto &buffer = *static_cast<std::string *>(userp);
  auto n = size * nmemb;
  return buffer.append((char *)contents, n), n;
}

/**
 * @brief Выполняет запрос к API через libcurl.
 * 
 * @param C Указатель на объект libcurl.
 * @param url URL-адрес для запроса.
 * @param headers Заголовки HTTP запроса.
 * @param log Поток для вывода журнала событий.
 * @return Возвращает указатель на объект JSON или NULL в случае ошибки.
 */
json_t *doRequest(CURL *C, const std::string &url, const curl_slist *headers,
                  std::ostream &log) {
  std::string recvBuffer;

  curl_easy_setopt(C, CURLOPT_WRITEDATA, recvBuffer.c_str());
  curl_easy_setopt(C, CURLOPT_URL, url.c_str());
  curl_easy_setopt(C, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(C, CURLOPT_DNS_CACHE_TIMEOUT, 3600);

  int attempt = 0;
  while (true) {

    // Повторная попытка, если была предыдущая ошибка
    if (attempt != 0) {
      log << "  API ОШИБКА: Повтор через 2 сек..." << std::endl;
      std::cout << "  API ОШИБКА: Повтор через 2 сек, см. журнал" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(2));
      recvBuffer.clear();
      curl_easy_setopt(C, CURLOPT_DNS_CACHE_TIMEOUT, 0);
    }

    // Выполнение запроса через libcurl
    CURLcode resCurl = curl_easy_perform(C);
    if (resCurl != CURLE_OK) {
      log << "Ошибка с cURL: ";
      log << curl_easy_strerror(resCurl) << '\n';
      log << "  URL: " << url << '\n';
      attempt++;
      continue;
    }

    // Декодирует строку JSON и возвращает объект или массив, либо NULL в случае ошибки
    json_error_t error;

    json_t *root = json_loads(recvBuffer.c_str(), 0, &error);
    if (!root) {
      long resp_code;
      curl_easy_getinfo(C, CURLINFO_RESPONSE_CODE, &resp_code);
      log << "Ответ сервера: " << resp_code << " - " << url << '\n'
          << "Ошибка с JSON: " << error.text << '\n'
          << "Буфер:\n"
          << recvBuffer << '\n';
      attempt++;
      continue;
    }

    return root;
  }
}

} // Конец анонимного пространства имен

/**
 * @brief Конструктор класса RestApi.
 * 
 * @param host Хост API.
 * @param cacert Путь к файлу сертификата.
 * @param log Поток для вывода журнала событий.
 */
RestApi::RestApi(std::string host, const char *cacert, std::ostream &log)
    : C(curl_easy_init()), host(std::move(host)), log(log) {

  assert(C);

  // Настройка сертификата
  if (cacert)
    curl_easy_setopt(C.get(), CURLOPT_CAINFO, cacert);
  else
    curl_easy_setopt(C.get(), CURLOPT_SSL_VERIFYPEER, false);

  curl_easy_setopt(C.get(), CURLOPT_CONNECTTIMEOUT, 10L);
  curl_easy_setopt(C.get(), CURLOPT_TIMEOUT, 20L);
  curl_easy_setopt(C.get(), CURLOPT_USERAGENT, "CryptoArb");
  curl_easy_setopt(C.get(), CURLOPT_ACCEPT_ENCODING, "gzip");

  curl_easy_setopt(C.get(), CURLOPT_WRITEFUNCTION, recvCallback);
}

/**
 * @brief Деструктор для объекта CURL.
 * 
 * @param c Указатель на объект CURL.
 */
void RestApi::CURL_deleter::operator()(CURL *c) {
  if (c)
    curl_easy_cleanup(c);
}

/**
 * @brief Деструктор для списка заголовков.
 * 
 * @param slist Указатель на список заголовков.
 */
void RestApi::CURL_deleter::operator()(curl_slist *slist) {
  if (slist)
    curl_slist_free_all(slist);
}

/**
 * @brief Выполняет HTTP GET запрос к API.
 * 
 * @param uri URI ресурса.
 * @param headers Список заголовков HTTP запроса.
 * @return Возвращает указатель на объект JSON или NULL в случае ошибки.
 */
json_t *RestApi::getRequest(const std::string &uri, unique_slist headers) {
  curl_easy_setopt(C.get(), CURLOPT_HTTPGET, true);
  return doRequest(C.get(), host + uri, headers.get(), log);
}

/**
 * @brief Выполняет HTTP POST запрос к API с данными.
 * 
 * @param uri URI ресурса.
 * @param headers Список заголовков HTTP запроса.
 * @param post_data Данные для POST запроса.
 * @return Возвращает указатель на объект JSON или NULL в случае ошибки.
 */
json_t *RestApi::postRequest(const std::string &uri,
                             unique_slist headers,
                             const std::string &post_data) {
  curl_easy_setopt(C.get(), CURLOPT_POSTFIELDS, post_data.data());
  curl_easy_setopt(C.get(), CURLOPT_POSTFIELDSIZE, post_data.size());
  return doRequest(C.get(), host + uri, headers.get(), log);
}

/**
 * @brief Выполняет HTTP POST запрос к API без заголовков.
 * 
 * @param uri URI ресурса.
 * @param post_data Данные для POST запроса.
 * @return Возвращает указатель на объект JSON или NULL в случае ошибки.
 */
json_t *RestApi::postRequest(const std::string &uri,
                             const std::string &post_data) {

  return postRequest(uri, nullptr, post_data);
}

#ifndef STOCK_FETCHER_H
#define STOCK_FETCHER_H

#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

double fetch_stock_price(const std::string& symbol, const std::string& api_key) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol + "&apikey=" + api_key;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    try {
        auto json_data = json::parse(readBuffer);
        std::string price_str = json_data["Global Quote"]["05. price"];
        return std::stod(price_str);
    } catch (...) {
        std::cerr << "Error parsing JSON or invalid symbol/API response.\n";
        return -1.0;
    }
}

#endif

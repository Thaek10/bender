#include "spdlog/spdlog.h"
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include "bitmex.h"

namespace bitmex {
BitMexTap::BitMexTap(bool testnet) : Broker(testnet) {
  instance_name_ = testnet_ ? "BitMex Test" : "BitMex Prod";
  host_ = testnet_ ? TESTNET_HOST : PROD_HOST;
  endpoint_ = API_ENDPOINT;

  apikey_secret_ = getenv("BITMEX_APIKEY_SECRET");
  apikey_ = getenv("BITMEX_APIKEY");
}

std::string BitMexTap::Sign(const std::string& data) {
  unsigned char result[2048];
  unsigned int resultlen;
  HMAC(EVP_sha256(), apikey_secret_, sizeof apikey_secret_, reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), result,
       &resultlen);
  return std::string((const char*)result, resultlen);
}

void BitMexTap::SendPrologue(websocket::stream<beast::ssl_stream<tcp::socket>>& ws) {}

void BitMexTap::Parse(const std::string& recbuf) {
  json::Document doc;
  // spdlog::info(recbuf);
  doc.Parse(recbuf.c_str());

  if (!doc.HasMember("data")) {
    spdlog::info(recbuf);
    return;
  }

  if (doc["table"] == "orderBookL2_25") {
    // FIXME
    // spdlog::info(recbuf);
  } else if (doc["table"] == "trade") {
    // FIXME
    ParseTrade(doc);
  } else {
    spdlog::info("BitMex unknown table: " + recbuf);
  }
}

void BitMexTap::ParseTrade(const json::Document& doc) {
  const json::Value& data = doc["data"];
  assert(data.IsArray());

  for (json::SizeType i = 0; i < data.Size(); i++) {
    const rapidjson::Value& c = data[i];
    Trade t;
    t.timestamp = c["timestamp"].GetString();
    t.symbol = c["symbol"].GetString();
    t.side = c["side"].GetString();
    t.size = c["size"].GetInt();
    t.price = c["price"].GetDouble();
    t.tickDirection = c["tickDirection"].GetString();
    t.trdMatchID = c["trdMatchID"].GetString();
    t.grossValue = c["grossValue"].GetInt();
    t.homeNotional = c["homeNotional"].GetDouble();
    t.foreignNotional = c["foreignNotional"].GetInt();
    // spdlog::info(t.String());
    trade_signals(t);
  }
}

} // namespace bitmex
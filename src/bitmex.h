#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <string>

#include "broker.h"
#include "types/bitmex/trade.h"

namespace bitmex {
class BitMexTap : public Broker {
public:
  BitMexTap(bool testnet);

  std::string Sign(const std::string& data);

private:
  void SendPrologue(websocket::stream<beast::ssl_stream<tcp::socket>>& ws);
  void Parse(const std::string& recbuf);

  beast::http::request<beast::http::string_body> BuildRequest(std::string& url, int side, int size, double price);
  bool ParseResponse(int side, int size, float price, std::string& body);

  void ParseTrade(const json::Document& doc);

  inline static char TESTNET_HOST[] = "www.bitmex.com";
  inline static char PROD_HOST[] = "www.bitmex.com";
  inline static char API_ENDPOINT[] = "/realtime?subscribe=orderBookL2_25:XBTUSD,trade:XBTUSD";
  // inline static char API_ENDPOINT[] = "/realtime?subscribe=instrument:XBTUSD,trade:XBTUSD";

  char* apikey_secret_;
  char* apikey_;

  inline static char name[] = "BitMex";

  bool testnet_;
};
}
#pragma once

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

namespace json = rapidjson;

enum Instance
{
  PROD,
  TESTNET
};

class Broker
{

public:
  Broker(bool testnet);
  ~Broker();

  bool EstablishConnection();
  void Run(void);

protected:
  std::string ReadChunk(websocket::stream<beast::ssl_stream<tcp::socket>> &ws);
  virtual void SendPrologue(websocket::stream<beast::ssl_stream<tcp::socket>> &ws) = 0;
  virtual void Parse(const std::string &recbuf) = 0;

  char *host_;
  char *endpoint_;
  std::string instance_name_;
  bool testnet_;

  // Used by the websocket
  beast::flat_buffer buffer_;
  // REST API
  net::io_context ioc_;
  ssl::context ctx_;
  beast::ssl_stream<tcp::socket> web_;
};

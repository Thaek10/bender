#pragma once

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
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

enum Instance { PROD, TESTNET };

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
  explicit WebSocketSession(const char* host, const char* port, const char* path)
      : resolver_(net::make_strand(ioc_))
      , ctx_{ssl::context::tlsv12_client}
      , ws_(net::make_strand(ioc_), ctx_)
      , host_(host)
      , port_(port)
      , path_(path) {}

  void Run();
  void OnResolve(beast::error_code ec, tcp::resolver::results_type results);
  void OnConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);
  void OnSslHandshake(beast::error_code ec);
  void OnHandshake(beast::error_code ec);
  void OnWrite(beast::error_code ec, std::size_t bytes);
  void OnRead(beast::error_code ec, std::size_t bytes);
  virtual void Parse(const std::string& recbuf) = 0;

private:
  net::io_context ioc_;
  beast::flat_buffer buffer_;
  ssl::context ctx_;
  websocket::stream<beast::ssl_stream<beast::tcp_stream>> ws_;
  tcp::resolver resolver_;

  const char* host_;
  const char* port_;
  const char* path_;
};

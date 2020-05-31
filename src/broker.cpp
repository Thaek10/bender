#include "spdlog/spdlog.h"

#include "broker.h"

Broker::Broker(bool testnet) : ctx_(ssl::context::tlsv12_client), web_(ioc_, ctx_), testnet_(testnet) {}

Broker::~Broker() {}

bool Broker::EstablishConnection() {
  try {
    tcp::resolver resolver{ioc_};

    auto const results = resolver.resolve(host_, "https");

    net::connect(web_.next_layer(), results.begin(), results.end());

    boost::asio::ip::tcp::no_delay option(true);
    web_.next_layer().set_option(option);

    web_.handshake(ssl::stream_base::client);
  } catch (std::exception& e) {
    spdlog::error(e.what());
    return false;
  }
  return true;
}

std::string Broker::ReadChunk(websocket::stream<beast::ssl_stream<tcp::socket>>& ws) {
  buffer_.consume(buffer_.size());
  ws.read(buffer_);
  return std::string(boost::asio::buffer_cast<char const*>(beast::buffers_front(buffer_.data())), boost::asio::buffer_size(buffer_.data()));
}

void Broker::Run(void) {
  while (true) {
    net::io_context ioc;
    boost::asio::ip::tcp::endpoint ep;
    ssl::context ctx{ssl::context::tlsv12_client};
    tcp::resolver resolver{ioc};

    assert(host_);

    auto const results = resolver.resolve(host_, "https");
    websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};
    net::connect(ws.next_layer().next_layer(), results.begin(), results.end());
    boost::asio::ip::tcp::no_delay option_nodelay(true);
    ws.next_layer().next_layer().set_option(option_nodelay);
    boost::asio::socket_base::keep_alive option_keepalive(true);
    ws.next_layer().next_layer().set_option(option_keepalive);
    ws.next_layer().handshake(ssl::stream_base::client);

    ws.handshake(host_, endpoint_);

    SendPrologue(ws);
    try {
      int flags = 0;
      while (true) {
        std::string c = ReadChunk(ws);
        Parse(c);
      }
    } catch (std::exception& e) {
      spdlog::error(e.what());
    }
  }
}

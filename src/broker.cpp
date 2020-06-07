#include "spdlog/spdlog.h"

#include "broker.h"
#include <chrono>

void WebSocketSession::Run() {
  resolver_.async_resolve(
      host_, port_, beast::bind_front_handler(&WebSocketSession::OnResolve, shared_from_this()));
  ioc_.run();
}

void WebSocketSession::OnResolve(beast::error_code ec, tcp::resolver::results_type results) {
  spdlog::info("OnResolve");
  if (ec) {
    spdlog::error("resolve: " + ec.message());
  }

  beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));
  beast::get_lowest_layer(ws_).async_connect(
      results, beast::bind_front_handler(&WebSocketSession::OnConnect, shared_from_this()));
}

void WebSocketSession::OnConnect(beast::error_code ec,
                                 tcp::resolver::results_type::endpoint_type ep) {
  if (ec) {
    spdlog::error("connect: " + ec.message());
  }
  beast::get_lowest_layer(ws_).expires_never();

  std::string host = std::string(host_) + ":" + std::to_string(ep.port());
  beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));
  ws_.next_layer().async_handshake(
      ssl::stream_base::client,
      beast::bind_front_handler(&WebSocketSession::OnSslHandshake, shared_from_this()));
}

void WebSocketSession::OnSslHandshake(beast::error_code ec) {
  if (ec) {
    spdlog::error("sslhandshake: " + ec.message());
  }
  beast::get_lowest_layer(ws_).expires_never();
  ws_.async_handshake(
      host_, path_, beast::bind_front_handler(&WebSocketSession::OnHandshake, shared_from_this()));
}
void WebSocketSession::OnHandshake(beast::error_code ec) {
  if (ec) {
    spdlog::error("handshake: " + ec.message());
  }

  ws_.async_write(net::buffer("foo"),
                  beast::bind_front_handler(&WebSocketSession::OnWrite, shared_from_this()));
}

void WebSocketSession::OnWrite(beast::error_code ec, std::size_t bytes) {
  if (ec) {
    spdlog::error("write: " + ec.message());
  }
  ws_.async_read(buffer_, beast::bind_front_handler(&WebSocketSession::OnRead, shared_from_this()));
}

void WebSocketSession::OnRead(beast::error_code ec, std::size_t bytes) {
  if (ec) {
    spdlog::error("read: " + ec.message());
  }
  spdlog::info(boost::beast::buffers_to_string(buffer_.data()));
}

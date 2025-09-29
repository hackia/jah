#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
  beast::tcp_stream stream_;
  beast::flat_buffer buffer_;
  http::request<http::string_body> request_;

public:
  explicit HttpSession(tcp::socket &&socket) : stream_(std::move(socket)) {}

  void start() {
    net::dispatch(stream_.get_executor(),
                  beast::bind_front_handler(&HttpSession::readRequest,
                                            shared_from_this()));
  }

private:
  void readRequest() {
    request_ = {};
    http::async_read(stream_, buffer_, request_,
                     beast::bind_front_handler(&HttpSession::handleRead,
                                               shared_from_this()));
  }

  void handleRead(const beast::error_code &ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec == http::error::end_of_stream) {
      return closeConnection();
    }

    if (ec) {
      std::cerr << "Read error: " << ec.message() << "\n";
      return;
    }

    processRequest();
  }

  void processRequest() {
    auto response = buildResponse();
    writeResponse(std::move(response));
  }

  http::response<http::string_body> buildResponse() const {
    http::response<http::string_body> response;
    response.version(request_.version());
    response.keep_alive(request_.keep_alive());
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);

    if (request_.target() == "/") {
      response.set(http::field::content_type, "text/plain");
      response.body() = "Hello, Jah!";
    } else if (request_.target() == "/api/version") {
      response.body() = R"({"version": "0.1.0"})";
    } else {
      response.result(http::status::not_found);
      response.body() = R"({"error": "Not Found"})";
    }
    response.prepare_payload();
    return response;
  }

  void writeResponse(http::response<http::string_body> &&response) {
    http::async_write(stream_, response,
                      beast::bind_front_handler(&HttpSession::handleWrite,
                                                shared_from_this()));
  }

  void handleWrite(const beast::error_code &ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) {
      std::cerr << "Write error: " << ec.message() << "\n";
    }

    closeConnection();
  }

  void closeConnection() {
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
  }
};

class HttpListener : public std::enable_shared_from_this<HttpListener> {
  net::io_context &ioContext_;
  tcp::acceptor acceptor_;

public:
  HttpListener(net::io_context &ioc, const tcp::endpoint &endpoint)
      : ioContext_(ioc), acceptor_(ioc) {
    beast::error_code ec;

    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
      std::cerr << "Open error: " << ec.message() << "\n";
      return;
    }

    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
      std::cerr << "Set option error: " << ec.message() << "\n";
      return;
    }

    acceptor_.bind(endpoint, ec);
    if (ec) {
      std::cerr << "Bind error: " << ec.message() << "\n";
      return;
    }

    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
      std::cerr << "Listen error: " << ec.message() << "\n";
      return;
    }
  }

  void start() { acceptConnection(); }

private:
  void acceptConnection() {
    acceptor_.async_accept(
        net::make_strand(ioContext_),
        beast::bind_front_handler(&HttpListener::handleAccept,
                                  shared_from_this()));
  }

  void handleAccept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
      std::cerr << "Accept error: " << ec.message() << "\n";
    } else {
      std::make_shared<HttpSession>(std::move(socket))->start();
    }

    acceptConnection();
  }
};

int main() {
  const auto address = net::ip::make_address("0.0.0.0");
  constexpr unsigned short port = 8080;
  constexpr int threadCount = 4;

  net::io_context ioContext{threadCount};

  std::make_shared<HttpListener>(ioContext, tcp::endpoint{address, port})
      ->start();

  std::cout << "Jah API server started at https://" << address << ":" << port
            << std::endl;
  std::cout << "Using " << threadCount << " threads." << std::endl;

  std::vector<std::thread> threads;
  threads.reserve(threadCount - 1);
  for (auto i = threadCount - 1; i > 0; --i) {
    threads.emplace_back([&ioContext] { ioContext.run(); });
  }
  for (auto &thread : threads) {
    thread.join();
  }

  return EXIT_SUCCESS;
}
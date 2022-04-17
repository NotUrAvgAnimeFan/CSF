#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  std::string str;
  stringstream ss;
  ss << port;
  ss >> str;

  // TODO: call open_clientfd to connect to the server
  m_fd = open_clientf(hostname, &str);
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);

  
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  if (m_fd != 0) {
    close(m_fd);
  }
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  if (m_fd != 0) {
    return true;
  }
}

void Connection::close() {
  // TODO: close the connection if it is open
  if (is_open()) {
    close(m_fd);
  }
  
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  
  
}


bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately


}

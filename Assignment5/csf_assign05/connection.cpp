#include <iostream>
#include <string>

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
  std::string str = std::to_string(port);

  // TODO: call open_clientfd to connect to the server
  m_fd = open_clientfd(hostname.c_str(), str.c_str());
  

  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);

  
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  if (m_fd >= 0) {
    Close(m_fd);
  }
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return m_fd >= 0;
}

void Connection::close() {
  // TODO: close the connection if it is open
  if (is_open()) {
    Close(m_fd);
  }
  
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  std::string tag = msg.tag;

  if (tag != TAG_SLOGIN && tag != TAG_RLOGIN && tag != TAG_JOIN && tag != TAG_LEAVE && tag != TAG_SENDALL && tag != TAG_QUIT ) {
    m_last_result = INVALID_MSG;
    return false;
  }

  std::string complete = tag + ':' + msg.data + '\n';
  
  
  if (complete.size() > msg.MAX_LEN) {
    m_last_result = INVALID_MSG;
    return false;
  }
  
  
  int num_data = rio_writen(m_fd, complete.c_str(), complete.size());
  
  
  //message sent unsuccessfully
  if (num_data < 0) {
    
    //I/O or EOF error
    m_last_result = EOF_OR_ERROR;
    
    return false;
  }

  //message sent successfully 
  m_last_result = SUCCESS;
  return true; 
}


bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  
  
  
  char buf[msg.MAX_LEN];

  size_t num_read = rio_readlineb(&m_fdbuf, buf, sizeof(buf)); //stalls here and doesn't continue
  
  //message received unsuccessfully
  if (num_read < 0) {

    m_last_result = EOF_OR_ERROR;
    
    
    return false;
  }

  std::string messageReceived = buf;
  std::string nothing = "";
  
  //putting everything received in data section of message
  msg = Message(nothing, messageReceived);

  //split data into tag and data fields and get vector of strings back
  std::vector<std::string> result = msg.split_payload();
  msg = Message(result[0], result[1]);

  
  //message received successfully, store tag and data
  m_last_result = SUCCESS;
  return true;
  
}

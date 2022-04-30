/*
 * Connection functions making communication between clients and the server work successfully.
 * CSF Assignment 5 MS1
 * Ricardo Morales Gonzalez
 * rmorale5@jhu.edu
 *
 * Ana Kuri
 * akuri1@jhu.edu
 */


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
  
  rio_readinitb(&m_fdbuf, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  std::string str = std::to_string(port);

  
  m_fd = open_clientfd(hostname.c_str(), str.c_str());
  

  
  rio_readinitb(&m_fdbuf, m_fd);

  
}

Connection::~Connection() {
  
  if (m_fd >= 0) {
    Close(m_fd);
  }
}

bool Connection::is_open() const {
  
  return m_fd >= 0;
}

void Connection::close() {
  
  if (is_open()) {
    Close(m_fd);
  }
  
}

bool Connection::send(const Message &msg) {


  
  // stores the tag of message
  std::string tag = msg.tag;
 
  // checks to make sure tag in msg is valid
  if (tag != TAG_ERR && tag != TAG_OK && tag != TAG_SLOGIN && tag != TAG_RLOGIN && tag != TAG_JOIN && tag != TAG_LEAVE && tag != TAG_SENDALL && tag != TAG_QUIT && tag != TAG_DELIVERY && TAG_EMPTY) {
    
    // if not set m_last_result and returns false
    m_last_result = INVALID_MSG;
    return false;
  }

  // compiles the entire string to be sent to the server
  std::string complete = tag + ':' + msg.data + '\n';
  
  
  if (complete.size() > msg.MAX_LEN) {
    m_last_result = INVALID_MSG;
    return false;
  }
  
  // stores the amount of bites actaully written
  
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

  // creates a char buffer that will store message coming from the server
  char buf[msg.MAX_LEN];

  // reads message from server and stores contents in buf
  size_t num_read = rio_readlineb(&m_fdbuf, buf, sizeof(buf)); 
  
  //message received unsuccessfully
  if (num_read < 0) {

    // I/O or EOF error
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  // converts buf to string
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

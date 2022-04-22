#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // TODO: connect to server
  Connection conn;
  conn.connect(server_hostname, server_port)
  
  if (!(con.is_open())) {
    cerr << "connection could not be established" << endl;
    return 1; 
  }
  

  // TODO: send slogin message
  Message slogin = new Message(TAG_SLOGIN, username);
  if (!(conn.send(slogin)) {
      if (conn.m_last_result == conn.INVALID_MSG) {
	cerr << "INVALID username" << endl;
	return 1;
      } else {
	cerr << "username could not be sent" << endl;
	return 1;
      }
    };

  
  Message sloginResult;
  conn.receive(sloginResult);
  if (sloginResult.tag == sloginResult.TAG_ERR) {
      cerr << sloginResult.data << endl;
      return 1;
    };
  
  

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate

  std::string input;
  Message toSend;
  Message received;
  
  while(getline(std::cin, input)) {

    if (input[0] == '/') {
      if (input.substr(1,4) == "join") {
	toSend = new Message(sloginResult.TAG_JOIN, input.substr(6));
      } else if (input == "/leave") {
	toSend = new Message(sloginResult.TAG_LEAVE, "");
      } else if (input == "/quit") {
	toSend = new Message(sloginResult.TAG_QUIT, "");
      } else {
	cerr << "command not defined" << endl;
	continue;
      }
      
    } else {
      toSend = new Message(sloginResult.TAG_SENDALL, input);
    }

    
    if (conn.send(toSend);) {
    
      conn.receive(received);
      
      if (received.tag == received.TAG_ERR) {
	cerr << received.data << endl;
      }
      
    } else if (conn.m_last_result == conn.INVALID_MSG) {
      cerr << "invalid tag used or total message sized exceeded 250 characters" << endl;
      
    } else {
      cerr << "message send was unsuccessful" << endl;
    }
    
  }


  
  return 0;
}

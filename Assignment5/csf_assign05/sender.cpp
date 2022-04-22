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
  conn.connect(server_hostname, server_port);
  
  if (!(conn.is_open())) {
    std::cerr << "connection could not be established" << std::endl;
    return 1; 
  }
  
  
  // TODO: send slogin message
  Message slogin(TAG_SLOGIN, username);
  if (!(conn.send(slogin))) {
    if (conn.get_last_result() == conn.INVALID_MSG) {
      std::cerr << "INVALID username" << std::endl;
      return 1;
    } else {
      std::cerr << "username could not be sent" << std::endl;
      return 1;
    }
  }
  
  
  Message sloginResult;
  conn.receive(sloginResult);
  if (sloginResult.tag == "err") {
    std::cerr << sloginResult.data << std::endl;
    return 1;
  }
  
  
  
  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  
  std::string input;
  Message toSend;
  Message received;
  
  while(getline(std::cin, input)) {
    
    if (input[0] == '/') {
      if (input.substr(1,4) == "join") {
	toSend = Message("join", input.substr(6));
      } else if (input == "/leave") {
	toSend = Message("leave", "");
      } else if (input == "/quit") {
	toSend = Message("quit", "");
      } else {
	std::cerr << "command not defined" << std::endl;
	continue;
      }
      
    } else {
      toSend = Message("sendall", input);
    }

    
    if (conn.send(toSend)) {
    
      conn.receive(received);
      
      if (received.tag == "err") {
	std::cerr << received.data << std::endl;
      }
      
    } else if (conn.get_last_result() == conn.INVALID_MSG) {
      std::cerr << "invalid tag used or total message sized exceeded 250 characters" << std::endl;
      
    } else {
      std::cerr << "message send was unsuccessful" << std::endl;
    }
    
  }


  
  return 0;
}

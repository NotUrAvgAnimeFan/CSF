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
  
  

  // TODO: send slogin message
  Message slogin = new Message(TAG_SLOGIN, username);
  conn.send(slogin);


  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate

  std::string input;
  Message toSend;
  while(getline(std::cin, input)) {
    toSend = new Message("", input);
    std::vector<std::string> tagAndPayload = toSend.split_payload();
    
  }


  
  return 0;
}

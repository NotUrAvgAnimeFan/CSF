#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;


  // TODO: connect to server
  conn.connect(server_hostname, server_port);

  
  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  
  Message rlogin("rlogin", username);
  conn.send(rlogin);

  Message rloginResult("", "");
  conn.receive(rloginResult);
  
  if (rloginResult.tag == "err") {
    std::cerr << rloginResult.data << std::endl;
    return 1;
  }

  Message join("join", room_name);
  Message joinResult;
  
  conn.receive(joinResult);

  if (joinResult.tag == "err") {
    std::cerr << joinResult.data << std::endl;
    return 2;
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  
  Message received;
  std::string room;
  std::string sender;
  std::string message;

  
  while(conn.receive(received)) {

    if (received.tag == "delivery") {
      std::stringstream s(received.data);
      getline(s, room, ':');
      getline(s, sender, ':');
      getline(s, message);

      std::cout << sender << ": "<< message << std::endl;
    } 
    
    
  }
  


  return 0;
}

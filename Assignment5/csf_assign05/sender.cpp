#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"


int commandWhatToDo(Message &toSend, std::string &input, std::string &command, std::string &roomToJoin) {

  if (command == "/join") {
    if (roomToJoin.size() > 0) {
      toSend = Message("join", roomToJoin);
    } else {
      std::cerr << "/join command needs room name\n> ";
      
      return 1;
    }
    
    
  } else if (input == "/leave") {
    toSend = Message("leave", "");
    roomToJoin = "";
  } else if (input == "/quit") {
    toSend = Message("quit", "");
  } else {
    std::cerr << "command not defined\n> ";
    return 1;
  }

  return 0;  
}

void receiveAfterSend(Connection &conn, Message &toSend, Message &received) {
  if (conn.send(toSend)) {
    
    conn.receive(received);
    
    if (toSend.tag == "quit") {
      return;
    }
    
    if (received.tag == "err") {
      std::cerr << received.data << std::endl;
    }
    
    
  } else if (conn.get_last_result() == conn.INVALID_MSG) {
    std::cerr << "invalid tag used or total message sized exceeded 250 characters" << std::endl;
    
  } else {
    std::cerr << "message send was unsuccessful" << std::endl;
  }
  
  std::cout << "> ";

}
  
void userInput(Connection &conn) {
  std::string input;
  std::string command;
  std::string roomToJoin;
  Message toSend;
  Message received;
  int skipReceive = 0;
  std::cout << "> ";
  
  while(getline(std::cin, input)) {
        
    if (input[0] == '/') {
      std::stringstream s(input);
      std::getline(s, command, ' ');
      std::getline(s, roomToJoin);

      if (commandWhatToDo(toSend, input, command, roomToJoin) == 1) {
	continue;
      }
      
    } else if (input.size() == 0) {
      skipReceive = 1;
    } else {
      toSend = Message("sendall", input.substr(0, input.size()));
    }

    
    if (skipReceive == 0) {
      receiveAfterSend(conn, toSend, received);
    } else {
      std::cout << "> ";
      skipReceive = 0;
    }
    /*
    if (conn.send(toSend)) {
      
      conn.receive(received);

      if (toSend.tag == "quit") {
	break;
      }
      
      if (received.tag == "err") {
	std::cerr << received.data << std::endl;
      }
      
      
    } else if (conn.get_last_result() == conn.INVALID_MSG) {
      std::cerr << "invalid tag used or total message sized exceeded 250 characters" << std::endl;
      
    } else {
      std::cerr << "message send was unsuccessful" << std::endl;
    }

    std::cout << "> ";
    */
  }
  
}


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
      return 2;
    } else {
      std::cerr << "username could not be sent" << std::endl;
      return 3;
    }
  }


  
  Message sloginResult;  
  
  if(!(conn.receive(sloginResult))) {
    
    std::cerr << "slogin result could not be received" << std::endl;
  };
  
  if (sloginResult.tag == "err") {
    std::cerr << sloginResult.data << std::endl;
    return 4;
  }
  
  
  
  
  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate

  userInput(conn);
  
  
  return 0;
}

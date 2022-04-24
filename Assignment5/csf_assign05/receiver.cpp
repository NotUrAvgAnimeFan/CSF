/*
 * Receiver file handles the receiver client which involves constantly getting data from server
 * CSF Assignment 5 MS1
 * Ricardo Morales Gonzalez
 * rmorale5@jhu.edu
 *
 * Ana Kuri
 * akuri1@jhu.edu
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"


/*
 * Main function where all the work for the receive client takes place,
 * it constantly calls reads from the server and displays the messages 
 * sent.
 *
 * Parameters:
 * argc - an int describing the number of command line arguments given to main
 * argv - a vector of strings that are the command line argumetns given to main
 *
 * Returns:
 * an int 0 (if main exits without errors) or another number if an error is encountered
 *
 */


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
  
  //connects to server with given paramenters
  conn.connect(server_hostname, server_port);

  // creates a message with the info to log in to the server
  Message rlogin("rlogin", username);
  
  //checks to see if the send was successsful if not the apropriate message is given and returned
  if (!(conn.send(rlogin))) {
    if (conn.get_last_result() == conn.EOF_OR_ERROR) {
      std::cerr << "send was unsuccessful" << std::endl;
      return 1;
    } else {
      std::cerr << "wrong message format" << std::endl;
      return 2;
    } 
  }

  // creates an empty message to receive the result from our previous send
  Message rloginResult("", "");

  //checks to see if the receive was successful if not the appropriate message is given and number printed
  if (!(conn.receive(rloginResult))) {
    std::cerr << "receive was unsuccessful" << std::endl;
    return 3;
  }
 
  // checks to see if the tag given back by server is "err" indicating and error thus printing the
  // appropriate message and returning
  if (rloginResult.tag == "err") {
    std::cerr << rloginResult.data << std::endl;
    return 1;
  }

  // creates a join message to join a particular room inside the server
  Message join("join", room_name);

  // checks to see if the send was successful if not find out why and print the appropriate message
  if (!(conn.send(join))) {
    if (conn.get_last_result() == conn.EOF_OR_ERROR) {
      std::cerr << "send was unsuccessful" << std::endl;
      return 1;
    } else {
      std::cerr << "wrong message format" << std::endl;
      return 2;
    }
  }


  // creates an empty message to get the result from the previous send
  Message joinResult;

  //checks to see if we can successfully receive message sent back by server from our previous send
  if (!(conn.receive(joinResult))) {
    std::cerr << "receive was unsuccessful" << std::endl;
    return 3;
  }

  //checks to see if tag of received message is "err" and if so prints the appropriate message and returns
  if (joinResult.tag == "err") {
    std::cerr << joinResult.data << std::endl;
    return 2;
  }

  // creates fields that will be filled later
  Message received;
  std::string room;
  std::string sender;
  std::string message;

  // while we can receive a message from the server process it
  while(conn.receive(received)) {

    // if the tag is delivery then print the appropriate data
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

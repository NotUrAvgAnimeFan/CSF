/*
 * Sender client is implemented here meant to process all messages to be sent by user
 * CSF Assignment 5 MS1
 *
 * Ricardo Morales Gonzalez
 * rmorale5@jhu.edu
 *
 * Ana Kuri
 * akuri1@jhu.edu
 */


#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"



/*
 * Determines what steps to take depending on what command is given by the user
 *
 * Parameters:
 *   toSend - a message object to be sent
 *   input - the input by the user as a string
 *   command - the command given by the user aka. what the user wants the program to do
 *   roomToJoin - holds the string of the room that the user might want ot join
 *
 * Returns:
 *   an int indicating wheter to continue on with the receive or to skip that part 
 */
int commandWhatToDo(Message &toSend, std::string &input, std::string &command, std::string &roomToJoin) {

  // checks to see what the command is and takes the appropriate steps
  if (command == "/join") {

    // makes sure that join command has a room to join
    if (roomToJoin.size() > 0) {
      toSend = Message("join", roomToJoin);
    } else {
      // otherwise print an error message
      std::cerr << "/join command needs room name\n> ";
      
      return 1;
    }
    
    
  } else if (input == "/leave") {
    //creates a leave message
    toSend = Message("leave", "");
    roomToJoin = "";
  } else if (input == "/quit") {
    //creates a quit message
    toSend = Message("quit", "");
  } else {

    // if no other command then print an error since command is not valid
    std::cerr << "command not defined\n> ";
    return 1;
  }

  return 0;  
}

/*
 * Receives a message from the server after a message has been sent
 *
 * Parameters:
 *   conn - the connection which will be used to receive messages from the server
 *   toSend - message that was previously sent
 *   received - will hold the appropriate tag and data that the server sends back
 *
 * Returns:
 *   an int indicating if a quit command was previously issued and making sure that message gets an
 *   appropriate response 
 */

int receiveAfterSend(Connection &conn, Message &toSend, Message &received) {
  // checks to see if a messsage is sent while sending at the same time
  if (conn.send(toSend)) {
    // receives message from server and stores contents in received
    conn.receive(received);

    // if tag of sent message was quit then return 1 to signal quitting of the program
    if (toSend.tag == "quit") {
      return 1;
    }

    // if received has tag err that means something went wrong and the data of the message
    // received is printed to standard error
    if (received.tag == "err") {
      std::cerr << received.data << std::endl;
    }
    
    // if sent isn't successful check why and print the appropriate message to standard error
  } else if (conn.get_last_result() == conn.INVALID_MSG) {
    std::cerr << "invalid tag used or total message sized exceeded 250 characters" << std::endl;
    
  } else {
    std::cerr << "message send was unsuccessful" << std::endl;
  }
  
  std::cout << "> ";
  return 0;
}


/*
 * Processes user input with a while loop and sends the appropriate values to
 * receiveAfterSend and commandWhatToDo helper functions
 *
 * Parameters:
 *   conn - the connection that will be used to connect to the server to send and receive messages
 *
 * Returns:
 *   nothing
 */

void userInput(Connection &conn) {

  // fields that will be used to process input from user 
  std::string input;
  std::string command;
  std::string roomToJoin;
  Message toSend;
  Message received;
  int skipReceive = 0;


  std::cout << "> ";

  // continually gets user input and stores it in input string
  while(getline(std::cin, input)) {

    // checks to see if input has the sign of being a command
    if (input[0] == '/') {

      // if so separate into command and room to join
      std::stringstream s(input);
      std::getline(s, command, ' ');
      std::getline(s, roomToJoin);

      // send to commandWhatToDo to process what to do
      if (commandWhatToDo(toSend, input, command, roomToJoin) == 1) {
	continue;
      }

      // if size == 0 then nothing was inputted so no processing needs to happen
    } else if (input.size() == 0) {
      skipReceive = 1;

      // otherwise input is message and as such toSend is set as a message to be sent to all receivers
    } else {
      toSend = Message("sendall", input.substr(0, input.size()));
    }


    // if skipReceive == 0 then receive message from server
    if (skipReceive == 0) {

      // if message from server is == 1 then return to quit the program 
      if (receiveAfterSend(conn, toSend, received) == 1) {
	return;
      }

      // styling guideline
    } else {
      std::cout << "> ";
      skipReceive = 0;
    }
  }
  
}


/*
 * main function that starts the connection from client to the server
 *
 * Parameters:
 *   argc - an int indicatin the number of command line arguments given to the client
 *   argv - a vector of strings that are the command line arguments given to the client
 *
 * Returns:
 *   0 if program exits normally another integer otherwise
 */
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

  // sets up a connection object and connects it to the server with the given server_hostname, and server_port
  Connection conn;
  conn.connect(server_hostname, server_port);

  //makes sure that the connection is open
  if (!(conn.is_open())) {
    std::cerr << "connection could not be established" << std::endl;
    return 1; 
  }

  // creates a message to login the sender with the given usernam
  Message slogin(TAG_SLOGIN, username);

  // sends and makes sure that the message was sent successfully to the server if not prints the appropriate
  // error message
  if (!(conn.send(slogin))) {
    if (conn.get_last_result() == conn.INVALID_MSG) {
      std::cerr << "INVALID username" << std::endl;
      return 2;
    } else {
      std::cerr << "username could not be sent" << std::endl;
      return 3;
    }
  }


  // makes an empty message which will be filled with the message received from the server in response to the
  // earlier sent message
  Message sloginResult;  

  // receives and makes sure that receive from server was successful if not print error message
  if(!(conn.receive(sloginResult))) {
    
    std::cerr << "slogin result could not be received" << std::endl;
  };
  
  if (sloginResult.tag == "err") {
    std::cerr << sloginResult.data << std::endl;
    return 4;
  }
  
  // send open connection to userInput to continually get input from the user and process it
  userInput(conn);
  

  // return normally
  return 0;
}

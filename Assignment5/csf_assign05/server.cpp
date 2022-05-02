/*
 * server.cpp file handles all connections and messges between clients with threads for each one  
 * CSF Assignment 5 MS2
 * Ricardo Morales Gonzalez
 * rmorale5@jhu.edu
 *
 * Ana Kuri
 * akuri1@jhu.edu
 *
 */




#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

struct ConnInfo {
  Connection *conn;
  Server *server;
  
  ConnInfo(Connection *conn, Server *server) : conn(conn), server(server) { }
  ~ConnInfo() {
    // destroy connection when ConnInfo object is destroyed
    delete conn;
  }
};

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

  /*
   * Handles logic for chatting with receiver
   * 
   * Parameters:
   *  msg - Message that contains join information
   *  info - The connection that will be used to send message to and from clients
   *  user - User that is the receiver and will get messages from its queue
   *
   * Returns:
   *  nothing
   *
   */
  
  
  void chat_with_receiver(Message &msg, std::unique_ptr<ConnInfo> &info, User *user) {
    // Receiver loop ------------------------------

    if (msg.tag != TAG_JOIN) {
      //sends an error message if first argument is not join
      info->conn->send(Message(TAG_ERR, "second message for receiver should be join"));
      return;
    } else {
      //sends an okay message back to indicate successful join
      info->conn->send(Message(TAG_OK, "successfully joined desired room"));
    }

    //room to add user to
    Room *room = info->server->find_or_create_room(msg.data);
    
    room->add_member(user);

    // receiver loop that will constantly get message from queue
    while (true) {

      
      // try to deque a Message from the user's MessageQueue
      Message* received = user->mqueue.dequeue();
     
      // if a Message was successfully dequeued, send a "delivery"
      // message to the receiver. If the send is unsuccessful,
      // break out of the loop (because it's likely that the receiver
      // has exited and the connection is no longer valid)

      //checks to see if a message was dequeued
      if (received != nullptr) {

	//if so send a delivery message back to client
	if (!info->conn->send(Message(TAG_DELIVERY, msg.data + ':' + received->data))) {
	  // if send doesn't work exit
	  break;
	}

	//delete the received message to stop leaks
	delete received;
	
      }
      
      // loops again
    }
    
    // make sure to remove the User from the room
    room->remove_member(user);
    
  }


  /*
   * Handles first join for sender by looping until join message received
   * 
   * Parameters:
   *  msg - Message that contains join information
   *  info - The connection that will be used to send message to and from clients
   *
   * Returns:
   *  -1 if error or quit message sent, 0 if exited as desired
   *
   */
  int join_loop(Message &msg, std::unique_ptr<ConnInfo> &info) {

    // checks to see if tag is not join as desired
    while(msg.tag != TAG_JOIN) {

      // if tag quit then send ok message back 
      if (msg.tag == TAG_QUIT) {
        info->conn->send(Message(TAG_OK, "quit program"));

	return -1;
      }

      // if not quit then send error message back if that fails then return -1
      if (!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	return -1;
      }
      

      // try to receive message again to get Join Message if failed to receive check
      // connection status if invalid message sent error back
      if (!info->conn->receive(msg)) {
	if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	  info->conn->send(Message(TAG_ERR, "invalid message"));
	}
	return -1;
      }
    }

    // exit normally
    return 0;
    
    
    
  }
  
  
  /*
   * Sendall_function takes care of sending message to all users in a room
   * 
   * Parameters:
   *  msg - Message that contains data of message to be sent
   *  info - The connection that will be used to send message to and from clients
   *  room - indicates which room to broadcast message to
   *  username - username of the sender of the message
   *
   * Returns:
   *  -1 if err occured which prompts exit or 0 if exits normally
   *
   */
  
  int sendall_function(Message &msg, std::unique_ptr<ConnInfo> &info, Room* room, std::string &username) {

    // checks to make sure there is a valid room to send message to
    if (room == nullptr) {
      // if not send an error message back to client
      if (!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	// return -1 if message could not be sent back
	return -1;
      }
      
      // makes sure that message being sent is of valid size
    } else if ((msg.data.size() + 1 + msg.tag.size()) > 255 || msg.data.size() == 0) {

      // if format is wrong sent error message back with description of error
      if (!info->conn->send(Message(TAG_ERR, "message format is wrong"))) {

	// return -1 if message could not sucessfully be sent
	return -1;
      }
      
    } else {

      //if everything else passes then broadcast message to all users of room
      
      room->broadcast_message(username, msg.data);

      // send okay message back to client indicating message was sent
      if (!info->conn->send(Message(TAG_OK, "message sent to all users in room"))) {
	// if sent was unsuccessful then exit with -1
	return -1;
      }
      
    }
    //exit normally
    return 0;
  }


  /*
   * leave_function handles the leaving of a sender from a room
   * 
   * Parameters:
   *  msg - contains info on message data
   *  info - connection to send messages to client
   *  room - will be changed to nullptr to indicate not in a room
   *
   * Returns:
   *  nothings since it prints all info
   *
   */
  int leave_function(Message &msg, std::unique_ptr<ConnInfo> &info, Room* &room) {

    // user is not in a room
    if (room == nullptr) {

      // send error back indicating user not in a room so cannot send leave message
      if(!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	// if error message could not be sent then exit with -1 indicating disconnect
	return -1;
      }

      // user is in a room so we set the room pointer to null to indicate not in a room anymore
    } else {
      room = nullptr;

      // send message to client indicating leaving was sucessful
      if (!info->conn->send(Message(TAG_OK, "left room"))) {
	// if message send was unsucessful then exit with -1
	return -1;
      }
      
      
    }
    //exit normally
    return 0;
  }
  

  /*
   * receive function makes sure to receive message from respective clients
   * 
   * Parameters:
   *  msg - message object that will hold the message received from client
   *
   * Returns:
   *  -1 if messsage could not be sucessfully received, 0 if everything goes well
   *
   */
  int receive_function(Message &msg, std::unique_ptr<ConnInfo> &info) {
    // tries to get message from client and store contents in msg
    if (!info->conn->receive(msg)) {
      // if not check why we couldn't receive message
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	// if invalid message then send error message back with proper description
	info->conn->send(Message(TAG_ERR, "invalid message"));
      }
      // return -1 to indicate message not received correctly
      return -1;
    }

    // exit normally indicating success
    return 0;
  }
  
  /*
   * split_work function sends variables to whatever method they need to go denpending on 
   * tag in message sent by the user
   * 
   * Parameters:
   *  msg - contains info on user sent message to the server
   *  info - connection to send messages to client
   *  room - contains the current room in which the user is in
   *  username - a string indicating the name of the user
   *
   * Returns:
   *  nothing
   *
   */
  void split_work(Message &msg, std::unique_ptr<ConnInfo> &info, Room* &room, std::string &username) {

    // while loop to constantly get message from user and perform desired task
    while (true) {

      // make sure that receive was successful if not return
      if (receive_function(msg, info) == -1) {
	return;
      }

      // if tag is join follow these instructions
      if (msg.tag == TAG_JOIN) {
	room = info->server->find_or_create_room(msg.data);

	// try to send ok message back to client
	if (!info->conn->send(Message(TAG_OK, "joined room" + msg.data))) {
	  // if cannot send message back exit
	  return;
	}
	
	// if tag is leave then follow these instructions
      } else if (msg.tag == TAG_LEAVE) {

	// perform leave function if it doesn't work then exit
	if (leave_function(msg, info, room) == -1) {
	  return;
	}
	
	//if tag is sendall perform these instructions
      } else if (msg.tag == TAG_SENDALL) {

	// try to send message to room if fails then exit
	if (sendall_function(msg, info, room, username) == -1) {
	  return;
	}

	//if message quit then send quit message to client and exit
      } else if (msg.tag == TAG_QUIT) {
        info->conn->send(Message(TAG_OK, "quit program"));

	return;
      } else {

	// tag doesn't exist so send back an error message
	if (!info->conn->send(Message(TAG_ERR, "invalid tag"))) {

	  // if we could not send message back to client then exit
	  return;
	}
      }
      
    }
    
  }
  
  /*
   * chat_with_sender sends relevant information to join loops and split work to perform
   *  work to process client instructions
   * 
   * Parameters:
   *  msg - will hold message sent to server by client and we will use information to do desired functions
   *  info - connection to send messages to client
   *  username - a string indicating the name of the user
   *
   * Returns:
   *  nothing
   *
   */
  void chat_with_sender(Message msg, std::unique_ptr<ConnInfo> &info, std::string &username) {
	 
    //sender loop --------------------------

    // join loop to try and get a join message from sender
    if (join_loop(msg, info) == -1) {
      // if cannot then exit
      return;
    }
    
    
    // send an ok message to client to indicate joining of desired room
    if (!info->conn->send(Message(TAG_OK, "successfully joined desired room"))) {
      // if message send fails then exit
      return;
    }


    // get desired room
    Room *room = info->server->find_or_create_room(msg.data);

    // perform work for sender
    split_work(msg, info, room, username);

    
  }

  /*
   * receive_and_login receives login message from clients/ also gets join message
   * 
   * Parameters:
   *  msg - will be used to house login data request
   *  username - a string with the username of the respective client user
   *  info - connection to send messages to client
   *  user - a user object with all relevant info of user that will be logged in
   *  join - will contain join request from clients to process
   *
   * Returns:
   *  -1 if something goes wrong and we have to exit or 0 if exit is normal
   *
   */
  int receive_and_login(Message &msg, std::string &username, std::unique_ptr<ConnInfo> &info, User* &user, Message &join) {

    // tries to get message from client with login info
    if (!info->conn->receive(msg)) {
      // if we could not receive message determine why and send appropriate error message to client
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	info->conn->send(Message(TAG_ERR, "invalid message"));
      }

      //exit with -1 to indicate error
      return -1;
    }

    // make sure that the tag is either login for sender or login for receiver
    if (msg.tag != TAG_SLOGIN && msg.tag != TAG_RLOGIN) {
      // if it is not send error message indicating that the first message should have been login message
      info->conn->send(Message(TAG_ERR, "first message should be slogin or rlogin"));
      // exit with -1 to indicate error
      return -1;
    }

    // set username as the data from msg received
    username = msg.data;
    // try to send ok message back to client to indicate success in receiving login message
    if (!info->conn->send(Message(TAG_OK, "welcome " + username))) {
      // if send fails return with -1 to indicate error
      return -1;
    }

    // sets username as new username object with giver username
    user = new User(username);

    // tries to receive join message from client
    if (!info->conn->receive(join)) {
      // if receive doesn't work determine why and send error message accordingly
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	info->conn->send(Message(TAG_ERR, "invalid message"));
      }

      // exit with -1 to signal error
      return -1;
    }

    // exit normally
    return 0;
  }
 

  /*
   * worker sends correct info to all helper functions to perform everything that needs to be handled by thread
   * 
   * Parameters:
   *  arg - arguments that contain relevant information such as connection
   *
   * Returns:
   *  nothing
   *
   */
  void *worker(void *arg) {
    
    pthread_detach(pthread_self());
    
    ConnInfo *info_ = static_cast<ConnInfo *>(arg);
    
    // use a std::unique_ptr to automatically destroy the ConnInfo object
    // when the worker function finishes; this will automatically ensure
    // that the Connection object is destroyed
    std::unique_ptr<ConnInfo> info(info_);
    
    Message msg;
    std::string username;
    User *user = nullptr;
    Message join;

    
    if (receive_and_login(msg, username, info, user, join) == -1) {
      return nullptr;
    }    
    
    // separate into different scenarios depending on if Sender or Receiver was called

    // makes sure that tag is for receiver and sends relevant info to chat_with_receiver function
    if (msg.tag == TAG_RLOGIN) {
      chat_with_receiver(join, info, user);
      
      
    } else {
      
      //sender loop --------------------------
      chat_with_sender(join, info, username);
       
    }

    // exit once everything is done
    return nullptr;
    
    
  }
  
}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  pthread_mutex_destroy(&m_lock);
  
}

bool Server::listen() {
  std::string port = std::to_string(m_port);
  m_ssock = open_listenfd(port.c_str());
  return m_ssock >= 0;
}

void Server::handle_client_requests() {

  assert(m_ssock >= 0);
  
  while (true) {
    int clientfd = accept(m_ssock, nullptr, nullptr);
    if (clientfd < 0) {
      std::cerr << "Error accepting connection\n";
      return;
    }
    
    ConnInfo *info = new ConnInfo(new Connection(clientfd), this);
    
    pthread_t thr_id;
    if (pthread_create(&thr_id, nullptr, worker, static_cast<void *>(info)) != 0) {
      std::cerr << "Could not create thread\n";
      return;
    }
  }
  
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // this function can be called from multiple threads, so
  // make sure the mutex is held while accessing the shared
  // data (the map of room names to room objects)
  Guard g(m_lock);

  Room *room;

  auto i = m_rooms.find(room_name);
  if (i == m_rooms.end()) {
    // room does not exist yet, so create it and add it to the map
    room = new Room(room_name);
    m_rooms[room_name] = room;
  } else {
    room = i->second;
  }

  return room;
  
}

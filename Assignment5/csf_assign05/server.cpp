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
  
  void chat_with_receiver(Message &msg, std::unique_ptr<ConnInfo> &info, User *user) {
    // Receiver loop ------------------------------

    if (msg.tag != TAG_JOIN) {
      info->conn->send(Message(TAG_ERR, "second message for receiver should be join"));
      return;
    } else {
      info->conn->send(Message(TAG_OK, "successfully joined desired room"));
    }
    
    Room *room = info->server->find_or_create_room(msg.data);
    
    room->add_member(user);
    
    while (true) {
      
      // try to deque a Message from the user's MessageQueue
      Message* received = user->mqueue.dequeue();
     
      // if a Message was successfully dequeued, send a "delivery"
      // message to the receiver. If the send is unsuccessful,
      // break out of the loop (because it's likely that the receiver
      // has exited and the connection is no longer valid)
      
      if (received != nullptr) {
	
	if (!info->conn->send(Message(TAG_DELIVERY, msg.data + ':' + received->data))) {
	  break;
	}
	
	delete received;
	
      }
      
      // loops again
    }
    
    // make sure to remove the User from the room
    room->remove_member(user);
    
  }

  
  int join_loop(Message &msg, std::unique_ptr<ConnInfo> &info) {
    
    while(msg.tag != TAG_JOIN) {
      
      if (!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	return -1;
      }
      
      
      if (!info->conn->receive(msg)) {
	if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	  info->conn->send(Message(TAG_ERR, "invalid message"));
	}
	return -1;
      }
    }
    
    return 0;
    
    
    
  }


  
  int sendall_function(Message &msg, std::unique_ptr<ConnInfo> &info, Room* room, std::string &username) {

    if (room == nullptr) {
      
      if (!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	return -1;
      }
      
      
    } else if ((msg.data.size() + 1 + msg.tag.size()) > 255 || msg.data.size() == 0) {
      
      if (!info->conn->send(Message(TAG_ERR, "message format is wrong"))) {
	return -1;
      }
      
    } else {
      
      room->broadcast_message(username, msg.data);
      
      if (!info->conn->send(Message(TAG_OK, "message sent to all users in room"))) {
	return -1;
      }
      
    }
    return 0;
  }

  int leave_function(Message &msg, std::unique_ptr<ConnInfo> &info, Room* room) {
    
    if (room == nullptr) {
      
      if(!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	return -1;
      }
      
    } else {
      room = nullptr;
      
      if (!info->conn->send(Message(TAG_OK, "left room"))) {
	return -1;
      }
      
      
    }
    
    return 0;
  }

  int receive_function(Message &msg, std::unique_ptr<ConnInfo> &info) {
    if (!info->conn->receive(msg)) {
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	info->conn->send(Message(TAG_ERR, "invalid message"));
      }
      return -1;
    }
    return 0;
  }


  void split_work(Message &msg, std::unique_ptr<ConnInfo> &info, Room* room, std::string &username) {

    while (true) {
      
      if (receive_function(msg, info) == -1) {
	return;
      }
      
      if (msg.tag == TAG_JOIN) {
	room = info->server->find_or_create_room(msg.data);
	
	if (!info->conn->send(Message(TAG_OK, "joined room" + msg.data))) {
	  return;
	}
	
	
      } else if (msg.tag == TAG_LEAVE) {
	
	if (leave_function(msg, info, room) == -1) {
	  return;
	}
	
	
      } else if (msg.tag == TAG_SENDALL) {
	if (sendall_function(msg, info, room, username) == -1) {
	  return;
	}
      } else {
	if (!info->conn->send(Message(TAG_ERR, "invalid tag"))) {
	  return;
	}
      }
      
    }
    
  }
  
  
  void chat_with_sender(Message msg, std::unique_ptr<ConnInfo> &info, std::string &username) {
	 
    //sender loop --------------------------

    if (join_loop(msg, info) == -1) {
      return;
    }
    
    /*
    while(msg.tag != TAG_JOIN) {
      
      if (!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	return;
      }
      
      
      if (!info->conn->receive(msg)) {
	if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	  info->conn->send(Message(TAG_ERR, "invalid message"));
	}
	return;
      }
    }
    */

    
    
    if (!info->conn->send(Message(TAG_OK, "successfully joined desired room"))) {
      return;
    }


    
    Room *room = info->server->find_or_create_room(msg.data);

    split_work(msg, info, room, username);

    /*
    while (true) {
      
      if (receive_function() == -1) {
	return;
      }
      
      
      if (!info->conn->receive(msg)) {
	if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	  info->conn->send(Message(TAG_ERR, "invalid message"));
	}
	return;
      }
      

      
      if (msg.tag == TAG_JOIN) {
	room = info->server->find_or_create_room(msg.data);
	
	if (!info->conn->send(Message(TAG_OK, "joined room" + msg.data))) {
	  return;
	}
	
	
      } else if (msg.tag == TAG_LEAVE) {

	if (leave_function(&msg, &info, room) == -1) {
	  return;
	}
	
	
	if (room == nullptr) {
	  
	  if(!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	    return;
	  }
	  
	} else {
	  room = nullptr;
	  
	  if (!info->conn->send(Message(TAG_OK, "left room"))) {
	    return;
	  }
	  
	  
	}
	
	
      } else if (msg.tag == TAG_SENDALL) {
	if (sendall_function(msg, info, room) == -1) {
	  return;
	}
	
	
	if (room == nullptr) {
	  
	  if (!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	    return;
	  }
	  
	  
	} else if ((msg.data.size() + 1 + msg.tag.size()) > 255 || msg.data.size() == 0) {
	  
	  if (!info->conn->send(Message(TAG_ERR, "message format is wrong"))) {
	    return;
	  }
	  
	} else {
	  
	  room->broadcast_message(username, msg.data);
	  
	  if (!info->conn->send(Message(TAG_OK, "message sent to all users in room"))) {
	    return;
	  }
	  
	}
	
      } else {
	if (!info->conn->send(Message(TAG_ERR, "invalid tag"))) {
	  return;
	  }
      }
	
    }
    */
  }
  
  void *worker(void *arg) {
    
    pthread_detach(pthread_self());
    
    ConnInfo *info_ = static_cast<ConnInfo *>(arg);
    
    // use a std::unique_ptr to automatically destroy the ConnInfo object
    // when the worker function finishes; this will automatically ensure
    // that the Connection object is destroyed
    std::unique_ptr<ConnInfo> info(info_);
    
    Message msg;
    
    if (!info->conn->receive(msg)) {
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	info->conn->send(Message(TAG_ERR, "invalid message"));
      }
      return nullptr;
    }
    
    if (msg.tag != TAG_SLOGIN && msg.tag != TAG_RLOGIN) {
      info->conn->send(Message(TAG_ERR, "first message should be slogin or rlogin"));
      return nullptr;
    }
    
    std::string username = msg.data;
    if (!info->conn->send(Message(TAG_OK, "welcome " + username))) {
      return nullptr;
    }
    
    
    User *user = new User(username);
    Message join;
    
    if (!info->conn->receive(join)) {
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	info->conn->send(Message(TAG_ERR, "invalid message"));
      }
      return nullptr;
    }
    
    
    
    // separate into different scenarios depending on if Sender or Receiver was called
    
    if (msg.tag == TAG_RLOGIN) {
      chat_with_receiver(join, info, user);
      
      /*
      // Receiver loop ------------------------------
      if (join.tag != TAG_JOIN) {
	info->conn->send(Message(TAG_ERR, "second message for receiver should be join"));
	return nullptr;
      } else {
	info->conn->send(Message(TAG_OK, "successfully joined desired room"));
      }
      
      Room *room = info->server->find_or_create_room(join.data);
      
      
      room->add_member(user);
      
      while (true) {
	
	// try to deque a Message from the user's MessageQueue
	Message* received = user->mqueue.dequeue();

        
	
	// if a Message was successfully dequeued, send a "delivery"
	// message to the receiver. If the send is unsuccessful,
	// break out of the loop (because it's likely that the receiver
	// has exited and the connection is no longer valid)
	
	if (received != nullptr) {
	    
	  if (!info->conn->send(Message(TAG_DELIVERY,join.data + ':' + received->data))) {
	    break;
	  }

	  delete received;
	  
	}

	// loops again
      }
      
      // make suer to remove the User from the room
      room->remove_member(user);
      */
    } else {
      
      //sender loop --------------------------
      chat_with_sender(join, info, username);
      
      /*
      while(join.tag != TAG_JOIN) {
	
	if (!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	  return nullptr;
	}
	
	
	if (!info->conn->receive(join)) {
	  if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	    info->conn->send(Message(TAG_ERR, "invalid message"));
	  }
	  return nullptr;
	}
      }
      
      if (!info->conn->send(Message(TAG_OK, "successfully joined desired room"))) {
	return nullptr;
      }
      
      Room *room = info->server->find_or_create_room(join.data);
      
      
      while (true) {
	if (!info->conn->receive(join)) {
	  if (info->conn->get_last_result() == Connection::INVALID_MSG) {
	    info->conn->send(Message(TAG_ERR, "invalid message"));
	  }
	  return nullptr;
	}
	
	if (join.tag == TAG_JOIN) {
	  room = info->server->find_or_create_room(join.data);
	  
	  if (!info->conn->send(Message(TAG_OK, "joined room" + join.data))) {
	    return nullptr;
	  }
	  
	  
	} else if (join.tag == TAG_LEAVE) {
	  
	  if (room == nullptr) {
	    
	    if(!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	      return nullptr;
	    }
	    
	  } else {
	    room = nullptr;
	    
	    if (!info->conn->send(Message(TAG_OK, "left room"))) {
	      return nullptr;
	    }
	    
	    
	  }
	  
	} else if (join.tag == TAG_SENDALL) {
	  
	  if (room == nullptr) {
	    
	    if (!info->conn->send(Message(TAG_ERR, "not in a room"))) {
	      return nullptr;
	    }
	    
	    
	  } else if ((join.data.size() + 1 + join.tag.size()) > 255 || join.data.size() == 0) {
	    
	    if (!info->conn->send(Message(TAG_ERR, "message format is wrong"))) {
	      return nullptr;
	    }
	    
	  } else {

	    room->broadcast_message(username, join.data);
	    
	    if (!info->conn->send(Message(TAG_OK, "message sent to all users in room"))) {
	      return nullptr;
	    }
	    
	  }
	  
	} else {
	  if (!info->conn->send(Message(TAG_ERR, "invalid tag"))) {
	    return nullptr;
	  }
	}
	
      }
      */
       
    }
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

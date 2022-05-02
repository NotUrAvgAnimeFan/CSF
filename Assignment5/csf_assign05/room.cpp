#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

#include <iostream>

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  pthread_mutex_init(&lock, nullptr);
  
}

Room::~Room() {
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  
  Guard g(lock);
  
  members.insert(user);

  
}

void Room::remove_member(User *user) {
  //------ critical Section
  Guard g(lock);
  
  members.erase(user);

}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {

  //-------- Critical Section

  Guard g(lock);
  
  std::set<User*>::iterator it;
  
  for (it = members.begin(); it != members.end(); it++) {
    
    if ((*it)->username.compare(sender_username) != 0) {
      Message *toQueue = new Message(TAG_DELIVERY, sender_username + ':' + message_text);

      (*it)->mqueue.enqueue(toQueue);
    }
  }


}

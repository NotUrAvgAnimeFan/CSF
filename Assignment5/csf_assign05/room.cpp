#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  pthread_mutex_init(&lock, nullptr);
  
}

Room::~Room() {
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  members.insert(user);
  
}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  members.erase(user);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room

  for (it = members.begin(); it != members.end(); it++) {
    if (*it->username.compare(sender_username) != 0) {
      Message toQueue(TAG_OK, message_text);
      *it->mqueue.enqueue(toQueue);
    }
  }


}

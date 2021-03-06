/*
 * Message Queue which are part of all users, handles
 * how messagues are queued and dequeued
 *
 * CSF Assignment 5 MS2
 * Ricardo Morales Gonzalez
 * rmorale5@jhu.edu
 *
 * Ana Kuri
 * akuri1@jhu.edu
 */


#include <cassert>
#include <ctime>
#include "message_queue.h"

MessageQueue::MessageQueue() {
  pthread_mutex_init(&m_lock, nullptr);
  sem_init(&m_avail, 0, 0);
  
}

MessageQueue::~MessageQueue() {
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);

}

void MessageQueue::enqueue(Message* msg) {
 
  m_messages.push_back(msg);

  
  sem_post(&m_avail);
  
}

Message* MessageQueue::dequeue() {
  
  struct timespec ts;

  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;

  
  
  int waitResult = sem_timedwait(&m_avail, &ts);

  if (waitResult == -1) {
    return nullptr;
  }

  Message* msg = m_messages.front();

  
  m_messages.pop_front();

  return msg;
}

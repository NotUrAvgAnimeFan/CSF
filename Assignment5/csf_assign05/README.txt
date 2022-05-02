Ricardo Morales Gonzalez
rmorale5@jhu.edu

Ana Kuri
akuri1@jhu.edu


MS1-------
Both partners worked together at the same time through a zoom call and as such we both contributed
the same amount to the entire project. The only parts where we weren't working together was when implementing
small bits of code or fixing bugs. As such the implementation of MS1 was equally completed by both members.


Eventually your report about how you implemented thread synchronization
in the server should go here

MS2 Synchronization report-
I determined that most of the syncrhonization was goig to be around the Room class due to its nature in
accessing users' information especifically the message_queues which are accessed by both
senders and receivers and perhaps even at the same time depending on the timing of the requests by
sender and receiver clients. Because of this the implementation of the Guard class was used. The
Guard allowed for the locking and unlocking of objects so that no other requests would mess up the
data and potentially cause errors in the data accessed. As such we determined critical sections to be inside the add_member, remove_member,
and braodcast_message functions of the room.cpp file.
I decided to add locks through the Gurad class to prevent such data inconsistencies
when accessing users' message_queues. By using Guard we were able to lock everything inside the functions
while the work was preformed avoiding inconsitencies within threads that could potentially cause issues down
the road. In this same manner, a guard was added to the server.cpp function
find_or_create_room to make sure that the room we are accessing does not get its information changed
while we are trying to access it. Again using the Guard class prevented us from causing deadlocks in
case we forgot to unlock object for their use in other threads. Guard auto did all of the work which
made it easy to avoid mistakes.

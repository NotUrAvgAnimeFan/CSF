README for CSF Assignment 3 Milestone 3
Conains contributions

--Ricardo Morales Gonzalez rmorale5@jhu.edu
--Ana Kuri akuri1@jhu.edu

Best Cache Configuration ----



What is the best Cache Configuartion?-----------
In order to find the best cache configuration we decided to keep 2 variables
the same. One was the trace which we were using to tests the different combinations
of parameters to run on ./csim. The other parameter was the total size of the cache.
We decided to make the cache a total of 1MB and changed the number of sets, number
of slots per set, and the size of the slots themselves so that they would equate a grand
total of 1MB of space inside our cache simulation. While changing the number of sets,
slots per set, and size of the slots we noticed a patterned. When increasing the size of
the slots themselves, the total cycle count would increase despite the load and store misses
decreasing resulting in hitting more often. This can be seen when csim was run with parameters
4096 16 16 write-allocate write-back lru < gcc.trace. These parameters caused a total cycle
count of 5,004,092 while ./csim 512 512 4 write-allocate write-back lru < gcc.trace resulted in
only 3,694,383 a decrease of around 1.5 million cycles. This is despite ./csim 4096 16 16
write-allocate write-back lru < gcc.trace having a load hit count of 315,854, a load miss count
of 2,343, a store hit count of 188,617, and a store miss count of 8,869. ./csim 512 512 4 write-allocate
write-back lru < gcc.trace, on the other hand. resulted in a load hit count of 313,766, a load miss
count of 4,431, a store hit count of 170,130, and a store miss count of 27,356. As you can see
from the data shown, despite having a higher cycle count, having a higher slot size resulted in an
overall increase of total cycle count. Since we decided that the performance of our code was better
represented by the total cycle count we decided to opt for ./csim 512 512 4 as the base of our
tests to determine which of the other variable combinations would produce the fatest cache in
terms of cycle count. Through the running of 6 different test to put to trial all possible
variable combinations we discovered that fifo and lru eviction practices resulted in the
exact same load,store hit and miss counts. The real difference was then a result of
what the cache was to do in the event of a store miss and store hit. Once all trials were
run, the cache setup with the lowest total cycle count resulted to the one with
write-allocate and write-back included in the call. This could be due to the fact
that write-allocate, upon a store miss, loads the data into the cache which later results in
a hit if that same data is to be accessed later as opposed to no-write-allocate which does not
load the data and thus causes misses if the data that wasn't writte to cache is wanted. The
addition of write-back makes is so that upon a hit the data in that specific spot in cache
will only be written to main memory in the case that that piece of data is evicted. This
thus saves the cycle count for until that data is evicted. As a result if the data is never
evicted the count does not acutate saving us from unneccessary cycles which in the end
result in a slower cache setup. In the end our opinion for the best cache setup is
one in which the last numerical parameter is the lowest size a slot can be with the
number of sets and slots per set numbers able to be interchaged between each other as
from our results below it was observed that they do not play a role in any of the counts.
The best behaviors for a store miss and hit are then write-allocate and write-back with
eviction behavior again not playing a role in the counts of either hits or misses or even
the total cycle count. That is what, based on our data found below, we determined to be
the best cache setup to produce the lowest total cycle count and the fastest running cache
setup despite not being the one with the highest hit to misses ratio.


Cache Test Data -----------
./csim 512 512 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 3694383

./csim 512 256 8 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314571
Load misses: 3626
Store hits: 180111
Store misses: 17375
Total cycles: 4715883

./csim 256 512 8 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314571
Load misses: 3626
Store hits: 180111
Store misses: 17375
Total cycles: 4715883

./csim 1024 256 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 3694383

./csim 256 1024 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 3694383

./csim 2048 128 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 3694383

./csim 4096 64 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 3694383

./csim 64 4096 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 3694383

./csim 32 4096 8 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314571
Load misses: 3626
Store hits: 180111
Store misses: 17375
Total cycles: 4715883

./csim 4096 32 8 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314571
Load misses: 3626
Store hits: 180111
Store misses: 17375
Total cycles: 4715883

./csim 4096 16 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 315854
Load misses: 2343
Store hits: 188617
Store misses: 8869
Total cycles: 5004092

./csim 512 512 4 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 3694383

./csim 512 512 4 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 3694383

./csim 512 512 4 write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 23470339

./csim 512 512 4 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313766
Load misses: 4431
Store hits: 170130
Store misses: 27356
Total cycles: 23470339

./csim 512 512 4 no-write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 308743
Load misses: 9454
Store hits: 160016
Store misses: 37470
Total cycles: 21172213

./csim 512 512 4 no-write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 308743
Load misses: 9454
Store hits: 160016
Store misses: 37470
Total cycles: 21172213



Contributions ----
We decided to work on this project as much as we could together while
in a zoom session. As such we both contributed an equal amount
to the development of this homework and at this current (2nd) milestone.
The only things we did separately was just general bug fixing but not
much progress was made after the initial completion of the homework.

03/31/2022 -

Like in the previous contributions, the work was devided equally among both
partners with only minor corrections being done at the discretion of each individual's
free time. Overall, most of the progresss was made jointly through a zoom call
resultingin an even ditribution of work and effort for Milestone 3.

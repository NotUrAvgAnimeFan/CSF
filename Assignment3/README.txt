README for CSF Assignment 3 Milestone 2
Conains contributions

--Ricardo Morales Gonzalez rmorale5@jhu.edu
--Ana Kuri akuri1@jhu.edu

Best Cache Configuration ----

Single set & block ---------------
1 1 1024 write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 166960
Load misses: 151237
Store hits: 155539
Store misses: 41947
Total cycles: 670849334

1 1 1024 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 166960
Load misses: 151237
Store hits: 155539
Store misses: 41947
Total cycles: 670849334

1 1 1024 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 166960
Load misses: 151237
Store hits: 155539
Store misses: 41947
Total cycles: 1587543367

1 1 1024 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 166960
Load misses: 151237
Store hits: 155539
Store misses: 41947
Total cycles: 1587506787


1 1 1024 no-write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 169741
Load misses: 148456
Store hits: 67842
Store misses: 129644
Total cycles: 3820608239

1 1 1024 no-write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 169741
Load misses: 148456
Store hits: 67842
Store misses: 129644
Total cycles: 3820608239

Direct mapped -------------

8 1 1024 write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 270621
Load misses: 47576
Store hits: 187015
Store misses: 10471
Total cycles: 1506277954

8 1 1024 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 270621
Load misses: 47576
Store hits: 187015
Store misses: 10471
Total cycles: 1506277954

8 1 1024 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 270621
Load misses: 47576
Store hits: 187015
Store misses: 10471
Total cycles: 1851281931


8 1 1024 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 270621
Load misses: 47576
Store hits: 187015
Store misses: 10471
Total cycles: 1851267683


8 1 1024 no-write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 270573
Load misses: 47624
Store hits: 161700
Store misses: 35786
Total cycles: 1239402897


8 1 1024 no-write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 270573
Load misses: 47624
Store hits: 161700
Store misses: 35786
Total cycles: 1239402897

fully Associative ------
1 8 1024 write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 295415
Load misses: 22782
Store hits: 193165
Store misses: 4321
Total cycles: 714105404

1 8 1024 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 289893
Load misses: 28304
Store hits: 191487
Store misses: 5999
Total cycles: 898427082

1 8 1024 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 295415
Load misses: 22782
Store hits: 193165
Store misses: 4321
Total cycles: 893144248

1 8 1024 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 289893
Load misses: 28304
Store hits: 191487
Store misses: 5999
Total cycles: 1197622883

1 8 1024 no-write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 293904
Load misses: 24257
Store hits: 171903
Store misses: 25583
Total cycles: 641217900

1 8 1024 no-write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 288603
Load misses: 29594
Store hits: 167474
Store misses: 30012
Total cycles: 777840671

m-way set-associative --------------
8 8 1024 write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317538
Load misses: 659
Store hits: 197133
Store misses: 353
Total cycles: 46171836

8 8 1024 write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317132
Load misses: 1065
Store hits: 197078
Store misses: 408
Total cycles: 57973491

8 8 1024 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317538
Load misses: 659
Store hits: 197133
Store misses: 353
Total cycles: 38557757

8 8 1024 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317132
Load misses: 1065
Store hits: 197078
Store misses: 408
Total cycles: 56016483

8 8 1024 no-write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317413
Load misses: 784
Store hits: 187114
Store misses: 10372
Total cycles: 40324311

8 8 1024 no-write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317059
Load misses: 1138
Store hits: 185796
Store misses: 11690
Total cycles: 49385393



Contributions ----
We decided to work on this project as much as we could together while
in a zoom session. As such we both contributed an equal amount
to the development of this homework and at this current (2nd) milestone.
The only things we did separately was just general bug fixing but not
much progress was made after the initial completion of the homework.

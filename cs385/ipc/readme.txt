Author: Shane Lopez |
Course: CS385       |
Project: 2, IPC     |
--------------------*

Files
-----------
1) readme.txt (this file here!)
2) master.cpp
3) worker.cpp
4) msg.h
5) makefile

Running
-----------
1) $ unzip slopez23CS385HW2.zip
2) $ make -f makefile
3) $ ./master a b c d e f
	a = # Buffers - Must be prime and < 32
	b = # Workers - Must be < a
	c = Min Sleep Val - Must be >= 0
	d = Max Sleep Val - Must be > c
	e = Random Seed 
	f = -lock or -nolock, determines whether to use semaphores

Summary
-----------
The purpose of this assignment is to illustrate race conditions experienced by processes competing for access to resources; master launches a specified number of worker instances which then read from and write to a shared memory segment of buffers, reporting any errors along the way. With semaphores engaged, no errors occur as buffers being examined or written to by workers are "locked"; without semaphores read and write errors are expected. Workers communicate with the master process via a message queue which utilizes a custom message struct, defined by msg.h (included in slopez23CS385HW2.zip).

Known Bugs
-----------
* On first run, mssgget will report success and program will exit; on subsequent runs program will execute as expected.
* In order to specify the -lock option, a random seed must be provided.
* Erroneous output regardless of semaphores for case: ./master 3 2 c d e f

~Fin~

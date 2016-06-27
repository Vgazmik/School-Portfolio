Course:     CS450 			|
Assignment: HW 5, Selective Repeat      |
Author:     Shane Lopez			|
-----------------------------------------

Build
----------
To compile programs client and server, run:
	
	$ make

Run
----------
On a remote machine (or a separate directory):
	
	$ ./server <port>

		* port defaults to 54323 if not specified

On your host machine (or a separate directory):

	$ ./client <server address> <port> <filename>
	
		* all fields must be specified

Notes
----------
	* The client transmits a file to the server using selective 
	 repeat UDP protocol.
	* At present, client only transmits .txt files properly, this is being
	 looked into.

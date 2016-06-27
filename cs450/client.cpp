/*  client.cpp

    This program transfers a file to a remote server, possibly through an
    intermediary relay.  The server will respond with an acknowledgement.
    
    This program then calculates the round-trip time from the start of
    transmission to receipt of the acknowledgement, and reports that along
    with the average overall transmission rate.
    
    Written by Shane Lopez for CS 450 HW1 January 2014
*/

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>

#include "CS450Header.h"
#define N_SZ 100

using namespace std;

int main(int argc, char *argv[])
{
    // User Input
    if(argc < 4) {
    	cout<<"Usage: [server name] [port #]"
	    <</*" [relay name] [port #] */" [file name]" << endl;
	return -1;
    }
    
    char name[N_SZ];
    socklen_t addl;
    char *serv = argv[1];
    int port = atoi(argv[2]);
    char *fname = argv[3];
    char sname[N_SZ];
    int fd = 0;
    int fsz;
    struct stat file;
    unsigned char *map;
    struct hostent *hent , *sent;
    struct sockaddr_in saddr;
    struct sockaddr_in laddr;
    struct in_addr sip;
    int sd = 0;
    time_t bt,at;
    CS450Header hs,hr;    
    
    /* Check for the following from command-line args, or ask the user:
	        
        Destination ( server ) name and port number
        Relay name and port number.  If none, communicate directly with server
        File to transfer.  Use OPEN(2) to verify that the file can be opened
        for reading, and ask again if necessary.
    */
    if(gethostname(name, N_SZ) < 0) {
    	perror("gethostname()");
        return -1;
    }
    
    hent = gethostbyname(name);
    sent = gethostbyname(serv);//sname);
    // Use FSTAT and MMAP to map the file to a memory buffer.  That will let the
    // virtual memory system page it in as needed instead of reading it byte
    // by byte.
    
    if((fd = open(fname,O_RDONLY)) < 0) {
    	perror("open()");
	return -1;
    }
    
    if(fstat(fd, &file) < 0) {
    	perror("fstat()");
	return -1;
    }
    cout << "\nFile: " 
         << fname 
         << " opened successfully\n" 
         << endl;
    fsz = file.st_size;
    map = (unsigned char*)mmap(0, fsz, PROT_READ, MAP_SHARED, fd, 0);
    void *addr = (void*)map;
    
    //close(fd);
    
    // Open a Connection to the server ( or relay )  TCP for the first HW
    // call SOCKET and CONNECT and verify that the connection opened.
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	perror("socket()");
	munmap(addr,fsz);
        return -1;
    }
    
    addl = sizeof(struct sockaddr);
    
    if(getsockname(sd,(struct sockaddr *)&laddr, &addl) < 0){
    	perror("getsockname()");
        munmap(addr,fsz);
        return -1; 	
    }
    
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(serv);
    saddr.sin_port = htons(port);
    
    // Note the time before beginning transmission
    bt = time(0);
    
    cout << "Time Before Transmission: " << ctime(&bt) << "\n" << endl;
    
    if(connect(sd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
    	perror("connect()");
	munmap(addr,fsz);
        return -1;
    }
    
    cout << "Connected Successfully!\n" << endl;
    
    // Create a CS450Header object, fill in the fields, and use SEND to write
    // it to the socket.
    hs.version = htonl(4);
    hs.UIN = htonl(654218587);
    hs.HW_number = htonl(1);
    hs.transactionNumber = htonl(1);
    memset(hs.ACCC,' ',sizeof(hs.ACCC));
    strncpy(hs.ACCC, "slopez23", sizeof("slopez23"));
    memset(hs.filename,' ',sizeof(hs.filename));
    strncpy(hs.filename, fname, sizeof(fname));
    hs.from_IP = htonl(inet_addr(hent->h_addr));
    hs.to_IP = htonl(inet_addr("54.213.83.180"));
    hs.packetType = htonl(1);
    hs.nbytes = htonl((u_int32_t)fsz);
    hs.relayCommand = htonl(0);
    hs.persistent = htonl(0);
    hs.saveFile = htonl(1);
    hs.from_Port = htons(laddr.sin_port);
    hs.to_Port = htons(54321);
    //hs.trueFromIP = ;
    //hs.trueToIP = ;
     
    // Use SEND to send the data file.  If it is too big to send in one gulp
    // Then multiple SEND commands may be necessary.
    
    send(sd, (char *)&hs, sizeof(hs), 0);
    // Use RECV to read in a CS450Header from the server, which should contain
    // some acknowledgement information.  
    //send(sd, &map, sizeof(map), 0);
    for(int k = 0; k < fsz; k++){write(sd, &map[k], sizeof(unsigned char));}
    recv(sd, (char *)&hr, sizeof(hr), 0);
    
    cout << "Received Packet Type: " << ntohl(hr.packetType) << "\n"
         << endl;
    
    hs.transactionNumber++;
    hs.packetType = 2;
    hs.relayCommand = htonl(1);
    
    send(sd, (char *)&hs, sizeof(hs), 0);
    
    close(sd);
    
    // Calculate the round-trip time and
    // bandwidth, and report them to the screen along with the size of the file
    // and output echo of all user input data.
    
    at = time(NULL);
    
    cout << "Time After Transmission: " << ctime(&at) << "\n" << endl;
    
    cout << "Time Elapsed: " << difftime(at,bt) << " seconds \n" << endl;
    
    if(ntohl(hr.packetType) == 2) {
    	cout << "<Success>\n"
             << "File \"" << hs.filename 
             << "\" sent from Client " << name 
             << " to Server at " << serv 
             << " on port " << port 
             << endl;
    }
    else {
    	cout << "<Failure>\n"
             << "File \"" << hs.filename 
             << "\" not sent from Client " << name 
             << " to Server at " << serv 
             << " on port " << port 
             << endl;
    }
    
    // When the job is done, either the client or the server must transmit a
    // CS450Header containing the command code to close the connection, and 
    // having transmitted it, close their end of the socket.  If a relay is 
    // involved, it will transmit the close command to the other side before
    // closing its connections.  Whichever process receives the close command
    // should close their end and also all open data files at that time.
    
    // If it is desired to transmit another file, possibly using a different
    // destination, protocol, relay, and/or other parameters, get the
    // necessary input from the user and try again.
    
    // When done, report overall statistics and exit.
    
    //system("PAUSE");
    munmap(addr,fsz);
    close(fd);
    cout << "Connection Closed!\n" << endl;
    
    return EXIT_SUCCESS;
}

